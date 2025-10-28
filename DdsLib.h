/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors.
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  This file was added as part of the upgrade to Qt6.
 *  Because QImage no longer supports the DDS format in Qt6.
 *  Copyright (C) 2025 Peter Grønbæk Andersen <peter@grnbk.io>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef DDSLIB_H
#define DDSLIB_H

#include <QThread>
#include "Texture.h"

class DdsLib : public QThread
{
    Q_OBJECT

public:
    DdsLib();
    static bool IsThread;
    Texture* texture;
    void run();
private:
    
protected:

};

#endif // DDSLIB_H
