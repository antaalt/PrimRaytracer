#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>
#include <functional>

namespace prim {

class ThreadPool {
	using Task = std::function<void(void)>;
public:
	ThreadPool();
	~ThreadPool();

	// Is thread pool waiting for tasks
	bool empty() const;
	// Wait for thread pool to be idle
	void wait();
	// Reset the queue
	void reset();
	// Add a task to the pool
	void addTask(Task task);
	// Start the thread pool
	void start();
	// Stop the thread pool
	void stop();
private:
	void loop();
private:
	std::atomic<bool> m_running;
	mutable std::mutex m_mutex;
	std::condition_variable m_condition;
	std::queue<Task> m_queue;
	std::vector<std::thread> m_threads;
};

}