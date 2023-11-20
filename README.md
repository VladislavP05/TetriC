# TetriC: A Tetris Clone Written in C

This is a personal project to make a Tetris clone entirely in C using the SDL2 library.

## How To Compile

All source code was written with linux in mind but it should be able to compile on Windows aswell. The only requirements are that you have installed and set up SDL2 on your PC.
Provided with the source code is also a Makefile which you should set to your preferred compiler and change the flags as needed.

### File Structure

- /bin - This is the binary folder. When compiling the source code all .o, .d and executable files will be stored here.
- /bin/images - Placeholder. Here will be stored all game textures.
- /src - Here are stored all the source code files.
- /src/common - Has source code that defines common functions and variables across the program.
- /src/include - Contains all .h files.

### Source Files

- tetric.c - This is the main source file and the entry to the program. It intializes the SDL subsystems, starts the game, controls the fps and unloads all resources when quiting.
- output.c - Creates the game window and renderer, renders the current frame and unloads video resources when quitting the game.
- game.c - Contains the game logic and events and defines structs for holding block info.
- input.c - Takes care of event handling and game controls.
- var.c - Holds all common global varibles and structs.
- util.c - Defines common functions used in the program.
- log.c - Initializes the logging system when starting the program. When initalized provides functions for outputing a formated string with the current time and a custom message to file and/or console.
- text.c - Currently not implemented...
