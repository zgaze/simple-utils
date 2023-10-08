#include <vector>
#include <functional>
#include "blockingconcurrentqueue.h"

namespace utils {

class ConcurrentExecutor {
	using Task = std::function<void()>;
 public:
	ConcurrentExecutor() {
		int32_t core_num = std::thread::hardware_concurrency();
		workers_.resize(core_num);
	}

	ConcurrentExecutor(int32_t worker_num) {
		workers_.resize(worker_num);
	}

	bool AddTask(Task && task) {
		queue_.enqueue(task);
		return true;
	}

	void Loop() {
		while (true) {
			Task task;
			queue_.wait_dequeue(task);
			task();
		}
	}

	void Start() {
		for (uint32_t i = 0; i < workers_.size(); i++) {
			workers_.at(i) = std::thread(&ConcurrentExecutor::Loop, this);
		}
	}
 private:
	moodycamel::BlockingConcurrentQueue<Task> queue_;
	std::vector<std::thread> workers_;
};

} // namespace utils
