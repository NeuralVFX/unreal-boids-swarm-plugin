
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

# Plugin Contents

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

#### BoidsGameModeBP - GameMode BluePrint
- A GameMode which has the `Default Pawn Class` overrriden with `DemoBoidsSwarm_BP`

#### DemoBoidsSwarm_BP - DemoBoidsSwarm BluePrint
- A Blueprint with a fish assigned as the `InstancedStaticMesh` and spheres parented to the Attrack/Repel `SceneComponents`

#### FishExample - Level
- A simple level to run `DemoBoidsSwarm_BP` Pawn

## BoidsDemoSwarm Attributes


### Boids Mesh
```
--Instance Size, default=.03, type=float                       # Scale of instances
```

### Boids Simulation
```
--Instance Count, default=1000, type=int                        # How many instances to include in simulation
--Width, default=400, type=float                                # Width of starting box for instances
--Height, default=400, type=float                               # Height of starting box for instances
--Depth, default=400, type=float                                # Depth of starting box for instances
--Min Dist, default=2.0, type=float                             # Distance at which neighbor repulsion starts
--Max Dist, default=400, type=float                             # Distance at wich neighbor attraction ends
--Vel Mult, default=400, type=float                             # Overall multiplier for velocity
--Min Vel, default=400, type=float                              # Minimum velocity clip value
--Max Vel, default=400, type=float                              # Maximum velocity clip value
--Max Acc, default=400, type=float                              # Clip value for overall velocity
--Goal Strength, default=400, type=float                        # Multiplier for target attraction velocity
--Avoid Dist, default=400, type=float                           # Distance cutoff for goal avoidance
--Avoid Strength, default=400, type=float                       # Multiplier for target repulsion velocity
--Ticket, default=0, type=int                                   # Which index is being used to query the DLL
```