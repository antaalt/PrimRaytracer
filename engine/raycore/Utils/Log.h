#pragma once

#include <iostream>

#define ERROR_HEADER	"ERROR > "
#define INFO_HEADER		"INFO  > "
#define DEBUG_HEADER	"DEBUG > "
#define WARN_HEADER		"WARN  > "

namespace Log {
	template <typename T>
	void info(const char *str, T data);
	template <typename T>
	void debug(const char *str, T data);
	template <typename T>
	void error(const char *str, T data);
	template <typename T>
	void warn(const char *str, T data);

	template<typename... Args>
	void info(Args... args);

	template<typename... Args>
	void debug(Args... args);

	template<typename... Args>
	void error(Args... args);

	template<typename... Args>
	void warn(Args... args);

	namespace {
		template<typename T, typename... Args>
		void print(T t, Args... args);
		template<typename T>
		void print(T t);

		template<typename T, typename... Args>
		void print(T t, Args... args)
		{
			//const int n = sizeof...(Args);
			std::cout << t;
			print(args...);
		}

		template<typename T>
		void print(T t)
		{
			std::cout << t << std::endl;
		}
	}

	template<typename T>
	void info(const char * str, T data)
	{
		std::cout << INFO_HEADER << str << data << std::endl;
	}

	template<typename T>
	void debug(const char * str, T data)
	{
		std::cout << DEBUG_HEADER << str << data << std::endl;
	}

	template<typename T>
	void error(const char * str, T data)
	{
		std::cerr << ERROR_HEADER << str << data << std::endl;
	}

	template<typename T>
	void warn(const char * str, T data)
	{
		std::cerr << WARN_HEADER << str << data << std::endl;
	}

	template<typename ...Args>
	void info(Args ...args)
	{
		std::cout << INFO_HEADER;
		print(args...);
	}

	template<typename ...Args>
	void debug(Args ...args)
	{
		std::cout << DEBUG_HEADER;
		print(args...);
	}

	template<typename ...Args>
	void error(Args ...args)
	{
		std::cout << ERROR_HEADER;
		print(args...);
	}

	template<typename ...Args>
	void warn(Args ...args)
	{
		std::cout << WARN_HEADER;
		print(args...);
	}

	
	
}