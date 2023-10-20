#pragma once

/*
(UNFINISHED) Initializes video output systems and variables.
*/
extern void init_SDL_video(void);

/*
(UNFINISHED) Renders a frame of the playing field, text and HUD.
*/
extern void render_frame(void);

/*
(UNFINISHED) Unloads resources used by output.c and destroys the window and renderer.
*/
extern void unload_SDL_video(void);