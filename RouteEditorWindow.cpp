/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QtWidgets>
#include <QStatusBar>
#include "RouteEditorGLWidget.h"
#include "RouteEditorWindow.h"
#include "Game.h"
#include "AceLib.h"
#include <QDebug>
#include "GuiFunct.h"
#include "ObjTools.h"
#include "TerrainTools.h"
#include "GeoTools.h"
#include "ActivityTools.h"
#include "NaviBox.h"
#include "ShapeViewWindow.h"
#include "AboutWindow.h"
#include "SoundManager.h"
#include "PropertiesAbstract.h"
#include "PropertiesUndefined.h"
#include "PropertiesStatic.h"
#include "PropertiesTransfer.h"
#include "PropertiesPlatform.h"
#include "PropertiesSiding.h"
#include "PropertiesCarspawner.h"
#include "PropertiesDyntrack.h"
#include "PropertiesSignal.h"
#include "PropertiesPickup.h"
#include "PropertiesForest.h"
#include "PropertiesSoundSource.h"
#include "PropertiesSpeedpost.h"
#include "PropertiesTrackObj.h"
#include "PropertiesGroup.h"
#include "PropertiesRuler.h"
#include "PropertiesLevelCr.h"
#include "PropertiesSoundRegion.h"
#include "PropertiesTerrain.h"
#include "PropertiesActivityObject.h"
#include "PropertiesTrackItem.h"
#include "PropertiesActivityPath.h"
#include "PropertiesConsist.h"
#include "Ref.h"
#include "NaviWindow.h"
#include "StatusWindow.h"
#include "ErrorMessagesWindow.h"
#include "ClientUsersWindow.h"
#include "ErrorMessagesLib.h"
#include "UnsavedDialog.h"
#include "ActivityEventWindow.h"
#include "ActivityEventProperties.h"
#include "ActivityServiceWindow.h"
#include "ActivityServiceProperties.h"
#include "ActivityTrafficWindow.h"
#include "ActivityTrafficProperties.h"
#include "ActivityTimetableWindow.h"
#include "ActivityTimetableProperties.h"
#include "RouteEditorClient.h"
#include "Route.h"
#include "LoadWindow.h"
#include "CELoadWindow.h"
#include "TexLib.h"
#include "PropertiesPolyForest.h"
#include "PropertiesHazard.h"



RouteEditorWindow::RouteEditorWindow() {

    objTools = new ObjTools("ObjTools");
    terrainTools = new TerrainTools("TerrainTools");
    geoTools = new GeoTools("GeoTools");
    activityTools = new ActivityTools("ActivityTools");
    //naviBox = new NaviBox();
    glWidget = new RouteEditorGLWidget(this);
    
    shapeViewWindow = new ShapeViewWindow(this);
    aboutWindow = new AboutWindow(this);
    naviWindow = new NaviWindow(this);
    statusWindow = new StatusWindow(this);
    
    
    errorMessagesWindow = ErrorMessagesLib::GetWindow(this);
    clientUsersWindow = new ClientUsersWindow(this);
    activityEventWindow = new ActivityEventWindow(this);
    activityServiceWindow = new ActivityServiceWindow(this);
    activityTrafficWindow = new ActivityTrafficWindow(this);
    activityTimetableWindow = new ActivityTimetableWindow(this);
    
    objProperties["Static"] = new PropertiesStatic;
    objProperties["Transfer"] = new PropertiesTransfer;
    objProperties["Platform"] = new PropertiesPlatform;
    objProperties["Siding"] = new PropertiesSiding;
    objProperties["Carspawner"] = new PropertiesCarspawner;
    objProperties["Dyntrack"] = new PropertiesDyntrack;
    objProperties["Signal"] = new PropertiesSignal;
    objProperties["Pickup"] = new PropertiesPickup;
    objProperties["Forest"] = new PropertiesForest;
    objProperties["Speedpost"] = new PropertiesSpeedpost;
    objProperties["SoundSource"] = new PropertiesSoundSource;
    objProperties["TrackObj"] = new PropertiesTrackObj;
    objProperties["Group"] = new PropertiesGroup;
    objProperties["Ruler"] = new PropertiesRuler;
    objProperties["SoundRegion"] = new PropertiesSoundRegion;
    objProperties["LevelCr"] = new PropertiesLevelCr;
    objProperties["Terrain"] = new PropertiesTerrain;
    objProperties["ActivityObject"] = new PropertiesActivityObject;
    objProperties["TrackItem"] = new PropertiesTrackItem;
    objProperties["ActivityPath"] = new PropertiesActivityPath;
    objProperties["ActivityConsist"] = new PropertiesConsist;
    objProperties["Hazard"] = new PropertiesHazard;
    
    // last 
    objProperties["Undefined"] = new PropertiesUndefined;
    
    QWidget* remain = new QWidget();
    
    box = new QWidget(this);
    box2 = new QWidget(this);
    box->setFixedWidth(250);
    box->setWindowTitle("Tools Window");
    box2->setWindowFilePath("Properties Window");
    box2->setMaximumWidth(160);
    box2->setMinimumWidth(160);
    //box2->setMaximumWidth(250);
    //box2->setMinimumWidth(250);
    QHBoxLayout *mainLayout2 = new QHBoxLayout; 
    mainLayout2->setSpacing(0);
    mainLayout2->setContentsMargins(0,0,0,0);
    mainLayout2->addWidget(objTools);
    mainLayout2->addWidget(terrainTools);
    mainLayout2->addWidget(geoTools);
    if(Game::serverClient == NULL){
        mainLayout2->addWidget(activityTools);
    }
    //mainLayout2->addWidget(naviBox);
    //mainLayout2->setAlignment(naviBox, Qt::AlignBottom);
    box->setLayout(mainLayout2);
    
    
    QVBoxLayout *mainLayout3 = new QVBoxLayout;
    mainLayout3->setContentsMargins(0,0,0,0);
    mainLayout3->setSpacing(0);
    //mainLayout3->addWidget(propertiesUndefined);
    
    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        it->hide();
        //console.log(obj.type);
        mainLayout3->addWidget(it);
    }
    
    //mainLayout3->addWidget(terrainTools);
    //mainLayout3->setAlignment(naviBox, Qt::AlignBottom);
    box2->setLayout(mainLayout3);

    glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(3,3,3,3);
    mainLayout->setSpacing(3);
    
    QString mainWindowLayout = Game::mainWindowLayout;
    if(!mainWindowLayout.toUpper().contains('W')){
        mainWindowLayout += 'W';
    }
    for(int i = 0; i < mainWindowLayout.length(); i++){
        if(mainWindowLayout[i].toUpper() == 'P')
            mainLayout->addWidget(box2);
        if(mainWindowLayout[i].toUpper() == 'T')
            mainLayout->addWidget(box);
        if(mainWindowLayout[i].toUpper() == 'W')
            mainLayout->addWidget(glWidget);
    }
    if(!mainWindowLayout.toUpper().contains('T')){
        box->move(this->pos());
        box->setWindowFlags(Qt::WindowType::Tool);
    }
    if(!mainWindowLayout.toUpper().contains('P')){
        box2->move(this->pos());
        box2->setWindowFlags(Qt::WindowType::Tool);
    }
    
    remain->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    
    this->setCentralWidget(remain);
    setWindowTitle(Game::AppName+" "+Game::AppVersion+" Route Editor");
    
    // MENUBAR
    // EFO -- modifying many keystroke shortcuts to eliminate overlaps    
    // Route
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence("Shift+Ctrl+S"));
    QObject::connect(saveAction, &QAction::triggered, this, &RouteEditorWindow::save);

    createPathsAction = new QAction(tr("&Create Debug Paths"), this);
    QObject::connect(createPathsAction, &QAction::triggered, this, &RouteEditorWindow::createPaths);
    
    reloadRefAction = new QAction(tr("&Reload Ref File"), this);
    QObject::connect(reloadRefAction, &QAction::triggered, this, &RouteEditorWindow::reloadRef);
    
    reloadMkrAction = new QAction(tr("&Reload Mkr Files"), this);
    QObject::connect(reloadMkrAction, &QAction::triggered, this, &RouteEditorWindow::reloadMkr);

    reloadSettingsAction = new QAction(tr("&Reload Settings File"), this);
    QObject::connect(reloadSettingsAction, &QAction::triggered, this, &RouteEditorWindow::reloadSettings);
  
    closeAction = new QAction(tr("&Close"), this);
    QObject::connect(closeAction, &QAction::triggered, this, &RouteEditorWindow::close);
      
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    QObject::connect(exitAction, &QAction::triggered, this, &RouteEditorWindow::close);
    
    trkEditr = new QAction(tr("E&dit route settings"), this);
    QObject::connect(trkEditr, &QAction::triggered, glWidget, &RouteEditorGLWidget::showTrkEditr);
    
    rebuildAction = new QAction(tr("Re&build TDB (experimental)"), this);
    
    
    if(Game::serverClient == NULL){
        routeMenu = menuBar()->addMenu(tr("&Route"));
        routeMenu->addAction(saveAction);
        routeMenu->addAction(reloadRefAction);
        routeMenu->addAction(reloadMkrAction);   
        routeMenu->addAction(reloadSettingsAction);           
        routeMenu->addAction(createPathsAction);
        routeMenu->addAction(trkEditr);
        //routeMenu->addAction(rebuildAction);    // Not yet ready
        routeMenu->addAction(exitAction);
    } else {
        routeMenu = menuBar()->addMenu(tr("&Server"));
        routeMenu->addAction(exitAction);
    }
    // Edit
    editMenu = menuBar()->addMenu(tr("&Edit"));
    if(Undo::UndoEnabled){
        undoAction = new QAction(tr("&Undo"), this); 
        undoAction->setShortcut(QKeySequence("Ctrl+Z"));
        QObject::connect(undoAction, &QAction::triggered, glWidget, &RouteEditorGLWidget::editUndo);
        editMenu->addAction(undoAction);
    }
    copyAction = new QAction(tr("&Copy"), this); 
    copyAction->setShortcut(QKeySequence("Ctrl+C"));
    QObject::connect(copyAction, &QAction::triggered, glWidget, &RouteEditorGLWidget::editCopy);
    editMenu->addAction(copyAction);
    pasteAction = new QAction(tr("&Paste"), this); 
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));
    QObject::connect(pasteAction, &QAction::triggered, glWidget, &RouteEditorGLWidget::editPaste);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    selectAction = new QAction(tr("&Select Tool"), this); 
    selectAction->setShortcut(QKeySequence("E"));
    QObject::connect(selectAction, &QAction::triggered, glWidget, &RouteEditorGLWidget::editSelect);
    editMenu->addAction(selectAction);
    // View
    viewMenu = menuBar()->addMenu(tr("&View"));
    //toolsAction = GuiFunct::newMenuCheckAction(tr("&Tools"), this); 
    //viewMenu->addAction(toolsAction);
    //QObject::connect(toolsAction, &QAction::triggered, this, &RouteEditorWindow::hideShowToolWidget);

    QAction* viewUnselectAll = new QAction(tr("&Unselect All"), this); 
    viewMenu->addAction(viewUnselectAll);
    QObject::connect(viewUnselectAll, &QAction::triggered, this, &RouteEditorWindow::viewUnselectAll);
    viewMenu->addSeparator();
    vViewWorldGrid = GuiFunct::newMenuCheckAction(tr("&World Grid"), this); 
    viewMenu->addAction(vViewWorldGrid);
    QObject::connect(vViewWorldGrid, &QAction::triggered, this, &RouteEditorWindow::viewWorldGrid);
    vViewTileGrid = GuiFunct::newMenuCheckAction(tr("Tile &Grid"), this); 
    viewMenu->addAction(vViewTileGrid);
    QObject::connect(vViewTileGrid, &QAction::triggered, this, &RouteEditorWindow::viewTileGrid);  
    vViewTerrainGrid = GuiFunct::newMenuCheckAction(tr("Te&rrain Grid"), this, false); 
    viewMenu->addAction(vViewTerrainGrid);
    QObject::connect(vViewTerrainGrid, &QAction::triggered, this, &RouteEditorWindow::viewTerrainGrid);
    vViewTerrainShape = GuiFunct::newMenuCheckAction(tr("&Hide Terrain Shape"), this, false); 
    viewMenu->addAction(vViewTerrainShape);
    QObject::connect(vViewTerrainShape, &QAction::triggered, this, &RouteEditorWindow::viewTerrainShape);
    vShowWorldObjPivotPoints = GuiFunct::newMenuCheckAction(tr("World&Obj Markers"), this, false); 
    viewMenu->addAction(vShowWorldObjPivotPoints);
    QObject::connect(vShowWorldObjPivotPoints, &QAction::triggered, this, &RouteEditorWindow::showWorldObjPivotPointsEnabled);
    vViewInteractives = GuiFunct::newMenuCheckAction(tr("&Interactives"), this); 
    viewMenu->addAction(vViewInteractives);
    QObject::connect(vViewInteractives, &QAction::triggered, this, &RouteEditorWindow::viewInteractives);
    vViewTrackDbLines = GuiFunct::newMenuCheckAction(tr("Track&DB Lines"), this); 
    viewMenu->addAction(vViewTrackDbLines);  
    QObject::connect(vViewTrackDbLines, &QAction::triggered, this, &RouteEditorWindow::viewTrackDbLines);
    vViewTsectionLines = GuiFunct::newMenuCheckAction(tr("T&section Lines"), this); 
    viewMenu->addAction(vViewTsectionLines);
    QObject::connect(vViewTsectionLines, &QAction::triggered, this, &RouteEditorWindow::viewTsectionLines);
    vViewTrackItems = GuiFunct::newMenuCheckAction(tr("&TrackDB Items"), this, Game::renderTrItems); 
    viewMenu->addAction(vViewTrackItems);
    QObject::connect(vViewTrackItems, &QAction::triggered, this, &RouteEditorWindow::viewTrackItems);
    
    vViewPointer3d = GuiFunct::newMenuCheckAction(tr("&3D Pointer"), this); 
    viewMenu->addAction(vViewPointer3d);
    QObject::connect(vViewPointer3d, &QAction::triggered, this, &RouteEditorWindow::viewPointer3d);
    vViewMarkers = GuiFunct::newMenuCheckAction(tr("&Markers"), this, Game::viewMarkers); 
    viewMenu->addAction(vViewMarkers);
    QObject::connect(vViewMarkers, &QAction::triggered, this, &RouteEditorWindow::viewMarkers);
    vViewSnapable = GuiFunct::newMenuCheckAction(tr("S&napable Points"), this, Game::viewSnapable); 
    viewMenu->addAction(vViewSnapable);
    QObject::connect(vViewSnapable, &QAction::triggered, this, &RouteEditorWindow::viewSnapable);
    QAction* vViewCompass = GuiFunct::newMenuCheckAction(tr("&Compass"), this, Game::viewCompass); 
    viewMenu->addAction(vViewCompass);
    QObject::connect(vViewCompass, &QAction::triggered, this, &RouteEditorWindow::viewCompass);

    // Tools
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    propertiesAction = GuiFunct::newMenuCheckAction(tr("&Properties"), this); 
    propertiesAction->setShortcut(QKeySequence("F5"));            
    toolsMenu->addAction(propertiesAction);
    QObject::connect(propertiesAction, &QAction::triggered, this, &RouteEditorWindow::hideShowPropertiesWidget);

    naviAction = GuiFunct::newMenuCheckAction(tr("&Navi Window"), this); 
    naviAction->setShortcut(QKeySequence("F6"));    
    toolsMenu->addAction(naviAction);
    QObject::connect(naviAction, &QAction::triggered, this, &RouteEditorWindow::hideShowNaviWidget);

    statAction = GuiFunct::newMenuCheckAction(tr("Status &Window"), this, false); 
    statAction->setShortcut(QKeySequence("F7"));
    toolsMenu->addAction(statAction);
    QObject::connect(statAction, &QAction::triggered, this, &RouteEditorWindow::hideShowStatWidget);
    
    shapeViewAction = GuiFunct::newMenuCheckAction(tr("&Shape View Window"), this, false); 
    toolsMenu->addAction(shapeViewAction);
    QObject::connect(shapeViewAction, &QAction::triggered, this, &RouteEditorWindow::hideShowShapeViewWidget);
    errorViewAction = GuiFunct::newMenuCheckAction(tr("&Errors and Messages"), this, false); 
    errorViewAction->setShortcut(QKeySequence("F8"));    
    toolsMenu->addAction(errorViewAction);
    QObject::connect(errorViewAction, &QAction::triggered, this, &RouteEditorWindow::hideShowErrorMsgWidget);
    toolsMenu->addSeparator();
    objectsAndTerrainAction = GuiFunct::newMenuCheckAction(tr("O&bjects and Terrain"), this); 
    toolsMenu->addAction(objectsAndTerrainAction);
    QObject::connect(objectsAndTerrainAction, &QAction::triggered, this, &RouteEditorWindow::showToolsObjectAndTerrain);
    objectsAction = GuiFunct::newMenuCheckAction(tr("&Objects"), this); 
    objectsAction->setShortcut(QKeySequence("F1"));
    toolsMenu->addAction(objectsAction);
    QObject::connect(objectsAction, &QAction::triggered, this, &RouteEditorWindow::showToolsObject);
    terrainAction = GuiFunct::newMenuCheckAction(tr("&Terrain"), this); 
    terrainAction->setChecked(false);    
    terrainAction->setShortcut(QKeySequence("F2"));
    toolsMenu->addAction(terrainAction);
    QObject::connect(terrainAction, &QAction::triggered, this, &RouteEditorWindow::showToolsTerrain);
    geoAction = GuiFunct::newMenuCheckAction(tr("&Geo"), this); 
    geoAction->setChecked(false);    
    geoAction->setShortcut(QKeySequence("F3"));
    toolsMenu->addAction(geoAction);
    QObject::connect(geoAction, &QAction::triggered, this, &RouteEditorWindow::showToolsGeo);
    activityAction = GuiFunct::newMenuCheckAction(tr("&Activity"), this); 
    activityAction->setChecked(false);    
    activityAction->setShortcut(QKeySequence("F4"));
    toolsMenu->addAction(activityAction);
    QObject::connect(activityAction, &QAction::triggered, this, &RouteEditorWindow::showToolsActivity);
    // Settings
    terrainCameraAction = GuiFunct::newMenuCheckAction(tr("&Stick Camera To Terrain"), this); 
    terrainCameraAction->setChecked(Game::cameraStickToTerrain);
    terrainCameraAction->setShortcut(QKeySequence("/"));
    QObject::connect(terrainCameraAction, &QAction::triggered, this, &RouteEditorWindow::terrainCamera);
    mstsShadowsAction = GuiFunct::newMenuCheckAction(tr("&MSTS Shadows"), this); 
    mstsShadowsAction->setChecked(Game::mstsShadows);
    QObject::connect(mstsShadowsAction, &QAction::triggered, this, &RouteEditorWindow::mstsShadows);
    QMenu *terrainMenu = new QMenu("&Terrain Editing:");
    QAction *detailTerrainAction = new QAction(tr("&Detailed Terrain"), this);
    QObject::connect(detailTerrainAction, &QAction::triggered, this, &RouteEditorWindow::detailedTerrainEnabled);
    QAction *distantTerrainAction = new QAction(tr("&Distant Terrain"), this);
    QObject::connect(distantTerrainAction, &QAction::triggered, this, &RouteEditorWindow::distantTerrainEnabled);
    terrainMenu->addAction(detailTerrainAction);
    terrainMenu->addAction(distantTerrainAction);
    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(terrainCameraAction);
    settingsMenu->addAction(mstsShadowsAction);
    settingsMenu->addMenu(terrainMenu);
    // Help
    aboutAction = new QAction(tr("&About"), this);
    QObject::connect(aboutAction, &QAction::triggered, this, &RouteEditorWindow::about);
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    
    hideAllTools();
    objTools->show();
    ///// EFO  End MENUBAR 
 
    if(Game::toolsHidden){
        box->hide();
        box2->hide();
        menuBar()->hide();
    } else {
        //box->show();
        //box2->show();
    }
    
    if(Game::playerMode){
        naviWindow->hide();
        statusWindow->hide();
        errorMessagesWindow->hide();
        box->hide();
        box2->hide();
        menuBar()->hide();
        this->viewUnselectAll();
    }
    
    if(Game::serverClient != NULL){
        clientUsersWindow->show();
    }
    
    QObject::connect(this, &RouteEditorWindow::sendMsg,
        glWidget, QOverload<QString>::of(&RouteEditorGLWidget::msg));

    QObject::connect(objTools,
        QOverload<QString>::of(&ObjTools::sendMsg),
        glWidget,
        [this](QString s){ glWidget->msg(s); });
    QObject::connect(objTools,
        QOverload<QString, bool>::of(&ObjTools::sendMsg),
        glWidget,
        [this](QString s, bool b){ glWidget->msg(s, b); });
    QObject::connect(objTools,
        QOverload<QString, int>::of(&ObjTools::sendMsg),
        glWidget,
        [this](QString s, int i){ glWidget->msg(s, i); });
    QObject::connect(objTools,
        QOverload<QString, float>::of(&ObjTools::sendMsg),
        glWidget,
        [this](QString s, float f){ glWidget->msg(s, f); });
    QObject::connect(objTools,
        QOverload<QString, QString>::of(&ObjTools::sendMsg),
        glWidget,
        [this](QString s1, QString s2){ glWidget->msg(s1, s2); });

    QObject::connect(glWidget,
        QOverload<QString>::of(&RouteEditorGLWidget::sendMsg),
        objTools,
        [this](QString s){ objTools->msg(s); });
    QObject::connect(glWidget,
        QOverload<QString, bool>::of(&RouteEditorGLWidget::sendMsg),
        objTools,
        [this](QString s, bool b){ objTools->msg(s, b); });
    QObject::connect(glWidget,
        QOverload<QString, int>::of(&RouteEditorGLWidget::sendMsg),
        objTools,
        [this](QString s, int i){ objTools->msg(s, i); });
    QObject::connect(glWidget,
        QOverload<QString, float>::of(&RouteEditorGLWidget::sendMsg),
        objTools,
        [this](QString s, float f){ objTools->msg(s, f); });
    QObject::connect(glWidget,
        QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
        objTools,
        [this](QString s1, QString s2){ objTools->msg(s1, s2); });

    QObject::connect(glWidget,
        QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s1, QString s2){ terrainTools->msg(s1, s2); });
    QObject::connect(glWidget,
        QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s1, QString s2){ geoTools->msg(s1, s2); });
    QObject::connect(glWidget,
        QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s1, QString s2){ activityTools->msg(s1, s2); });
    QObject::connect(glWidget,
        QOverload<QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s){ activityTools->msg(s); });
    QObject::connect(glWidget,
        QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s1, QString s2){ activityEventWindow->eventProperties->msg(s1, s2); });

    QObject::connect(naviWindow,
        QOverload<QString>::of(&NaviWindow::sendMsg),
        this,
        [this](QString s){ glWidget->msg(s); });
    QObject::connect(naviWindow,
        QOverload<QString, bool>::of(&NaviWindow::sendMsg),
        this,
        [this](QString s, bool b){ glWidget->msg(s, b); });
    QObject::connect(naviWindow,
        QOverload<QString, int>::of(&NaviWindow::sendMsg),
        this,
        [this](QString s, int i){ glWidget->msg(s, i); });
    QObject::connect(naviWindow,
        QOverload<QString, float>::of(&NaviWindow::sendMsg),
        this,
        [this](QString s, float f){ glWidget->msg(s, f); });
    QObject::connect(naviWindow,
        QOverload<QString, QString>::of(&NaviWindow::sendMsg),
        this,
        [this](QString s1, QString s2){ glWidget->msg(s1, s2); });

    QObject::connect(glWidget,
        QOverload<QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s){ shapeViewWindow->msg(s); });
    QObject::connect(glWidget,
        QOverload<QString, bool>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s, bool b){ shapeViewWindow->msg(s, b); });
    QObject::connect(glWidget,
        QOverload<QString, int>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s, int i){ shapeViewWindow->msg(s, i); });
    QObject::connect(glWidget,
        QOverload<QString, float>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s, float f){ shapeViewWindow->msg(s, f); });
    QObject::connect(glWidget,
        QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
        this,
        [this](QString s1, QString s2){ shapeViewWindow->msg(s1, s2); });

    QObject::connect(glWidget,
        QOverload<int, int>::of(&RouteEditorGLWidget::naviInfo),
        this,
        [this](int a, int b){ naviWindow->naviInfo(a, b); });

    QObject::connect(glWidget,
        QOverload<PreciseTileCoordinate*>::of(&RouteEditorGLWidget::posInfo),
        this,
        [this](PreciseTileCoordinate* coord){ naviWindow->posInfo(coord); });

    QObject::connect(glWidget,
        QOverload<float*>::of(&RouteEditorGLWidget::pointerInfo),
        this,
        [this](float* ptr){ naviWindow->pointerInfo(ptr); });

    QObject::connect(glWidget,
        QOverload<QMap<QString, Coords*>>::of(&RouteEditorGLWidget::mkrList),
        this,
        [this](QMap<QString, Coords*> map){ naviWindow->mkrList(map); });

    QObject::connect(glWidget,
        QOverload<QMap<QString, Coords*>>::of(&RouteEditorGLWidget::mkrList),
        this,
        [this](QMap<QString, Coords*> map){ geoTools->mkrList(map); });

    QObject::connect(geoTools,
        QOverload<QMap<int, QPair<int,int>*>>::of(&GeoTools::createNewTiles),
        this,
        [this](QMap<int, QPair<int,int>*> map){ glWidget->createNewTiles(map); });

    QObject::connect(geoTools,
        QOverload<QMap<int, QPair<int,int>*>>::of(&GeoTools::createNewLoTiles),
        this,
        [this](QMap<int, QPair<int,int>*> map){ glWidget->createNewLoTiles(map); });

    QObject::connect(glWidget,
        QOverload<Route*>::of(&RouteEditorGLWidget::routeLoaded),
        this,
        [this](Route* r){ objTools->routeLoaded(r); });

    QObject::connect(glWidget,
        QOverload<Route*>::of(&RouteEditorGLWidget::routeLoaded),
        this,
        [this](Route* r){ activityTools->routeLoaded(r); });

    QObject::connect(objTools,
        QOverload<QString>::of(&ObjTools::enableTool),
        this,
        [this](QString tool){ glWidget->enableTool(tool); });

    QObject::connect(terrainTools,
        QOverload<QString>::of(&TerrainTools::enableTool),
        this,
        [this](QString tool){ glWidget->enableTool(tool); });

    QObject::connect(geoTools,
        QOverload<QString>::of(&GeoTools::enableTool),
        this,
        [this](QString tool){ glWidget->enableTool(tool); });

    QObject::connect(activityTools,
        QOverload<QString>::of(&ActivityTools::enableTool),
        this,
        [this](QString tool){ glWidget->enableTool(tool); });

    QObject::connect(activityEventWindow->eventProperties,
        QOverload<QString>::of(&ActivityEventProperties::enableTool),
        this,
        [this](QString tool){ glWidget->enableTool(tool); });
    
    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        QObject::connect(it,
            &PropertiesAbstract::enableTool,
            glWidget,
            [this](const QString &s){ glWidget->enableTool(s); });
        QObject::connect(glWidget,
            QOverload<QString, QString>::of(&RouteEditorGLWidget::sendMsg),
            it,
            [it](const QString &s1, const QString &s2){ it->msg(s1, s2); });
    }

    if (auto propertiesDyntrack = qobject_cast<PropertiesDyntrack*>(objProperties["Dyntrack"])) {
        QObject::connect(propertiesDyntrack, &PropertiesDyntrack::enableTool,
            glWidget, [this](const QString &s){ glWidget->enableTool(s); });

        QObject::connect(glWidget,
            QOverload<int, int, float*>::of(&RouteEditorGLWidget::flexData),
            propertiesDyntrack, [propertiesDyntrack](int a, int b, float* f){ propertiesDyntrack->flexData(a, b, f); });

        QObject::connect(propertiesDyntrack, &PropertiesDyntrack::setMoveStep,
            glWidget, [this](float f){ glWidget->setMoveStep(f); });
    }

    if (auto activityObject = qobject_cast<PropertiesActivityObject*>(objProperties["ActivityObject"])) {
        QObject::connect(activityObject, &PropertiesActivityObject::sendMsg,
            glWidget, [this](const QString &s){ glWidget->msg(s); });
    }

    if (auto activityConsist = qobject_cast<PropertiesConsist*>(objProperties["ActivityConsist"])) {
        QObject::connect(activityConsist, &PropertiesConsist::cameraObject,
            glWidget, [this](GameObj* obj){ glWidget->setCameraObject(obj); });
    }

    if (auto trackObject = qobject_cast<PropertiesTrackObj*>(objProperties["TrackObj"])) {
        QObject::connect(trackObject, &PropertiesTrackObj::setMoveStep,
            glWidget, [this](float f){ glWidget->setMoveStep(f); });
    }

    QObject::connect(terrainTools, &TerrainTools::setPaintBrush,
        glWidget, [this](Brush* brush){ glWidget->setPaintBrush(brush); });

    QObject::connect(glWidget,
        QOverload<int>::of(&RouteEditorGLWidget::setBrushTextureId),
        terrainTools, [this](int id){ terrainTools->setBrushTextureId(id); });

    QObject::connect(naviWindow, &NaviWindow::jumpTo,
        glWidget, [this](PreciseTileCoordinate* c){ glWidget->jumpTo(c); });

    QObject::connect(glWidget,
        QOverload<Ref::RefItem*>::of(&RouteEditorGLWidget::itemSelected),
        objTools, [this](Ref::RefItem* item){ objTools->itemSelected(item); });

    QObject::connect(glWidget, &RouteEditorGLWidget::showProperties,
        this, &RouteEditorWindow::showProperties);

    QObject::connect(glWidget, &RouteEditorGLWidget::updateProperties,
        this, &RouteEditorWindow::updateProperties);

    QObject::connect(this, &RouteEditorWindow::exitNow,
        aboutWindow, &AboutWindow::exitNow);

    QObject::connect(naviWindow, &NaviWindow::windowClosed,
        this, &RouteEditorWindow::naviWindowClosed);

    QObject::connect(statusWindow, &StatusWindow::windowClosed,
        this, &RouteEditorWindow::statusWindowClosed);

    QObject::connect(errorMessagesWindow, &ErrorMessagesWindow::windowClosed,
        this, &RouteEditorWindow::errorMessagesWindowClosed);

    QObject::connect(shapeViewWindow, &ShapeViewWindow::windowClosed,
        this, &RouteEditorWindow::shapeVeiwWindowClosed);

    QObject::connect(glWidget, &RouteEditorGLWidget::setToolbox,
        this, &RouteEditorWindow::setToolbox);

    QObject::connect(activityTools, &ActivityTools::objectSelected,
        glWidget,
        [this](GameObj* obj){ glWidget->objectSelected(obj); }
    );

    QObject::connect(activityTools, &ActivityTools::showActivityEventEditor,
        this, &RouteEditorWindow::showActivityEventEditor);

    QObject::connect(activityTools, &ActivityTools::showActivityServiceEditor,
        this, &RouteEditorWindow::showActivityServiceEditor);

    QObject::connect(activityTools, &ActivityTools::showActivityTrafficEditor,
        this, &RouteEditorWindow::showActivityTrafficEditor);

    QObject::connect(activityTools, &ActivityTools::showActivityTimetableEditor,
        this, &RouteEditorWindow::showActivityTimetableEditor);

    QObject::connect(activityTools, &ActivityTools::showEvents,
        activityEventWindow, &ActivityEventWindow::showEvents);

    QObject::connect(activityTools, &ActivityTools::showServices,
        activityServiceWindow, &ActivityServiceWindow::showServices);

    QObject::connect(activityTools, &ActivityTools::showTraffic,
        activityTrafficWindow, &ActivityTrafficWindow::showTraffic);

    QObject::connect(activityServiceWindow, &ActivityServiceWindow::reloadServicesList,
        activityTools, &ActivityTools::reloadServicesList);

    QObject::connect(activityTrafficWindow, &ActivityTrafficWindow::reloadTrafficsList,
        activityTools, &ActivityTools::reloadTrafficsList);

    QObject::connect(activityTools, &ActivityTools::showTimetable,
        activityTimetableWindow, &ActivityTimetableWindow::showTimetable);

    QObject::connect(activityEventWindow->eventProperties, &ActivityEventProperties::jumpTo,
        glWidget,
        [this](PreciseTileCoordinate* pos){ glWidget->jumpTo(pos); }
    );

    QObject::connect(activityTools, QOverload<PreciseTileCoordinate*>::of(&ActivityTools::jumpTo),
        glWidget,
        [this](PreciseTileCoordinate* pos){ glWidget->jumpTo(pos); }
    );

    QObject::connect(errorMessagesWindow, &ErrorMessagesWindow::jumpTo,
        glWidget,
        [this](PreciseTileCoordinate* pos){ glWidget->jumpTo(pos); }
    );

    QObject::connect(errorMessagesWindow, &ErrorMessagesWindow::selectObject,
        glWidget,
        [this](GameObj* obj){ glWidget->objectSelected(obj); }
    );

    QObject::connect(activityTools, &ActivityTools::sendMsg,
        glWidget,
        [this](const QString& msg){ glWidget->msg(msg); }
    );

    QObject::connect(this, &RouteEditorWindow::reloadRefFile,
        glWidget, &RouteEditorGLWidget::reloadRefFile);

    /// This connects the menu to the GLWidget where the work happens
    QObject::connect(this, &RouteEditorWindow::reloadMkrFile,
        glWidget, &RouteEditorGLWidget::reloadMkrFiles);

    QObject::connect(glWidget, &RouteEditorGLWidget::refreshObjLists,
        objTools, &ObjTools::refreshObjLists);
    
        if(Game::serverClient != NULL)
        QObject::connect(Game::serverClient, &RouteEditorClient::refreshObjLists,
            objTools, &ObjTools::refreshObjLists);

    
     /// EFO Status Update  was changed from naviWindow
    //QObject::connect(glWidget, &RouteEditorGLWidget::updStatus, naviWindow, &NaviWindow::recStatus);
    //QObject::connect(objTools, &ObjTools::updStatus, naviWindow, &NaviWindow::recStatus);

      /// EFO Status Update  changed to statusWindow 
    QObject::connect(this, &RouteEditorWindow::updStatus, statusWindow, &StatusWindow::recStatus);
    QObject::connect(glWidget, &RouteEditorGLWidget::updStatus, statusWindow, &StatusWindow::recStatus);
    QObject::connect(objTools, &ObjTools::updStatus, statusWindow, &StatusWindow::recStatus);
 
      // connects the GLWidget to the Navi window for marker update      
    // QObject::connect(glWidget, &RouteEditorGLWidget::MkrFiles, naviWindow, &NaviWindow::mkrList);
    QObject::connect(glWidget, &RouteEditorGLWidget::updStatus, naviWindow, &NaviWindow::recStatus);
      /// EFO connect the status buttons to the other windows
    
    QObject::connect(glWidget, &RouteEditorGLWidget::preloadTexturesSignal, terrainTools, &TerrainTools::preloadTextures);

      
}

void RouteEditorWindow::keyPressEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void RouteEditorWindow::exitToLoadWindow(){
        LoadWindow *loadWindow = new LoadWindow();

        QStringList winPos = Game::mainPos.split(","); 
        if(winPos.count() > 1) loadWindow->move( winPos[0].trimmed().toInt(), winPos[1].trimmed().toInt());        
        loadWindow->show();        
}

void RouteEditorWindow::closeEvent(QCloseEvent * event ){
    QVector<QString> unsavedItems;
    glWidget->getUnsavedInfo(unsavedItems);
    
    /// EFO List missing shapes
    QFile file("./" + Game::route + "_missingShapes.txt");    
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QStringList sortedFileList = Route::missingList;
        sortedFileList.sort();
        for (const QString& fileName : sortedFileList) {
            out << fileName << " \n";
        }
        file.close();        
    }      
/*
    QFile file2("./" + Game::route + "_texturesUsed.txt");    
    if (file2.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file2);
        QStringList sortedFileList = Route::texturesList;
        sortedFileList.sort();
        for (const QString& fileName : sortedFileList) {
            out << fileName << " \n";
        }
        file.close();        
    }
 * */          
        
    if(unsavedItems.size() == 0){
        if(Game::debugOutput) qDebug() << "Nothing to Save";
        emit exitNow();
        event->accept();
        SoundManager::CloseAl();              
        //exitToLoadWindow();
        //qApp->quit();
        return;
    }
   
    UnsavedDialog unsavedDialog;   /// EFO need to add the stwqc here when terrain and world are split
    unsavedDialog.setWindowTitle("Save changes?");
    unsavedDialog.setMsg("Save changes in route?");
    for(int i = 0; i < unsavedItems.size(); i++){
        unsavedDialog.items.addItem(unsavedItems[i]);
    }
    unsavedDialog.exec();
    if(unsavedDialog.changed == 0){
        event->ignore();
        return;
    }
    if(unsavedDialog.changed == 2){
        emit exitNow();
        event->accept();
        SoundManager::CloseAl();        
        //exitToLoadWindow();                
        //qApp->quit();
        return;
    }

    //// EFO  need to flesh this out for saving terrain and world separately
    save();


    
    emit exitNow();
    event->accept();
    
    SoundManager::CloseAl();
    //exitToLoadWindow();    
    //qApp->quit();
    
}

void RouteEditorWindow::save(){
    emit sendMsg(QString("save"));
    emit updStatus(QString("stat0"),QString("Saved"));    
}

void RouteEditorWindow::reloadRef(){
    emit reloadRefFile();
}

void RouteEditorWindow::reloadMkr(){
    /// emit signal to GLW which fires off Route->something
    emit reloadMkrFile();    
    if(Game::debugOutput) qDebug() << "Menu triggered ->REW->emit reloadMkrFile";    

    
}

void RouteEditorWindow::reloadSettings(){
    /// emit signal to GLW which fires off Route->something
    Game::reload = true;
    Game::load();    
    
}



void RouteEditorWindow::refreshErrors(){
    emit refreshErrorList(); 
}

void RouteEditorWindow::createPaths(){
    QMessageBox msgBox;
    msgBox.setText("This action will delete all your existing activity paths and create new simple paths! Continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    switch (msgBox.exec()) {
      case QMessageBox::Yes:
          emit sendMsg(QString("createPaths"));
          break;
      case QMessageBox::No:
          break;
      default:
          break;
    }
}

void RouteEditorWindow::terrainCamera(bool val){
    Game::cameraStickToTerrain = val;
}

void RouteEditorWindow::detailedTerrainEnabled(){
    emit this->sendMsg("editDetailedTerrain");
}

void RouteEditorWindow::distantTerrainEnabled(){
    emit this->sendMsg("editDistantTerrain");
}
    
void RouteEditorWindow::mstsShadows(bool val){
    Game::mstsShadows = val;
}

void RouteEditorWindow::about(){
    aboutWindow->show();
}

void RouteEditorWindow::showTerrainTreeEditr(){
    emit sendMsg(QString("showTerrainTreeEditr"));
}

void RouteEditorWindow::showToolsObject(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("objTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsObjectAndTerrain(bool show){
    if(show){
        hideShowToolWidget(true);
        hideAllTools();
        objTools->show();
        objectsAndTerrainAction->setChecked(true);
        terrainTools->show();
        box->setFixedWidth(500);
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsTerrain(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("terrainTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsGeo(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("geoTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showToolsActivity(bool show){
    if(show){
        hideShowToolWidget(true);
        setToolbox("activityTools");
    } else {
        hideShowToolWidget(false);
    }
}

void RouteEditorWindow::showActivityEventEditor(){
    activityEventWindow->show();
}

void RouteEditorWindow::showActivityServiceEditor(){
    activityServiceWindow->show();
}

void RouteEditorWindow::showActivityTrafficEditor(){
    activityTrafficWindow->show();
}

void RouteEditorWindow::showActivityTimetableEditor(){
    activityTimetableWindow->show();
}

void RouteEditorWindow::setToolbox(QString name){
    if(name == "objTools"){
        hideAllTools();
        objTools->show();
        objectsAction->setChecked(true);
    }
    if(name == "terrainTools"){
        hideAllTools();
        terrainTools->show();
        terrainAction->setChecked(true);       
    }
    if(name == "geoTools"){
        hideAllTools();
        geoTools->show();
        geoAction->setChecked(true);
    }
    if(name == "activityTools"){
        hideAllTools();
        if(Game::serverClient == NULL){
            activityTools->show();
            activityAction->setChecked(true);
        }
    }
}

void RouteEditorWindow::hideAllTools(){
    objTools->hide();
    terrainTools->hide();
    geoTools->hide();
    activityTools->hide();
    objectsAction->setChecked(false);
    terrainAction->setChecked(false);     
    geoAction->setChecked(false);
    activityAction->setChecked(false);
    objectsAndTerrainAction->setChecked(false);
    box->setFixedWidth(250);
}

void RouteEditorWindow::showProperties(GameObj* obj){
    // hide all
    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        it->hide();
    }
    if(obj == NULL) return;
    // show 
    //qDebug() << obj->typeObj;

    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        if(!it->support(obj)) continue;
        it->show();
        it->showObj(obj);
        return;
    }
}

void RouteEditorWindow::updateProperties(GameObj* obj){
    if(obj == NULL) return;
    // show 

    //for (std::vector<PropertiesAbstract*>::iterator it = objProperties.begin(); it != objProperties.end(); ++it) {
    foreach (PropertiesAbstract *it, objProperties){
        if(it == NULL) continue;
        if(it->isVisible() && it->support(obj)){
            it->updateObj(obj);
            return;
        }
    }
}

void RouteEditorWindow::hideShowPropertiesWidget(bool show){
    if(show) 
        { box2->show(); propertiesAction->setChecked(true); }
    else 
        { box2->hide(); propertiesAction->setChecked(false); }
}



void RouteEditorWindow::hideShowShapeViewWidget(bool show){
    if(show) shapeViewWindow->show();
    else shapeViewWindow->hide();
}

void RouteEditorWindow::hideShowErrorMsgWidget(bool show){
    if(show) {
        errorMessagesWindow->show();
    }
    else errorMessagesWindow->hide();
}

void RouteEditorWindow::hideShowNaviWidget(bool show){
    if(show) { naviWindow->show();  }
    else { naviWindow->hide();  }
}

void RouteEditorWindow::hideShowStatWidget(bool show){
    if(show) { statusWindow->show();  }
    else { statusWindow->hide();  }
}
void RouteEditorWindow::hideShowToolWidget(bool show){
    if(show) { box->show();     }
    else     { box->hide();    }
}

void RouteEditorWindow::viewWorldGrid(bool show){
    Game::viewWorldGrid = show;
}
void RouteEditorWindow::viewTileGrid(bool show){
    Game::viewTileGrid = show;
}
void RouteEditorWindow::viewTerrainShape(bool show){
    Game::viewTerrainShape = !show;
}
void RouteEditorWindow::viewTerrainGrid(bool show){
    Game::viewTerrainGrid = show;
}
void RouteEditorWindow::showWorldObjPivotPointsEnabled(bool show){
    Game::showWorldObjPivotPoints = show;
}
void RouteEditorWindow::viewInteractives(bool show){
    Game::viewInteractives = show;
}
void RouteEditorWindow::viewTrackDbLines(bool show){
    Game::viewTrackDbLines = show;
}
void RouteEditorWindow::viewTsectionLines(bool show){
    Game::viewTsectionLines = show;
}

void RouteEditorWindow::viewTrackItems(bool show){
    Game::renderTrItems = show;
}

void RouteEditorWindow::viewPointer3d(bool show){
    Game::viewPointer3d = show;
}

void RouteEditorWindow::viewMarkers(bool show){
    Game::viewMarkers = show;
}

void RouteEditorWindow::viewSnapable(bool show){
    Game::viewSnapable = show;
}

void RouteEditorWindow::viewCompass(bool show){
    Game::viewCompass = show;
}

void RouteEditorWindow::showRoute(){
    if(Game::serverClient == NULL){
        if(!glWidget->initRoute()){
            emit exitNow();
            SoundManager::CloseAl();
            qApp->quit();
            return;
        }
        show();
    } else {
        QObject::connect(glWidget, &RouteEditorGLWidget::showWindow, this, &RouteEditorWindow::show);
        glWidget->initRoute();
    }
}

// EFO Move windows
void RouteEditorWindow::show(){
//    naviWindow->move(0,800);
//    statusWindow->move(0,500);
    
    if(!Game::playerMode){
        naviWindow->show();
        QStringList winPos = Game::naviPos.split(","); 
        
        if(winPos.count() < 2)                
        {            
            const int naviTemp1 = this->x() - 300;  // left of window 
            const int naviTemp2 = this->y() + 500;  // 500 from the top corner
            naviWindow->move(std::max(0,naviTemp1) , std::min(naviTemp2,QApplication::primaryScreen()->geometry().bottom()-200));
        }    
        
        /// EFO Status Window is only enabled if you trigger the "S" in MainWindowLayout
        if(Game::mainWindowLayout.contains("s"))
         {
             statusWindow->show();
             statAction->setChecked(true);
         }
         winPos = Game::statusPos.split(","); 
        
          if(winPos.count() < 2)                
          {            
            const int naviTemp1 = this->x() - 300;  // left of window 
            const int naviTemp2 = this->y() + 200;  // 200 from the top corner
            statusWindow->move(std::max(0,naviTemp1) , std::min(naviTemp2,QApplication::primaryScreen()->geometry().bottom()-200));
          }            
    }
    
    if(Game::lockCamera == true) emit updStatus(QString("camera"),QString("Camera Locked")); else emit updStatus(QString("camera"),QString("Camera Unlocked"));
    
    QMainWindow::show();
}

void RouteEditorWindow::naviWindowClosed(){
    naviAction->blockSignals(true);
    naviAction->setChecked(false);
    naviAction->blockSignals(false);
}

void RouteEditorWindow::statusWindowClosed(){
    statAction->blockSignals(true);
    statAction->setChecked(false);
    statAction->blockSignals(false);
}


void RouteEditorWindow::errorMessagesWindowClosed(){
    errorViewAction->blockSignals(true);
    errorViewAction->setChecked(false);
    errorViewAction->blockSignals(false);
}

void RouteEditorWindow::shapeVeiwWindowClosed(){
    shapeViewAction->blockSignals(true);
    shapeViewAction->setChecked(false);
    shapeViewAction->blockSignals(false);
}

void RouteEditorWindow::viewUnselectAll(){

    vViewWorldGrid->setChecked(false);
    vViewTileGrid->setChecked(false);
    vViewTerrainGrid->setChecked(false);
    vViewTerrainShape->setChecked(false);
    vShowWorldObjPivotPoints->setChecked(false);
    vViewInteractives->setChecked(false);
    vViewTrackDbLines->setChecked(false);
    vViewTsectionLines->setChecked(false);
    vViewTrackItems->setChecked(false);
    vViewPointer3d->setChecked(false);
    vViewMarkers->setChecked(false);
    vViewSnapable->setChecked(false);

    vViewWorldGrid->triggered(false);
    vViewTileGrid->triggered(false);
    vViewTerrainGrid->triggered(false);
    vViewTerrainShape->triggered(false);
    vShowWorldObjPivotPoints->triggered(false);
    vViewInteractives->triggered(false);
    vViewTrackDbLines->triggered(false);
    vViewTsectionLines->triggered(false);
    vViewTrackItems->triggered(false);
    vViewPointer3d->triggered(false);
    vViewMarkers->triggered(false);
    vViewSnapable->triggered(false);
}
//void Window::exitNow(){
//    this->hide();
//}
