/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROPERTIESGROUP_H
#define	PROPERTIESGROUP_H

#include "PropertiesAbstract.h"

class GroupObj;

class PropertiesGroup : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesGroup();
    virtual ~PropertiesGroup();
    bool support(GameObj* obj);
    void showObj(GameObj* obj);
    void updateObj(GameObj* obj);
    
public slots:
    void enableCustomDetailLevelEnabled(Qt::CheckState val);
    void customDetailLevelEdited(QString val);
    void checkboxAnimEdited(Qt::CheckState val);
    void checkboxTerrainEdited(Qt::CheckState val);
    void cShadowTypeEdited(int val);
    void chIndividualRotationEdited(Qt::CheckState val);
    void editPositionYEnabled(QString val);
    
signals:
    
private:
    QCheckBox checkboxAnim;
    QCheckBox checkboxTerrain;
    QCheckBox chSeparateRotation;
    QComboBox cShadowType;
};

#endif	/* PROPERTIESGROUP_H */

