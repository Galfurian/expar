/// @brief Logging functionalities.
/// @author Enrico Fraccaroli
/// @date 03/08/2019

#pragma once

#include <string>
#include <cstring>

enum {
	NO_FLAGS = 0UL,
	DEBUG = 1UL << 0UL,
	WARNING = 1UL << 1UL,
	ERROR = 1UL << 2UL,
	INFO = 1UL << 3UL,
	ALL_FLAGS = DEBUG | WARNING | ERROR | INFO
};

namespace logging {

void set_flags(unsigned int flags);

void clear_flags(unsigned int flags);

bool check_flags(unsigned int flags);

std::string get_simple_date();

std::string get_simple_time();

void print(char const* file,
		   char const* fun,
		   int line,
		   unsigned int flags,
		   char const* format, ...);

} // namespace logging

#define __FILENAME__ (std::strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define _log(...) logging::print(__FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define _info(...) _log(INFO, __VA_ARGS__)
#define _error(...) _log(ERROR, __VA_ARGS__)
#define _warning(...) _log(WARNING, __VA_ARGS__)
#define _debug(...) _log(DEBUG, __VA_ARGS__)
