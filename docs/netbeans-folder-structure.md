
# Netbeans folder structure

This document is just for reference in case at some point it is decided to make the folder structure like the virtual folders within the Netbeans IDE project, rather than having all source files in a single directory.

CMake can be set up to handle subfolders within a project as described below.

References to all .h files within source/header files need to be adjusted as well.

## How to set up CMake with subfolders

### Top-level CMakeLists.txt

Place e.g. within `/src` along with `App.cpp`, `App.h`, `main.cpp` and whatever other source files there are in the top-level source folder.

Adjust the current top-level CMakeLists.txt. Replace `<a subdir>` with actual subdirectory names.

```cmake
...
add_executable(TSRE5
  App.cpp
  main.cpp
  resources.qrc
)

add_subdirectory(<a subdir>)
add_subdirectory(<a subdir>)
add_subdirectory(<a subdir>)
...
```

### Subfolder CMakeLists.txt with further subfolders

Place e.g. within `/src/somesubdir`.

Replace `<a subdir>` with actual subdirectory names.
And replace `<dir>` with the name of the directory that contains the CMakeLists.txt file.

```cmake
file(GLOB <dir> *.cpp)

add_subdirectory(<a subdir>)
add_subdirectory(<a subdir>)

target_sources(TSRE5 PRIVATE ${<dir>})
```

### Subfolder CMakeLists.txt with no subfolders

Place e.g. within `/src/somesubdir/somesubdirwithoutsubfolders`.

Replace `<dir>` with the name of the directory that contains the CMakeLists.txt file.

```cmake
file(GLOB <dir> *.cpp)

target_sources(TSRE5 PRIVATE ${<dir>})
```


## Folder structure (only .h files shown)

Corresponding .cpp files are in the same folders.

/
├── src/
│   ├── camera/
│   │   ├── Camera.h
│   │   ├── CameraConsist.h
│   │   ├── CameraFree.h
│   │   └── CameraRot.h
│   ├── conEditor/
│   │   ├── CELoadWindow.h
│   │   ├── ConEditorWindow.h
│   │   ├── ConListWidget.h
│   │   ├── ConUnitsWidget.h
│   │   ├── EngListWidget.h
│   │   ├── OverwriteDialog.h
│   │   └── RandomConsist.h
│   ├── coords/
│   │   ├── Coords.h
│   │   ├── CoordsGpx.h
│   │   ├── CoordsKml.h
│   │   ├── CoordsMkr.h
│   │   └── CoordsRoutePlaces.h
│   ├── fileFunctions/
│   │   ├── FileBuffer.h
│   │   ├── FileFunctions.h
│   │   ├── ParserX.h
│   │   ├── ReadFile.h
│   │   ├── TS.h
│   │   └── TarFile.h
│   ├── GEO/
│   │   ├── GeoCoordinates.h
│   │   ├── GeoHgtFile.h
│   │   ├── GeoTerrainFile.h
│   │   ├── GeoTiffFile.h
│   │   ├── HeightWindow.h
│   │   ├── MapData.h
│   │   ├── MapDataOSM.h
│   │   ├── MapDataUrlImage.h
│   │   ├── MapWindow.h
│   │   ├── OSMFeatures.h
│   │   └── UriImageDrawThread.h
│   ├── GUI/
│   │   ├── ActionChooseDialog.h
│   │   ├── ChooseFileDialog.h
│   │   ├── ClickableLabel.h
│   │   ├── GuiFunct.h
│   │   ├── TextEditDialog.h
│   │   └── UnsavedDialog.h
│   ├── HUD/
│   │   ├── GuiGlCompass.h
│   │   └── SimpleHud.h
│   ├── math3d/
│   │   ├── Flex.h
│   │   ├── GLMatrix.h
│   │   ├── Intersections.h
│   │   ├── Vector2f.h
│   │   ├── Vector2i.h
│   │   ├── Vector3f.h
│   │   └── Vector4f.h
│   ├── ogl/
│   │   ├── GLH.h
│   │   ├── GLUU.h
│   │   ├── OglObj.h
│   │   ├── Pointer3d.h
│   │   ├── PoleObj.h
│   │   ├── Shader.h
│   │   ├── TextObj.h
│   │   └── TrackItemObj.h
│   ├── ProceduralShapes/
│   │   ├── ComplexLine.h
│   │   ├── GlobalDefinitions.h
│   │   ├── ObjFile.h
│   │   ├── ProceduralMstsDyntrack.h
│   │   ├── ProceduralShape.h
│   │   └── ShapeTemplates.h
│   ├── renderer/
│   │   ├── OpenGL3Renderer.h
│   │   ├── RenderItem.h
│   │   └── Renderer.h
│   ├── routeEditor/
│   │   ├── activity/
│   │   │   ├── ActivityEventProperties.h
│   │   │   ├── ActivityEventWindow.h
│   │   │   ├── ActivityServiceProperties.h
│   │   │   ├── ActivityServiceWindow.h
│   │   │   ├── ActivityTimetableProperties.h
│   │   │   ├── ActivityTimetableWindow.h
│   │   │   ├── ActivityTrafficProperties.h
│   │   │   └── ActivityTrafficWindow.h
│   │   ├── properties/
│   │   │   ├── EditFileNameDialog.h
│   │   │   ├── PropertiesAbstract.h
│   │   │   ├── PropertiesActivityObject.h
│   │   │   ├── PropertiesActivityPath.h
│   │   │   ├── PropertiesCarspawner.h
│   │   │   ├── PropertiesConsist.h
│   │   │   ├── PropertiesDyntrack.h
│   │   │   ├── PropertiesForest.h
│   │   │   ├── PropertiesGroup.h
│   │   │   ├── PropertiesLevelCr.h
│   │   │   ├── PropertiesPickup.h
│   │   │   ├── PropertiesPlatform.h
│   │   │   ├── PropertiesRuler.h
│   │   │   ├── PropertiesSiding.h
│   │   │   ├── PropertiesSignal.h
│   │   │   ├── PropertiesSoundRegion.h
│   │   │   ├── PropertiesSoundSource.h
│   │   │   ├── PropertiesSpeedpost.h
│   │   │   ├── PropertiesStatic.h
│   │   │   ├── PropertiesTerrain.h
│   │   │   ├── PropertiesTrackItem.h
│   │   │   ├── PropertiesTrackObj.h
│   │   │   ├── PropertiesTransfer.h
│   │   │   ├── PropertiesUndefined.h
│   │   │   ├── RandomTransformWorldObjDialog.h
│   │   │   ├── SignalWindow.h
│   │   │   ├── SignalWindowLink.h
│   │   │   ├── TerrainWaterWindow2.h
│   │   │   └── TransformWorldObjDialog.h
│   │   ├── AboutWindow.h
│   │   ├── ActivityTools.h
│   │   ├── ClientUsersWindow.h
│   │   ├── ErrorMessageProperties.h
│   │   ├── ErrorMessagesWindow.h
│   │   ├── GeoTools.h
│   │   ├── LoadWindow.h
│   │   ├── NaviBox.h
│   │   ├── NaviWindow.h
│   │   ├── NewRouteWindow.h
│   │   ├── ObjTools.h
│   │   ├── PlayActivitySelectWindow.h
│   │   ├── RouteEditorClient.h
│   │   ├── RouteEditorGLWidget.h
│   │   ├── RouteEditorServer.h
│   │   ├── RouteEditorWindow.h
│   │   ├── ShapeViewWindow.h
│   │   ├── TerrainTools.h
│   │   ├── TerrainTreeWindow.h
│   │   ├── TerrainWaterWindow.h
│   │   └── TrkWindow.h
│   ├── shape/
│   │   ├── sfile/
│   │   │   ├── SFile.h
│   │   │   ├── SFileC.h
│   │   │   └── SFileX.h
│   │   └── ShapeLib.h
│   ├── shapeViewer/
│   │   ├── ConInfoWidget.h
│   │   ├── ContentHierarchyInfo.h
│   │   ├── EngInfoWidget.h
│   │   ├── ShapeHierarchyInfo.h
│   │   ├── ShapeHierarchyWindow.h
│   │   ├── ShapeInfoWidget.h
│   │   ├── ShapeTextureInfo.h
│   │   ├── ShapeTexturesWindow.h
│   │   ├── ShapeViewerGLWidget.h
│   │   ├── ShapeViewerNavigatorWidget.h
│   │   └── ShapeViewerWindow.h
│   ├── sound/
│   │   ├── MstsSoundDefinition.h
│   │   ├── SoundLib.h
│   │   ├── SoundManager.h
│   │   ├── SoundSource.h
│   │   └── SoundVariables.h
│   ├── TDB/
│   │   ├── Ruch.h
│   │   ├── SigCfg.h
│   │   ├── SignalShape.h
│   │   ├── SignalType.h
│   │   ├── SpeedPost.h
│   │   ├── SpeedPostDAT.h
│   │   ├── TDB.h
│   │   ├── TDBClient.h
│   │   ├── TRitem.h
│   │   ├── TRnode.h
│   │   ├── TSection.h
│   │   ├── TSectionDAT.h
│   │   └── TrackShape.h
│   ├── texture/
│   │   ├── AceLib.h
│   │   ├── Brush.h
│   │   ├── ImageLib.h
│   │   ├── MapLib.h
│   │   ├── PaintTexLib.h
│   │   ├── TexLib.h
│   │   └── Texture.h
│   ├── trains/
│   │   ├── ActLib.h
│   │   ├── Activity.h
│   │   ├── ActivityEvent.h
│   │   ├── ActivityObject.h
│   │   ├── ActivityTimetable.h
│   │   ├── ConLib.h
│   │   ├── Consist.h
│   │   ├── Eng.h
│   │   ├── EngLib.h
│   │   ├── Path.h
│   │   ├── Service.h
│   │   ├── Traffic.h
│   │   └── TrainNetworkEng.h
│   ├── world/
│   │   ├── objects/
│   │   │   ├── CarSpawnerObj.h
│   │   │   ├── DynTrackObj.h
│   │   │   ├── ForestObj.h
│   │   │   ├── GroupObj.h
│   │   │   ├── HazardObj.h
│   │   │   ├── LevelCrObj.h
│   │   │   ├── PickupObj.h
│   │   │   ├── PlatformObj.h
│   │   │   ├── RulerObj.h
│   │   │   ├── SignalObj.h
│   │   │   ├── SoundRegionObj.h
│   │   │   ├── SoundSourceObj.h
│   │   │   ├── SpeedpostObj.h
│   │   │   ├── StaticObj.h
│   │   │   ├── TrWatermarkObj.h
│   │   │   ├── TrackObj.h
│   │   │   ├── TransferObj.h
│   │   │   └── WorldObj.h
│   │   ├── Environment.h
│   │   ├── OrtsWeatherChange.h
│   │   ├── QuadTree.h
│   │   ├── Ref.h
│   │   ├── Route.h
│   │   ├── RouteClient.h
│   │   ├── Skydome.h
│   │   ├── SoundList.h
│   │   ├── TFile.h
│   │   ├── Terrain.h
│   │   ├── TerrainClient.h
│   │   ├── TerrainInfo.h
│   │   ├── TerrainLib.h
│   │   ├── TerrainLibQt.h
│   │   ├── TerrainLibSimple.h
│   │   ├── Tile.h
│   │   └── Trk.h
│   ├── App.h
│   ├── ClientInfo.h
│   ├── ErrorMessage.h
│   ├── ErrorMessagesLib.h
│   ├── main.cpp
│   ├── Game.h
│   ├── GameObj.h
│   └── Undo.h
├── LICENSE.md
└── README.md
