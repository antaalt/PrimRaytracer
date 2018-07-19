#include "Log.h"

#include <iostream>

namespace Log {

	void info(const char * str)
	{
		std::cout << INFO_HEADER << str << std::endl;
	}
	void debug(const char * str)
	{
		std::cout << DEBUG_HEADER << str << std::endl;
	}

	void error(const char * str)
	{
		std::cerr << ERROR_HEADER << str << std::endl;
	}

	void warn(const char * str)
	{
		std::cerr << WARN_HEADER << str << std::endl;
	}

}