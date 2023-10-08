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
class PayloadBlock {
	PayloadBlock();
	~PayloadBlock();
	Payload* FindKeyFromBlock(const char* key, int32_t len,
		double score, int32_t payload_size);
	bool Insert(int64_t doc_id, double score, char* payload_data);
	double min_;
	double max_;
	int32_t next_;
	int32_t size_;
	int32_t payload_size_;
	char payloads_[N]; 
};

// TODO 需要改成尾插
template<int N> 
bool PayloadBlock<N>::Reblance(PayloadBlock<N>* block, double mid_score) {
	block->min_ = this->min_;
	int32_t offset = 0;
	for (int i = 0; i < this->size + 1/2; ++i) {
		memcpy(&new_payload->payloads_[offset],
			payloads_[offset], payload_size_);
		Payload* payload = reinterpret_cast<Payload*>&new_payload->payloads_[offset];
		block->max = payload->score;
		offset += payload_size_;
	}
	block->next_ = this;
	Payload* payload = reinterpret_cast<Payload*>&new_payload->payloads_[offset];
	this->min = payload->score;
}

template<int N>
bool PayloadBlock<N>::Insert(int64_t doc_id, double score,
	char* payload_data, PayloadBlock<N>* new_payload) {
	// 不存在, index里面
	if (score < min_) {
		new_payload = new PayloadBlock<N>();
		new_payload->min = score;
		// 分裂
		size_t offset = 0;
		if (payload_size_ * (this->size + 1) >= N) {
			memcpy(&new_payload->payloads_[offset], payload_data, payload_size_);
			offset += payload_size_;
			for (int i = 0; i < this->size + 1/2; ++i) {
				memcpy(&new_payload->payloads_[offset],
					payloads_[offset -payload_size_], payload_size_);
				offset += payload_size_;
				Payload* payload = reinterpret_cast<Payload*>&new_payload->payloads_[offset];
				new_payload->max = payload->score; 
			}
		} else {
			memcpy(&new_payload->payloads_[0], payload_data, payload_size_);
			memcpy(&new_payload->payloads_[payload_size_], payloads_, this->size * payload_size_); 
			new_payload->max = this->max_;
		}
		new_payload->next = this->next_;
		new_payload->size = this->size_ + 1;
	} else if (score >= min_ && score <= max_) {
	}
}

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

