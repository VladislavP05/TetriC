// log.c

// Creates, edits and adds entries to the program log file

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../include/log.h"

#define LOG_STR_BUFFER 512 // Time string buffer

static FILE *log_file = NULL;         // Current time used in log
static char log_time[LOG_STR_BUFFER]; // Formated time string

extern void upadate_log_time(void)
{
    time_t real_time;
    time(&real_time);
    strftime(log_time, LOG_STR_BUFFER, "[%Y:%m:%d]: %X", localtime(&real_time));
    return;
}

extern void write_log(const char *message, int flags)
{
    assert(message && flags);

    char msg_type[4];

    switch (flags & 0b1100)
    {
    case LOG_TYPE_INF:
    {
        strcpy(msg_type, "INF");
        break;
    }

    case LOG_TYPE_WRN:
    {
        strcpy(msg_type, "WRN");
        break;
    }

    case LOG_TYPE_ERR:
    {
        strcpy(msg_type, "ERR");
        break;
    }

    default:
    {
        fprintf(stderr, "Unknown Log Type\n");
        exit(1);
    }
    }

    switch (flags & 0b0011)
    {
    case LOG_OUT_CONSOLE:
    {
        if (fprintf(stdout, "%s - %s: %s\n", log_time, msg_type, message) < 0)
        {
            fprintf(stderr, "Writing to Console failed\n");
            exit(1);
        }
        break;
    }

    case LOG_OUT_FILE:
    {
        if (fprintf(log_file, "%s - %s: %s\n", log_time, msg_type, message) < 0)
        {
            fprintf(stderr, "Writing to File failed\n");
            exit(1);
        }
        break;
    }

    case LOG_OUT_BOTH:
    {
        if (fprintf(stdout, "%s - %s: %s\n", log_time, msg_type, message) < 0)
        {
            fprintf(stderr, "Writing to Console failed\n");
            exit(1);
        }
        if (fprintf(log_file, "%s - %s: %s\n", log_time, msg_type, message) < 0)
        {
            fprintf(stderr, "Writing to File failed\n");
            exit(1);
        }
        break;
    }

    default:
    {
        fprintf(stderr, "Unknown Log Output\n");
        exit(1);
    }
    }

    return;
}

extern void unload_log(void)
{
    assert(log_file);

    write_log("End log", LOG_OUT_FILE | LOG_TYPE_INF);

    fclose(log_file);
    return;
}

extern void init_log(void)
{
    time_t real_time;
    time(&real_time);
    strftime(log_time, LOG_STR_BUFFER, "[%Y:%m:%d]: %X", localtime(&real_time));

    log_file = fopen("game_log.log", "w");
    if (!log_file)
    {
        write_log("Could not create log file.\n", LOG_OUT_CONSOLE | LOG_TYPE_ERR);
        exit(1);
    }

    write_log("Start log", LOG_OUT_FILE | LOG_TYPE_INF);

    return;
}