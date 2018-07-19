#include "Timer.h"

#include "Log.h"

namespace Utils {

Timer::Timer() : m_tickCount(0)
{
}


Timer::~Timer()
{
}

void Timer::tick()
{
	m_lastTick = std::chrono::steady_clock::now();
}

void Timer::displayFPS()
{
	m_tickCount++;
	long long duration = elapsedTime<milliseconds>();
	if (duration > 1000)
	{
		tick();
		Log::debug("FPS : ", m_tickCount);
		m_tickCount = 0;
	}
}

}