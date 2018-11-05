#include "Timer.h"

#include "Config.h"

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

}