#include "ThreadPool.h"

namespace prim {

ThreadPool::ThreadPool() : 
	m_running(false), 
	m_threads(std::thread::hardware_concurrency())
{
}

ThreadPool::~ThreadPool()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_running)
	{
		lock.unlock();
		stop();
	}
}

bool ThreadPool::empty() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_queue.empty();
}

size_t ThreadPool::size() const
{
	return m_threads.size();
}

void ThreadPool::wait()
{
	/*std::unique_lock<std::mutex> lock(m_mutex);
	m_condition.wait(lock, [&] { return m_queue.empty(); });*/
}

void ThreadPool::reset()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::queue<Task>().swap(m_queue);
}

void ThreadPool::addTask(Task task)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_queue.push(task);
	lock.unlock();
	m_condition.notify_one();
}

void ThreadPool::start()
{
	m_running = true;
	for (std::thread &thread : m_threads)
		thread = std::thread(&ThreadPool::loop, this);
}
void ThreadPool::stop()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_running = false;
	std::queue<Task>().swap(m_queue); // empty queue
	lock.unlock();
	m_condition.notify_all(); // wake up all threads.
	for (std::thread &thread : m_threads)
		thread.join();
}

void ThreadPool::loop()
{
	while (m_running)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [&] {return !m_queue.empty() || !m_running; });
		if (!m_queue.empty())
		{
			Task task = m_queue.front();
			m_queue.pop();
			lock.unlock();

			task();
		}
	}
}

}
