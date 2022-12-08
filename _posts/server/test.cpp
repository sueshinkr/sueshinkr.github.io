#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

using namespace std;

mutex m;
vector<int> v;

template<typename T>
class LockGuard
{
	public:
		LockGuard(T& m)
		{
			_mutex = &m;
			_mutex->lock();
		}

		~LockGuard()
		{
			_mutex->unlock();
		}
	private:
		T* _mutex;
};

void Push()
{
	for (int i = 0; i < 10000; i++)
	{
		m.lock();
		v.push_back(i);
		m.unlock();

		LockGuard<std::mutex> lockGuard(m);
		v.push_back(i);

		// 표준
		// std::lock_guard<std::mutex> lockGuard2(m);

		//std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
		//uniqueLock.lock();
	}
}

int main()
{
	v.reserve(20000);
	std::thread t1(Push);
	std::thread t2(Push);
	t1.join();
	t2.join();
	
	cout << v.size() << endl;
}
