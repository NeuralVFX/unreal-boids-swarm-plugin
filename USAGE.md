
# Getting Started

## Requirements:

- Unreal Engine 4.26

## Project Setup

- Make a C++ Unreal Project 
- Make sure you have a `Plugins` folder inside of the project
- Clone this Repo into the plugins folder (Or download the zip file and unzip there)
- Right click your `.uproject` file and click `Generate Visual Studio Project Files`
- Build the project from Visual Studio, using `Build Solution`
![](Images/plugin_a.jpg)

#### Find Plugin
- Open the compiled project in Unreal Engine
- From `Edit->Plugins`, locate the `Other` Category
- Find `BoidsSwarm` and click `Enable`

#### Scene/World Setup

- To setup keymapping, goto `Edit->Project Settings` and click `Import`, and then import the file `Resources/KeyMapping.ini`
- The Boids actor is a Pawn, so in `World Settigs`, make sure that `GameMode Override` is set to `BoidsGameMode_BP`, this has the Pawn set already.
- Open `Settings->Project Settings` and find `GameInstanceClass`, replace this with `cDataStorageGameInstance`
- In the Content Manager, navigate to BoidsSwarm Content `Level->FishExample`
![](Images/attributes_a.jpg)

## Run It

#### Run Test Scene

- Play the level!
- Move the mouse to slide the Boids target around
- Hit space bar to switch between controlling the Attrack/Repel targets

## Classes

#### DemoBoidsSwarm - Pawn Class
- Pawn class which contains an `InstancedStaticMesh`, a `Camera`, and two `SceneComponents` to control the Attrack/Repel targets.
- On every tick, this queries the next step of the BOIDs simulation and updates the instanced `InstancedStaticMesh`
- The positition/vel is queried from `UDataStorageGameInstance`

#### cDataStoageWrapper - Object Class
- Wrapper for `boids.dll`
- Finds and initiates `DLL`
- Exposes functions of `DLL`

#### UDataStorageGameInstance - GameInstance Class
- This is a wrapper for `cDataStoageWrapper`
- Manages starting and stopping `LibTorch` based on the game state
- Retrieves `LibTorch` output, to pass on to `DemoBoidsSwarm`

## Content

#### Fish - StaticMesh
- A fish model which is instanced in `DemoBoidsSwarm_BP`

#### FishMat, FishTexture - Material, Texture
- Material and texture assigned to `Fish`

#### GreenMat, RedMat - Materials
- Materials assigned to spheres in `DemoBoidsSwarm_BP`

#### BoidsGameModeBP - GameMode BluePrint
- A GameMode which has the `Default Pawn Class` overrriden with `DemoBoidsSwarm_BP`

#### DemoBoidsSwarm_BP - DemoBoidsSwarm BluePrint
- A BluePrint with a Fish assigned as the `InstancedStaticMesh` and Spheres parented to the Attrack/Repel `SceneComponents`

#### FishExample - Level
- A simple level to run `DemoBoidsSwarm_BP` Pawn
