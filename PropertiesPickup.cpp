/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesPickup.h"
#include "PickupObj.h"
#include "GuiFunct.h"
#include "Game.h"

PropertiesPickup::PropertiesPickup() {
    QDoubleValidator* doubleValidator = new QDoubleValidator(-1000000, 1000000, 6, this);
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);

    cPickupType.addItem("Freight - grain");
    cPickupType.addItem("Freight - coal");
    cPickupType.addItem("Freight - gravel");
    cPickupType.addItem("Freight - sand");
    cPickupType.addItem("Fuel - water");
    cPickupType.addItem("Fuel - coal");
    cPickupType.addItem("Fuel - diesel");
    cPickupType.addItem("Special - mail");
    cPickupType.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&cPickupType, &QComboBox::currentIndexChanged,
        this, &PropertiesPickup::cPickupTypeEdited);

    cAnimType.addItem("Activated animation only");
    cAnimType.addItem("Proximity animation only");
    cAnimType.addItem("Activated effects only");
    cAnimType.addItem("Activated anim. and effects");
    cAnimType.addItem("Proximity anim. and effects");
    cAnimType.addItem("On empty animation only");
    cAnimType.addItem("Fuel hose only");
    cAnimType.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&cAnimType, &QComboBox::currentIndexChanged,
        this, &PropertiesPickup::cAnimTypeEdited);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0, 1, 1, 1);
    infoLabel = new QLabel("Pickup:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3, 0, 0, 0);
    vbox->addWidget(infoLabel);

    QLabel *label = new QLabel("Filename:");
    label->setContentsMargins(3, 0, 0, 0);
    vbox->addWidget(label);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    label = new QLabel("Filename - broken:");
    label->setContentsMargins(3, 0, 0, 0);
    vbox->addWidget(label);
    this->eBrokenFileName.setDisabled(true);
    this->eBrokenFileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->eBrokenFileName);
    label = new QLabel("Properties:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3, 0, 0, 0);
    vbox->addWidget(label);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3, 0, 3, 0);
    label = new QLabel("Type:");
    label->setContentsMargins(3, 0, 0, 0);
    vbox->addWidget(label);
    vbox->addWidget(&cPickupType);
    vlist->addRow("Capacity:", &eCapacity);
    eCapacity.setValidator(doubleValidator);
    QObject::connect(&eCapacity, &QLineEdit::textEdited,
        this, &PropertiesPickup::eCapacityEnabled);

    vlist->addRow("Content:", &eContent);
    eContent.setValidator(doubleValidator);
    QObject::connect(&eContent, &QLineEdit::textEdited,
        this, &PropertiesPickup::eContentEnabled);

    vlist->addRow("Fill rate:", &eFill);
    eFill.setValidator(doubleValidator);
    QObject::connect(&eFill, &QLineEdit::textEdited,
        this, &PropertiesPickup::eFillEnabled);

    vlist->addRow("Speed min:", &eSpeedMin);
    eSpeedMin.setValidator(doubleValidator);
    QObject::connect(&eSpeedMin, &QLineEdit::textEdited,
        this, &PropertiesPickup::eSpeedMinEnabled);

    vlist->addRow("Speed max:", &eSpeedMax);
    eSpeedMax.setValidator(doubleValidator);
    QObject::connect(&eSpeedMax, &QLineEdit::textEdited,
        this, &PropertiesPickup::eSpeedMaxEnabled);

    vbox->addItem(vlist);
    label = new QLabel("Anim type:");
    label->setContentsMargins(3, 0, 0, 0);
    vbox->addWidget(label);
    vbox->addWidget(&cAnimType);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3, 0, 3, 0);
    vlist->addRow("Anim lenght:", &eAnimLength);
    eAnimLength.setValidator(doubleValidator);
    QObject::connect(&eAnimLength, &QLineEdit::textEdited,
        this, &PropertiesPickup::eAnimLengthEnabled);

    vbox->addItem(vlist);
    vbox->addWidget(&chInfinite);
    vbox->addWidget(&chBroken);
    chInfinite.setText("Infinite capacity");
    QObject::connect(&chInfinite, &QCheckBox::checkStateChanged,
        this, &PropertiesPickup::chInfiniteEnabled);
    chBroken.setText("Broken by default");
    QObject::connect(&chBroken, &QCheckBox::checkStateChanged,
        this, &PropertiesPickup::chBrokenEnabled);
    vbox->addStretch(1);
    this->setLayout(vbox);

}

PropertiesPickup::~PropertiesPickup() {
}

void PropertiesPickup::showObj(GameObj* obj) {
    if (obj == NULL) {
        infoLabel->setText("NULL");
        return;
    }
    worldObj = (WorldObj*)obj;
    pobj = (PickupObj*) obj;

    this->infoLabel->setText("Object: " + pobj->type);
    this->fileName.setText(pobj->fileName);
    this->eBrokenFileName.setText(pobj->fileName.split('.')[0] + "_d.s");
    cPickupType.blockSignals(true);
    cPickupType.setCurrentIndex(pobj->getTypeId());
    cPickupType.blockSignals(false);
    eCapacity.setText(QString::number(pobj->getCapacity()));
    eContent.setText(QString::number(pobj->getPickupContent()));
    eFill.setText(QString::number(pobj->getFillRate()));
    eSpeedMin.setText(QString::number(pobj->getSpeedMin()));
    eSpeedMax.setText(QString::number(pobj->getSpeedMax()));
    cAnimType.blockSignals(true);
    cAnimType.setCurrentIndex(pobj->getAnimTypeId());
    cAnimType.blockSignals(false);
    eAnimLength.setText(QString::number(pobj->getAnimLength()));
    this->chInfinite.blockSignals(true);
    chInfinite.setChecked((pobj->isInfinite()));
    this->chInfinite.blockSignals(false);
    this->chBroken.blockSignals(true);
    this->chBroken.setChecked(pobj->isBroken());
    this->chBroken.blockSignals(false);
}

void PropertiesPickup::cPickupTypeEdited(int val) {
    if (pobj == NULL) {
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    pobj->setTypeId(val);
}

void PropertiesPickup::cAnimTypeEdited(int val) {
    if (pobj == NULL) {
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    pobj->setAnimTypeId(val);
}

void PropertiesPickup::eCapacityEnabled(QString val){
    if (pobj == NULL) {
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        pobj->setCapacity(fval);
    }
}

void PropertiesPickup::eContentEnabled(QString val){
    if (pobj == NULL) {
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::StateBegin();
        Undo::PushGameObjData(worldObj);
        Undo::PushTrackDB(Game::trackDB);
        pobj->setPickupContent(fval);
        Undo::StateEnd();
    }
}

void PropertiesPickup::eFillEnabled(QString val){
    if (pobj == NULL) {
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        pobj->setFillRate(fval);
    }
}

void PropertiesPickup::eSpeedMinEnabled(QString val){
    if (pobj == NULL) {
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        pobj->setSpeedMin(fval);
    }
}

void PropertiesPickup::eSpeedMaxEnabled(QString val){
    if (pobj == NULL) {
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        pobj->setSpeedMax(fval);
    }
}

void PropertiesPickup::eAnimLengthEnabled(QString val){
    if (pobj == NULL) {
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        pobj->setAnimLength(fval);
    }
}

void PropertiesPickup::chInfiniteEnabled(Qt::CheckState val){
    if (pobj == NULL) {
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    if(val == Qt::Checked){
        pobj->setInfinite(true);
    } else {
        pobj->setInfinite(false);
    }
}

void PropertiesPickup::chBrokenEnabled(Qt::CheckState val){
    if (pobj == NULL) {
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    if(val == Qt::Checked){
        pobj->setBroken(true);
    } else {
        pobj->setBroken(false);
    }
}

bool PropertiesPickup::support(GameObj* obj) {
    if (obj == NULL)
        return false;
    if(obj->typeObj != GameObj::worldobj)
        return false;
    if(((WorldObj*)obj)->type == "pickup")
        return true;
    return false;
}