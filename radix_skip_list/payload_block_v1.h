#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>


namespace kernel {
namespace index {

struct Payload {
	int64_t doc_id;
	char data_[];
};

template<int N>
struct PayloadBlock {
	double min_;
	double max_;
	int32_t next_;
	int32_t size_;
	int32_t payload_size_;
	char payloads_[N]; 
};

template<int N>
Payload* FindKeyFromBlock(const char* key, int32_t len, double score) {
	if (score < min_) {
		return nullptr;
	} else if (score >= min_ && score <= max_)  {
		// 二分查找
	} else {
		// 找到next的block
		RadixSkipList<N>* next = /* next */;
		return next->FindKeyFromBlock(key, len, score);
	}
}

} // namespace index
} // namespace kernel 

