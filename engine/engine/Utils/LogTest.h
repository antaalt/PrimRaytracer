#pragma once

#include <ostream>
#include <sstream>

#include "Timer.h"

#ifdef ERROR
#undef ERROR
#endif

enum LogLevel {
	ERROR,
	WARNING,
	INFO,
	DEBUG,
	DEBUG1,
	DEBUG2,
	DEBUG3,
	DEBUG4
};
//http://www.drdobbs.com/cpp/logging-in-c-part-2/221900468?pgno=2
namespace Utils {
	class Log
	{
	private:
		std::ostringstream& Get(LogLevel level = INFO)
		{
			//os << "- " << NowTime();
			os << " " << levelToString(level) << ": ";
			os << (level > DEBUG ? '\0' : level - DEBUG, '\t');
			messageLevel = level;
			return os;
		}

		void flush()
		{
			if (messageLevel >= Log::ReportingLevel())
			{
				os << std::endl;
				fprintf(stderr, "%s", os.str().c_str());
				fflush(stderr);
				os.clear();
			}
		}
		std::string levelToString(LogLevel level)
		{
			switch (level)
			{
			case ERROR:
				return "ERROR";
			case WARNING:
				return "WARNING";
			case INFO:
				return "INFO";
			case DEBUG:
				return "DEBUG";
			case DEBUG1:
				return "DEBUG1";
			case DEBUG2:
				return "DEBUG2";
			case DEBUG3:
				return "DEBUG3";
			case DEBUG4:
				return "DEBUG4";
			default:
				return "DEFAULT";
			}
		}
	public:
		Log() {}
		
		virtual ~Log()
		{
			flush();
		}

		std::ostringstream & operator[](LogLevel level)
		{
			return Get(level);
		}
		LogLevel& ReportingLevel() { return messageLevel; }

	private:

		LogLevel messageLevel;
		std::ostringstream os;

		Timer time;
	};

}