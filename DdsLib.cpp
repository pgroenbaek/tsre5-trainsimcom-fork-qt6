/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors.
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "DdsLib.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
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

#define DDSD_CAPS 0x1
#define DDSD_HEIGHT 0x2
#define DDSD_WIDTH 0x4
#define DDSD_PITCH 0x8
#define DDSD_PIXELFORMAT 0x1000
#define DDSD_MIPMAPCOUNT 0x20000
#define DDSD_LINEARSIZE 0x80000
#define DDSD_DEPTH 0x800000

#define DDPF_ALPHAPIXELS 0x1
#define DDPF_FOURCC 0x4
#define DDPF_RGB 0x40

#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

bool DdsLib::IsThread = true;

DdsLib::DdsLib() {}

static void fillColor(unsigned char* dst, uint32_t color, bool hasAlpha, uint8_t alpha = 255) {
    dst[0] = (color & 0xFF);
    dst[1] = (color >> 8) & 0xFF;
    dst[2] = (color >> 16) & 0xFF;
    dst[3] = hasAlpha ? alpha : 255;
}

void DdsLib::decodeDXT1(const uint8_t* block, std::vector<unsigned char>& out, int bx, int by, int width) {
    uint16_t c0 = block[0] | (block[1] << 8);
    uint16_t c1 = block[2] | (block[3] << 8);

    unsigned char r[4], g[4], b[4];
    r[0] = ((c0 >> 11) & 0x1F) << 3;
    g[0] = ((c0 >> 5) & 0x3F) << 2;
    b[0] = (c0 & 0x1F) << 3;

    r[1] = ((c1 >> 11) & 0x1F) << 3;
    g[1] = ((c1 >> 5) & 0x3F) << 2;
    b[1] = (c1 & 0x1F) << 3;

    if (c0 > c1) {
        r[2] = (2*r[0]+r[1])/3; g[2] = (2*g[0]+g[1])/3; b[2] = (2*b[0]+b[1])/3;
        r[3] = (r[0]+2*r[1])/3; g[3] = (g[0]+2*g[1])/3; b[3] = (b[0]+2*b[1])/3;
    } else {
        r[2] = (r[0]+r[1])/2; g[2] = (g[0]+g[1])/2; b[2] = (b[0]+b[1])/2;
        r[3] = 0; g[3] = 0; b[3] = 0;
    }

    uint32_t indices = block[4] | (block[5]<<8) | (block[6]<<16) | (block[7]<<24);

    for(int j=0;j<4;j++) {
        for(int i=0;i<4;i++) {
            int idx = (indices >> (2*(4*j+i))) & 0x03;
            int px = bx*4 + i;
            int py = by*4 + j;
            if(px < width && py < out.size()/width/4) {
                unsigned char* dst = &out[(py*width+px)*4];
                dst[0] = r[idx];
                dst[1] = g[idx];
                dst[2] = b[idx];
            }
        }
    }
}


void DdsLib::decodeDXT3(const uint8_t* block, std::vector<unsigned char>& out, int bx, int by, int width) {
    for(int j=0;j<4;j++) {
        uint16_t rowAlpha = block[j*2] | (block[j*2+1]<<8);
        for(int i=0;i<4;i++) {
            int alpha = ((rowAlpha >> (i*4)) & 0xF) * 17;
            int px = bx*4 + i;
            int py = by*4 + j;
            if(px < width && py < out.size()/width/4) {
                unsigned char* dst = &out[(py*width+px)*4];
                dst[3] = alpha;
            }
        }
    }
    decodeDXT1(block+8, out, bx, by, width);
}


void DdsLib::decodeDXT5(const uint8_t* block, std::vector<unsigned char>& out, int bx, int by, int width) {
    uint8_t alpha0 = block[0];
    uint8_t alpha1 = block[1];

    uint64_t alphaBits = 0;
    for(int i=0;i<6;i++) alphaBits |= (uint64_t(block[2+i]) << (8*i));

    for(int i=0;i<16;i++) {
        int shift = i*3;
        uint8_t index = (alphaBits >> shift) & 0x7;

        int alphaVal = 0;
        if(index==0) alphaVal = alpha0;
        else if(index==1) alphaVal = alpha1;
        else if(alpha0 > alpha1) alphaVal = ((8-index)*alpha0 + (index-1)*alpha1)/7;
        else alphaVal = ((6-index)*alpha0 + (index-1)*alpha1)/5;

        int px = bx*4 + (i%4);
        int py = by*4 + (i/4);
        if(px < width && py < out.size()/width/4) {
            out[(py*width+px)*4 + 3] = alphaVal;
        }
    }
    decodeDXT1(block+8, out, bx, by, width);
}


void DdsLib::run() {
    std::string filename = texture->pathid.toStdString();
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        texture->missing = true;
        texture->loaded = false;
        return;
    }

    char magic[4];
    file.read(magic, 4);
    if (strncmp(magic, "DDS ", 4) != 0) {
        texture->error = true;
        texture->loaded = false;
        return;
    }

    DDS_HEADER header;
    file.read(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));

    int width = header.dwWidth;
    int height = header.dwHeight;

    uint32_t fourcc = header.ddspf.dwFourCC;
    bool isCompressed = (header.ddspf.dwFlags & DDPF_FOURCC);

    int blockSize = (fourcc == FOURCC_DXT1 ? 8 : 16);
    size_t dataSize = ((width+3)/4) * ((height+3)/4) * blockSize;

    std::vector<uint8_t> data(dataSize);
    file.read((char*)data.data(), dataSize);
    file.close();

    std::vector<unsigned char> out(width * height * 4, 0);

    if (!isCompressed) {
        memcpy(out.data(), data.data(), width * height * (header.ddspf.dwRGBBitCount/8));
    } else {
        int bx = (width + 3) / 4;
        int by = (height + 3) / 4;

        const uint8_t* ptr = data.data();

        for (int y = 0; y < by; y++) {
            for (int x = 0; x < bx; x++) {
                switch (fourcc) {
                    case FOURCC_DXT1: decodeDXT1(ptr, out, x, y, width); break;
                    case FOURCC_DXT3: decodeDXT3(ptr, out, x, y, width); break;
                    case FOURCC_DXT5: decodeDXT5(ptr, out, x, y, width); break;
                }
                ptr += blockSize;
            }
        }
    }

    texture->width = width;
    texture->height = height;
    
    if (isCompressed) {
        texture->bytesPerPixel = 4;
        texture->type = GL_RGBA;
    } else {
        if (header.ddspf.dwRGBBitCount == 32) {
            texture->bytesPerPixel = 4;
            texture->type = GL_RGBA;
        } else if (header.ddspf.dwRGBBitCount == 24) {
            texture->bytesPerPixel = 3;
            texture->type = GL_RGB;
        } else {
            texture->bytesPerPixel = 4;
            texture->type = GL_RGBA;
        }
    }

    texture->imageData = new unsigned char[width * height * 4];
    memcpy(texture->imageData, out.data(), width * height * 4);

    texture->loaded = true;
    texture->editable = false;
}