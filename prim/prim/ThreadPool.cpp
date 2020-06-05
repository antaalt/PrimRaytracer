#include "ThreadPool.h"

namespace prim {
// https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
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
	m_condition.notify_all(); // wake up all threads.
	std::queue<Task>().swap(m_queue); // empty queue
	for (std::thread &thread : m_threads)
		thread.join();
}

void ThreadPool::loop()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [&] {return !m_queue.empty() || !m_running; });
		Task task = m_queue.front();
		m_queue.pop();
		lock.unlock();

		task();
	}
}

}
