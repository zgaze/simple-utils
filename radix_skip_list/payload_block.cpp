
#include "payload_block.h"

using namespace kernel::index;

int main() {
	int m = 512;  // 示例值
	int n = 32;   // 示例值
	auto foo = PayloadBlockFactoryM<512, 1024, 4096>::create(m, n);
	printf("foo :%p,foo->min :%f\n", foo, foo->min_);
	return 0;
}
