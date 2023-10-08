#include <mutex>
#include <vector>
#include <functional>
#include <condition_variable>
#include <deque>
#include <thread>

namespace utils {

class ThreadPool {
	using Task = std::function<void()>;
 public:
	ThreadPool() {
		int32_t core_num = std::thread::hardware_concurrency();
		workers_.resize(core_num);
	}

	bool AddTask(Task && task) {
		{
			std::lock_guard<std::mutex> lock(this->mutex_);
			queue_.push_front(task);
		}
		this->condition_.notify_one();
	}

	void Loop() {
		while (true) {
			Task task;
			{
				std::unique_lock<std::mutex> lock(this->mutex_);
				this->condition_.wait(lock, [=]{ return !this->queue_.empty();});
				task = queue_.back();
				queue_.pop_back();
			}
			task();
		}
	}

	void Start() {
		for (uint32_t i = 0; i < workers_.size(); i++) {
			  workers_.at(i) = std::thread(&ThreadPool::Loop, this);
		}
	}

 private:
	std::vector<std::thread> workers_;
	std::mutex mutex_;
	std::condition_variable condition_;
	std::deque<Task> queue_;
};


} // namespace utils
