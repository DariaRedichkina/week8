#include <iostream>
#include <exception>
#include <memory>
#include <mutex>
#include <stack>
#include <stdexcept>
#include <thread>
#include <boost/lockfree/stack.hpp>
#include <vector>
#include <random>
#include <future>
#include <chrono>
#include "Timer.h"
#include <fstream>

template < typename T >
class Threadsafe_Stack
{
public:

	Threadsafe_Stack() = default;

	Threadsafe_Stack(const Threadsafe_Stack& other)
	{
		std::lock_guard < std::mutex > lock(other.m_mutex);
		m_data = other.m_data;
	}

	Threadsafe_Stack& operator=(const Threadsafe_Stack&) = delete;

public:

	void push(T value)
	{
		std::lock_guard < std::mutex > lock(m_mutex);
		m_data.push(value);
	}

	std::shared_ptr < T > pop()
	{
		std::lock_guard < std::mutex > lock(m_mutex);

		if (m_data.empty())
		{
			throw std::range_error("empty stack");
		}

		const auto result = std::make_shared < T >(m_data.top());
		m_data.pop();

		return result;
	}

	void pop(T& value)
	{
		std::lock_guard < std::mutex > lock(m_mutex);

		if (m_data.empty())
		{
			throw std::range_error("empty stack");
		}

		value = m_data.top();
		m_data.pop();
	}
	bool empty() const
	{
		std::lock_guard < std::mutex > lock(m_mutex);
		return m_data.empty();
	}

private:

	std::stack < T > m_data;

private:

	mutable std::mutex m_mutex;
};
class Threads_Guard
{
public:

	explicit Threads_Guard(std::vector < std::thread >& threads) :
		m_threads(threads)
	{}

	Threads_Guard(Threads_Guard const&) = delete;

	Threads_Guard& operator=(Threads_Guard const&) = delete;

	~Threads_Guard() noexcept
	{
		try
		{
			for (std::size_t i = 0; i < m_threads.size(); ++i)
			{
				if (m_threads[i].joinable())
				{
					m_threads[i].join();
				}
			}
		}
		catch (...)
		{
			// std::abort();
		}
	}

private:

	std::vector < std::thread >& m_threads;
};
void pusha(Threadsafe_Stack<int>* stack, bool* start, int* end,int size) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> dis(0, 10);
	using namespace std::chrono_literals;
	while (*start == false) {
		std::this_thread::sleep_for(2ms);
	}
	for (int i = 0; i < size; i++) {
		stack->push(dis(g));
		int b;
		stack->pop(b);
	}
	*end = *end + 1;
}
void pushb(boost::lockfree::stack<int>* stack, bool* start, int* end,int size) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> dis(0, 10);
	using namespace std::chrono_literals;
	while (*start == false) {
		std::this_thread::sleep_for(2ms);
	}
	for (int i = 0; i < size; i++) {
		stack->push(dis(g));
		int a;
		stack->pop(a);
	}
	*end = *end + 1;

}
void call(bool* flag, Timer*time) {
	if (*flag == false) {
		*flag = true; 
		std::cout << time->print() << std::endl;
	}
}


int main()
{
	std::fstream outa("test1.txt", std::ios::out);
	std::fstream outb("test2.txt", std::ios::out);
	for (int size = 10; size < 10000; size =size+100) {
		std::vector<std::thread> thra(10);
		std::vector<std::thread> thrb(10);
		std::vector<std::future<void>> future(20);
		Threadsafe_Stack<int> a;
		boost::lockfree::stack<int> b(1000);
		bool start = false, flaga = false, flagb = false;
		int counta = 0, countb = 0;
		for (int i = 0; i < 10; i++) {
			std::packaged_task<void(Threadsafe_Stack<int>*, bool*, int*, int)> taska(pusha);
			future[i] = taska.get_future();
			thra[i] = std::thread(std::move(taska), &a, &start, &counta, size);
			std::packaged_task<void(boost::lockfree::stack<int>*, bool*, int*, int)> taskb(pushb);
			future[i + 10] = taskb.get_future();
			thrb[i] = std::thread(std::move(taskb), &b, &start, &countb, size);
		}
		Timer timer;
		start = true;
		while (counta != 11 || countb != 11) {
			if (counta == 10) {
				if (flaga == false) {
					flaga = true;
					outa << timer.print() << std::endl;
					counta++;
				}
			}
			if (countb == 10){
				if (flagb == false) {
					flagb = true;
					outb << timer.print() << std::endl;
					countb++;
				}
			}
			std::cout << std::endl;
		}
		
		for (int i = 0; i < 20; i++) {
			future[i].get();
		}
		for (int i = 0; i < 10; i++) {
			thra[i].join();
			thrb[i].join();
		}
		
	}
	outa.close();
	outb.close();

}
