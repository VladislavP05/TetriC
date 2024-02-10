#pragma once

#define LOG_OUT_CONSOLE 0b0001 // Log will output to stdout
#define LOG_OUT_FILE 0b0010    // Log will output to game_log.log
#define LOG_OUT_BOTH 0b0011    // Log will output to stdout and game_log.log
#define LOG_TYPE_INF 0b0100    // Sets the log type to info
#define LOG_TYPE_WRN 0b1000    // Sets the log type to warning
#define LOG_TYPE_ERR 0b1100    // Sets the log type to error

/*
(UNFINISHED) Gets the current system time and formats it for use in logs.
*/
extern void upadate_log_time(void);

/*
(UNFINISHED) Writes a message to the log file and/or console.

Parameters:
    message - Text to be used in the log
    flags - Must have an OUT and TYPE flag OR'ed together (example LOG_OUT_CONSOLE | LOG_TYPE_INF). See log.h for flags.
*/
extern void write_log(const char *message, int flags);

/*
(UNFINISHED) Closes the log file stream and unloads any associated resources.
*/
extern void unload_log(void);

/*
(UNFINISHED) Creates the log file, initializing any resources used by the system. 
*/
extern void init_log(void);