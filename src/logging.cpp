/// @brief Logging functionalities.
/// @author Enrico Fraccaroli
/// @date 03/08/2019

#include "logging.hpp"

#include <sstream>
#include <cstdarg>
#include <iomanip>
#include <iostream>

// FOREGROUND
#define KRST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define BOLD(x) "\x1B[1m" x KRST
#define UNDL(x) "\x1B[4m" x KRST

static std::string flags_tostring(unsigned int flags)
{
	std::stringstream ss;
	if (flags & ERROR) {
		ss << "ERROR ";
	}
	if (flags & WARNING) {
		ss << "WARNING ";
	}
	if (flags & DEBUG) {
		ss << "DEBUG ";
	}
	if (flags & INFO) {
		ss << "INFO ";
	}
	return ss.str();
}

namespace logging
{
///
static unsigned int display_flags = 0;

void set_flags(unsigned int flags)
{
	display_flags |= flags;
}

void clear_flags(unsigned int flags)
{
	display_flags &= ~flags;
}

bool check_flags(unsigned int flags)
{
	return (display_flags & flags);
}

std::string get_simple_date()
{
	static char buffer[32];
	time_t now = time(nullptr);
	strftime(buffer, 32, "%d/%m/%y", localtime(&now));
	return buffer;
}

std::string get_simple_time()
{
	static char buffer[32];
	time_t now = time(nullptr);
	strftime(buffer, 32, "%H:%M", localtime(&now));
	return buffer;
}

void print(char const *file, char const *func, int line, unsigned int flags,
		   char const *format, ...)
{
	if ((!check_flags(flags)) && !(flags & ERROR)) {
		return;
	}
	static size_t current_length = 0;
	static char *buffer = NULL;
	va_list args1;
	va_start(args1, format);
	va_list args2;
	va_copy(args2, args1);
	size_t length = (1UL + std::vsnprintf(NULL, 0, format, args1));
	if (length > current_length) {
		if (buffer == NULL) {
			buffer =
				static_cast<char *>(std::malloc(sizeof(char) * (length + 1UL)));
		} else {
			buffer = static_cast<char *>(
				std::realloc(buffer, sizeof(int) * (length + 1UL)));
		}
		current_length = length;
	}
	va_end(args1);
	std::vsnprintf(buffer, length, format, args2);
	va_end(args2);

	// == PREPARE LOG =========================================================
	std::stringstream ss;
	// Add the date and timestamp.
	ss << "[" << get_simple_date() << " " << get_simple_time() << "]";
	// Add the flags.
	ss << "[" << std::setw(10) << std::left << flags_tostring(flags) << "]";
	// Add the file:line
	ss << "[" << std::setw(30) << std::left
	   << (std::string(file) + ":" + std::to_string(line)) << "] ";
	if (flags & ERROR) {
		ss << KRED << buffer << KRST "\n";
	} else if (flags & WARNING) {
		ss << KYEL << buffer << KRST "\n";
	} else if (flags & DEBUG) {
		ss << KCYN << buffer << KRST "\n";
	} else if (flags & INFO) {
		ss << buffer << "\n";
	}
	std::cout << ss.str() << std::flush;
	if (flags & ERROR) {
		throw std::runtime_error("Runtime error.");
	}
}

} // namespace logging
