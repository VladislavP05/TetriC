#pragma once

/*
(UNFINISHED) Checks if the program is running faster than the set FPS and delaying if necessary. Also updates the delta_time variable.
*/
static void update_frame_time(void);

/*
(UNFINISHED) Initializes resources used in tetric.c and calls the init functions of other systems.
*/
static void initialize_resources(void);

/*
(UNFINISHED) Unloads resources used in tetric.c and calls the unload functions of other systems.
*/
static void unload_resources(void);