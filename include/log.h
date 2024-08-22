/**
*************************************************************
* @file     log.h
* @author   Stackoverflow : https://stackoverflow.com/a/23446001
* @date     20/07/2024
* @brief
*************************************************************
*/

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void log_error(const char *message, ...);
void log_info(const char *message, ...);
void log_debug(const char *message, ...);

#endif