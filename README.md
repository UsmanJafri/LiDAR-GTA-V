LiDAR GTA V
============================
*A plugin for Grand Theft Auto V that generates a labeled LiDAR point cloud form the game environment.*

<img src="/samples/LiDAR Sample - Traffic.png">

## Requirements

* [Grand Theft Auto V](https://store.steampowered.com/app/271590/Grand_Theft_Auto_V/)
* [ScriptHookV by Alexander Blade](http://www.dev-c.com/gtav/scripthookv/)

## Installation

1. Install ScriptHookV following its accompanying instructions.
2. Copy-paste *LiDAR GTA V.asi* found in [*bin/Release*](https://github.com/UsmanJafri/LiDAR-GTA-V/tree/master/LiDAR%20GTA%20V/bin/Release) into your GTA-V directory (the folder containing *GTAV.exe*)
3. Make a folder named *LiDAR Output* in your game directory.

## How to use

* Press V to cycle through camera modes until first person mode is enabled.
* Press F6 to generate a LiDAR point cloud of the world.
* The point cloud will span 360 degrees horizontally, and -10 to 30 degrees from the aim position vertically with a range of 75 meters.
* Parameters including Horizontal FOV, Vertical FOV, Range, Density and output filename can be configured by editing the input parameters to the *lidar* function on *line 139* of *script.cpp* and rebuilding the solution.
* Output file is located under *#gamedirectory#/LiDAR_Output/LiDAR_GTAV.ply*
* The output is a Polygon File Format (.PLY) file containing only vertex data.
* Vertex Color Code:

| Vertex Color | Label |
|---|---|
| Red | Vehicle |
| Green | Humans and Animals |
| Blue | Game props |
| White | Road, Buildings and other hitable textures |
| Black | No hit |

## Contributing

* You will need Visual Studio 2018 or higher to open the project.
* Any contributions to the project are welcomed, it's recommended to use GitHub [Pull requests](https://help.github.com/articles/using-pull-requests/).

## Acknowledgements

* [Native Trainer](http://www.dev-c.com/gtav/scripthookv/) (a ScriptHookV sample) was used as base project to import configuration.

## Samples

<img src="/samples/LiDAR Sample - Michael Home.png">

<img src="/samples/LiDAR Sample - Test Area.png">

<img src="/samples/LiDAR Sample - Traffic.png">
