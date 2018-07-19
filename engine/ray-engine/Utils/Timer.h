#pragma once

#include <chrono>

namespace Utils
{
	class Timer
	{
	public:
		typedef std::chrono::seconds seconds;
		typedef std::chrono::milliseconds milliseconds;
		typedef std::chrono::microseconds microseconds;
		typedef std::chrono::nanoseconds nanoseconds;
	public:
		Timer();
		~Timer();

		void tick();

		template<typename Duration = Timer::seconds>
		long long elapsedTime();

		void displayFPS();

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_lastTick;
		unsigned int m_tickCount;

		template<typename Duration = Timer::seconds>
		long long elapsedTimeSince(std::chrono::time_point<std::chrono::steady_clock> & tick);
	};

	template<typename Duration>
	inline long long Timer::elapsedTime()
	{
		return elapsedTimeSince<Duration>(m_lastTick);
	}
	template<typename Duration>
	inline long long Timer::elapsedTimeSince(std::chrono::time_point<std::chrono::steady_clock>& tick)
	{
		return (std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - tick)).count();
	}
}

