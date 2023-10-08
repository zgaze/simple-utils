#include "thread_pool.h"
#include <unistd.h>
#include <sys/time.h>

static int64_t Fib(int32_t n) {
	if (n <= 2) {
		return n;
	} else {
		return Fib(n - 1) + Fib(n - 2);
	}
}

static uint64_t TimeStamp() {
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}


int main() {
	utils::ThreadPool executor;
	executor.Start();
	uint64_t begin = TimeStamp();
	for (int i = 0; i < 50; ++i) {
		executor.AddTask([i, begin]() {
			int64_t result = Fib(i);
			printf("fib [%d] is %ld cost :%lu\n", i, result,  TimeStamp() - begin);
			} );
	}
	sleep(100);
	return 0;
}
