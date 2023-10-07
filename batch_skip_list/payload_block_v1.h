#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>


namespace kernel {
namespace index {

struct Payload {
	int64_t doc_id;
	double score;
	char data_[];
};

template<int N>
struct PayloadBlock {
	Payload* FindKeyFromBlock(const char* key, int32_t len, double score, int32_t payload_size);
	double min_;
	double max_;
	int32_t next_;
	int32_t size_;
	int32_t payload_size_;
	char payloads_[N]; 
};

template<int N>
Payload* PayloadBlock<N>::FindKeyFromBlock(const char* key, int32_t len,
	double score, int32_t payload_size) {
	if (score < min_) {
		return nullptr;
	} else if (score >= min_ && score <= max_)  {
		// payload size =  X ?
		int32_t payload_size = payload_size_;
		int32_t size = size_;
		char* payloads = (char*)&payloads_[0];
		Payload* begin = reinterpret_cast<Payload*>(payloads);
		Payload* end = reinterpret_cast<Payload*>(payloads + payload_size * size_);
		int low = 0, high = size;
		while (low <= high) {
			int mid = low + (high - low) / 2;
			Payload* mid_payload = reinterpret_cast<Payload*>(payloads + mid * payload_size);
			if (mid_payload->score == score) {
				return mid_payload;
			} else if (mid_payload->score > score) {
				high = mid - 1;
			} else {
				low = mid + 1;
			}
		}
		return nullptr;
		// 二分查找
	} else {
		// 找到next的block
		PayloadBlock<N>* next = reinterpret_cast<PayloadBlock<N>*>((char*)next);
		return next->FindKeyFromBlock(key, len, score, payload_size);
	}
}

} // namespace index
} // namespace kernel 

