#pragma once

#include <util/c_includes.h>

void LoggerInit();
// Universal function for log message
void __LogMessage(int type, const char* file, unsigned line, const char* format, ...);
void LoggerRelease();

// Color codes for colorful output in stdout
#define RESET       "\033[0m"
#define BLACK       "\033[30m"             /* Black */
#define RED         "\033[31m"             /* Red */
#define GREEN       "\033[32m"             /* Green */
#define YELLOW      "\033[33m"             /* Yellow */
#define BLUE        "\033[34m"             /* Blue */
#define MAGENTA     "\033[35m"             /* Magenta */
#define CYAN        "\033[36m"             /* Cyan */
#define WHITE       "\033[37m"             /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

enum LogType
{
	LOG_INFO = 0,
	LOG_ERROR,
	LOG_WARNING
};

#define LOG_MSG(...) __LogMessage(LOG_INFO, __BASE_FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...) __LogMessage(LOG_ERROR, __BASE_FILE__, __LINE__, __VA_ARGS__)
#define LOG_WAR(...) __LogMessage(LOG_WARNING, __BASE_FILE__, __LINE__, __VA_ARGS__)