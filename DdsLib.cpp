/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors.
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

/*#include "DdsLib.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include "Game.h"
#include <OpenImageIO/imageio.h>
#include <memory>

OIIO_NAMESPACE_USING

bool DdsLib::IsThread = true;

DdsLib::DdsLib() {}

void DdsLib::run() {
    std::string filename = texture->pathid.toStdString();
    std::unique_ptr<ImageInput> in(ImageInput::open(filename));

    if (!in) {
        if (!IsThread) {
            texture->missing = true;
            if (Game::debugOutput)
                qDebug() << "DDS: not exist or cannot open " << texture->pathid;
        }
        return;
    }

    const ImageSpec &spec = in->spec();
    int width = spec.width;
    int height = spec.height;
    int channels = spec.nchannels;

    if (channels < 3) {
        // Force at least RGB
        if (Game::debugOutput)
            qDebug() << "DDS: image has less than 3 channels " << texture->pathid;
        channels = 3;
    }

    std::vector<unsigned char> pixels(width * height * channels);
    if (!in->read_image(0, 0, 0, channels, TypeDesc::UINT8, &pixels[0])) {
        if (!IsThread) {
            texture->missing = true;
            qDebug() << "DDS: failed to read image " << texture->pathid;
        }
        return;
    }
    in->close();

    texture->width = width;
    texture->height = height;
    texture->bytesPerPixel = channels;

    if (channels == 4) {
        texture->type = GL_RGBA;
    } else {
        texture->type = GL_RGB;
    }

    // Copy pixel data
    texture->imageData = new unsigned char[width * height * channels];
    memcpy(texture->imageData, pixels.data(), width * height * channels);

    texture->loaded = true;
    texture->editable = true;

    if (!IsThread) {
        qDebug() << "tex:" << texture->pathid
                 << " " << width << "x" << height
                 << " channels:" << channels;
    }

    return;
}*/

#include "DdsLib.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include "Game.h"
#include <fstream>
#include <vector>
#include <cstring>
#include <cassert>
#include <GL/gl.h>

#pragma pack(push, 1)
struct DDS_PIXELFORMAT {
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwFourCC;
    uint32_t dwRGBBitCount;
    uint32_t dwRBitMask;
    uint32_t dwGBitMask;
    uint32_t dwBBitMask;
    uint32_t dwABitMask;
};

struct DDS_HEADER {
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwHeight;
    uint32_t dwWidth;
    uint32_t dwPitchOrLinearSize;
    uint32_t dwDepth;
    uint32_t dwMipMapCount;
    uint32_t dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    uint32_t dwCaps;
    uint32_t dwCaps2;
    uint32_t dwCaps3;
    uint32_t dwCaps4;
    uint32_t dwReserved2;
};
#pragma pack(pop)

// DDS flags
#define DDSD_CAPS 0x1
#define DDSD_HEIGHT 0x2
#define DDSD_WIDTH 0x4
#define DDSD_PITCH 0x8
#define DDSD_PIXELFORMAT 0x1000
#define DDSD_MIPMAPCOUNT 0x20000
#define DDSD_LINEARSIZE 0x80000
#define DDSD_DEPTH 0x800000

// Pixel format flags
#define DDPF_ALPHAPIXELS 0x1
#define DDPF_FOURCC 0x4
#define DDPF_RGB 0x40

// Compression FourCC
#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

bool DdsLib::IsThread = true;

DdsLib::DdsLib() {}

void DdsLib::run() {
    std::string filename = texture->pathid.toStdString();
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        if (!IsThread) {
            texture->missing = true;
            if (Game::debugOutput)
                qDebug() << "DDS: not exist or cannot open " << texture->pathid;
        }
        return;
    }

    // Read magic number
    char magic[4];
    file.read(magic, 4);
    if (strncmp(magic, "DDS ", 4) != 0) {
        if (!IsThread) {
            texture->missing = true;
            qDebug() << "DDS: invalid magic " << texture->pathid;
        }
        return;
    }

    // Read DDS header
    DDS_HEADER header;
    file.read(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));

    int width = header.dwWidth;
    int height = header.dwHeight;
    int mipMapCount = (header.dwFlags & DDSD_MIPMAPCOUNT) ? header.dwMipMapCount : 1;

    GLenum format = 0;
    bool compressed = false;
    int blockSize = 16;

    if (header.ddspf.dwFlags & DDPF_FOURCC) {
        compressed = true;
        switch (header.ddspf.dwFourCC) {
            case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; blockSize = 8; break;
            case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
            case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
            default:
                qDebug() << "DDS: unsupported compression " << texture->pathid;
                texture->missing = true;
                return;
        }
    } else if (header.ddspf.dwFlags & DDPF_RGB) {
        format = (header.ddspf.dwRGBBitCount == 32) ? GL_RGBA : GL_RGB;
        compressed = false;
    } else {
        qDebug() << "DDS: unsupported format " << texture->pathid;
        texture->missing = true;
        return;
    }

    size_t dataSize = 0;
    if (compressed) {
        // Compressed: calculate size of all mip levels
        int w = width, h = height;
        for (int i = 0; i < mipMapCount; i++) {
            int size = ((w+3)/4) * ((h+3)/4) * blockSize;
            dataSize += size;
            w = std::max(1, w/2);
            h = std::max(1, h/2);
        }
    } else {
        dataSize = width * height * ((header.ddspf.dwRGBBitCount) / 8);
    }

    std::vector<unsigned char> pixels(dataSize);
    file.read(reinterpret_cast<char*>(pixels.data()), dataSize);
    file.close();

    texture->width = width;
    texture->height = height;
    texture->bytesPerPixel = (header.ddspf.dwRGBBitCount) / 8;
    texture->type = format;
    texture->imageData = new unsigned char[dataSize];
    memcpy(texture->imageData, pixels.data(), dataSize);
    texture->loaded = true;
    texture->editable = true;

    if (!IsThread) {
        qDebug() << "DDS:" << texture->pathid
                 << width << "x" << height
                 << "mipmaps:" << mipMapCount
                 << "compressed:" << compressed;
    }
}
