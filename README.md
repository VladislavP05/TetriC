# TetriC: A Tetris Clone Written in C

This is a personal project to make a Tetris clone entirely in C using the SDL2 library.

## Installation

### Debian:

1. Download and unpack the .zip file
2. Run the `sdlInstall.sh` file
3. You can now run the program from the Tetric file
  
## Controls

- A/D - Move the active piece to the left/right
- Q/R - Rotate the active piece clockwise/contra-clockwise
- S - Increase the falling speed of the active piece 

## How To Compile

All source code was written with Linux in mind but it should compile on Windows as well. The requirements for the program to compile are `SDL2-dev` `SDL2_image-dev` `SDL2_ttf-dev`.
They can be installed from your distro's package manager.

### File Structure

- /bin - This is the binary folder. When compiling the source code all .o, .d and executable files will be stored here.
- /bin/images - Placeholder. Here will be stored all game textures.
- /src - This is the source files location.
- /src/common - Contains the program library source files.
- /src/include - Contains all .h files.

### Source Files

- tetric.c - This is the main source file and the entry to the program. It intializes the SDL subsystems, starts the game, controls the fps and unloads all resources when quiting.
- output.c - Creates the game window and renderer, renders the current frame and unloads video resources when quitting the game.
- game.c - Contains the game logic and events and defines structs for holding block info.
- input.c - Takes care of event handling and game controls.
- var.c - Holds all common global varibles and structs.
- util.c - Defines common functions used in the program.
- log.c - Initializes the logging system when starting the program. When initalized provides functions for outputing a formated string with the current time and a custom message to file and/or console.
- ui.c - Handles ui initialization, texture loading and unloading, and text creation.

## Sources

The font is [VCR OSD Mono](https://www.1001freefonts.com/vcr-osd-mono.font) by mrmanet.
