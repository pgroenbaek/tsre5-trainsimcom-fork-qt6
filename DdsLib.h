/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors.
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef DDSLIB_H
#define DDSLIB_H

#include <QThread>
#include "Texture.h"
#include <cstdint>
#include <vector>

class DdsLib : public QThread
{
    Q_OBJECT

public:
    DdsLib();
    static bool IsThread;
    Texture* texture;
    void run() override;

private:
    void decodeDXT1(const uint8_t* block, std::vector<unsigned char>& out, int bx, int by, int width);
    void decodeDXT3(const uint8_t* block, std::vector<unsigned char>& out, int bx, int by, int width);
    void decodeDXT5(const uint8_t* block, std::vector<unsigned char>& out, int bx, int by, int width);

protected:

};

#endif // DDSLIB_H
