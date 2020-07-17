<img src="https://i.imgur.com/oED4t4X.png" width=500></img>  
---
**Deadrop** is a small game engine that was made as part of the Afterback project (for game technology development).  
It has a system-based design philosophy where the main engine functionalities are defined as systems, and it has a core library that provides support functionality to the rest of the engine in a platform-independent way.

## History
The engine was written from 2017 to 2019 with the goal of exploring games and graphics development and for internal use, but later on it was decided that Deadrop will continue its life as an open source project.

## Technologies
- Written in modern C++17.
- Uses DirectX 11 Graphics API (as the main back-end by abstracting it).
- Compiles using Visual Studio 2019
- Premake5 build system.

## What's in it for you?
- **Deadrop** was made as a research project, and as such, it is a good starting point for developers that want to get into the development of game engines and games.  
- If you want to start but don't know how all the parts fit together or how graphics apis are used in games, reading Deadrop's source code is a great way to figure that out.  
- The source code is heavily commented and makes use of up-to-date technologies so you can easily apply whatever you learn from it in your future projects.

## How to compile
- Download the latest Premake5 build system from (https://premake.github.io).
- Clone this repository using git, or just download it as a ZIP file.
- Run Premake5 on the project's root directory (where premake5.lua is) like this:  
  > premake5 vs2019  
  
  This will generate a new directory at 'build/vs2019' which will contain the Visual Studio solution.
- Open the ProjectDeadrop.sln in Visual Studio and compile.

## Where is the rest of the code?
Since Deadrop was a research project, a lot of work is being put into refactoring the source code and commenting it so that you can easily understand it and use it, therefore, more of the source code will be released as soon it is refactored.  
Here is a list of what is expected to be released:
- A Windowing System.
- An Input System with Keyboard and Mouse (5-buttons) support.
- A Rendering System (responsible for using the graphics abstraction to render scenes).
- A Scripting System. [WIP]
- Support for glTF format through conversion. [WIP]
- And more functionality will be added as the project goes.

## How to use it
As more source code is released, hopefully there will be a series of demos to introduce the usage of the engine.  
But if you can't wait until then, I suggest you dig into the source code and not be afraid to get your hands dirty.  
Documentation is also expected to become available in the future.  
>This section will be updated as soon as more information is available.

## License
> You can check out the full license in the 'LICENSE' file [here](https://github.com/Afterback/Deadrop/blob/master/LICENSE).  

This project is licensed under the terms of the **Zlib** license.