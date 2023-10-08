#pragma once

#include "payload_block_v1.h" 

namespace kernel {
namespace index {

static constexpr int32_t min_payload_block_size = 256;
static constexpr int32_t max_payload_block_size = 4096;

struct KvPair {
	KvPair(const void* k = nullptr, int32_t l = 0, int64_t v = 0) :
		key(k), len(l), value(v) {}
	const void* key;
	int32_t len;
	int64_t value;
	double score;
	int32_t patyload_len;
	char payload[];
};

struct SkipListLevelIndex {
	SkipListLevelIndex* next_;
	int64_t size_;
	struct LevelPair {
		double score_;
		SkipListLevelIndex* data_;
	};
	LevelPair lv[8];
};

class Index {
	bool Insert(const KvPair& kv) = 0;
  bool Delete(const KvPair& kv) = 0;
	bool Search(KvPair* kv) = 0;
};

char* mmap_data;

// 还是要多个index公用一个文件内存池
template<int N = min_payload_block_size>
class BatchSkipList : public Index {
 public:
	BatchSkipList() {
		header_ = new BatchSkipListHeader();
		header_->total_size_ = 0;
		header_->payload_size_ = 4 * sizeof(int64_t); 

		level_index_ = new SkipListLevelIndex();
		level_index_->next_ = nullptr;
	}
	~BatchSkipList() {
		delete header_;
		delete level_index_;
		delete payload_;
	}

	bool Insert(const KvPair& kv) override;
  bool Delete(const KvPair& kv) override;
	bool Search(KvPair* kv) override;
	struct BatchSkipListHeader {
		int64_t total_size_;
		int32_t block_size_ = min_payload_block_size;
		int32_t payload_size_;
	};
 private:
	BatchSkipListHeader *header_;
	SkipListLevelIndex* level_index_;	
	PayloadBlock<N>* payload_;
};

template<int N>
bool BatchSkipList<N>::Insert(const KvPair& kv) {
	// 根据score 到index找到payload, 行为和 sort table的查找类似
	double score = kv->score_;
	PayloadBlock<N>* block = level_index_->FindMatchBlock(score); 
	if (block != nullptr) {
		block->Insert(kv);
	} else {
		// alloc 一个block
		block = nullptr;
	}
	level_index_->UpdateIndex(block, score);
	header_->total_size_ ++;
}

template<int N>
Payload* BatchSkipList<N>::GetPayloadByKeyScore(
	const char* key, int32_t len, double score) {
	return payload_->FindKeyFromBlock(key, len, score, header_->payload_size_);
}

template<int N>
bool BatchSkipList<N>::Search(KvPair* kv) {
	Payload* data = GetPayloadByKeyScore(kv->key, kv->len, kv->score);
	if (data == nullptr) {
		return false;
	}
	if (data->doc_id == *(int64_t*)(char*)kv->key) {
		kv->score = data->score;
		kv->payload = data->data_;
		return true;
	}
	return false;
}

template<int N>
bool BatchSkipList<N>::LoadFromMemory(const char* const_begin) {
	char* begin = const_cast<char*>(const_begin);
	header_ = reinterpret_cast<BatchSkipListHeader*>(begin);
	begin += sizeof(BatchSkipListHeader);
	level_index_ = reinterpret_cast<SkipListLevelIndex*>(begin);
	begin += sizeof(SkipListLevelIndex);
	payload_ = reinterpret_cast<PayloadBlock<N>*>(begin);
	begin += sizeof(PayloadBlock<N>);
}

} // namespace index
} // namespace kernel
