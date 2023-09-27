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

class Index {
	bool Insert(const KvPair& kv) = 0;
  bool Delete(const KvPair& kv) = 0;
	bool Search(KvPair* kv) = 0;
};

// 还是要多个index公用一个文件内存池
template<int N = min_payload_block_size>
class RadixSkipList : public Index {
 public:
	RadixSkipList();
	~RadixSkipList();

	bool Insert(const KvPair& kv) override;
  bool Delete(const KvPair& kv) override;
	bool Search(KvPair* kv) override;
	struct RadixSkipListHeader {
		int64_t total_size_;
		int32_t block_size_ = min_payload_block_size;
		int32_t payload_size_;
	};
 private:
	RadixSkipListHeader *header_;
	SkipListLevelIndex* level_index_;	
	PayloadBlock<N>* payload_;
};

template<int N>
bool RadixSkipList<N>::Insert(const KvPair& kv) {
	
}

template<int N>
Payload* RadixSkipList<N>::GetPayloadByKeyScore(
	const char* key, int32_t len, double score) {
	return payload_->FindKeyFromBlock(key, len, score);
}

template<int N>
bool RadixSkipList<N>::Search(KvPair* kv) {
	Payload* data = GetPayloadByKeyScore(kv->key, kv->len, kv->score);
}

template<int N>
bool RadixSkipList<N>::LoadFromMemory(const char* const_begin) {
	char* begin = const_cast<char*>(const_begin);
	header_ = reinterpret_cast<RadixSkipListHeader*>(begin);
	begin += sizeof(RadixSkipListHeader);
	level_index_ = reinterpret_cast<SkipListLevelIndex*>(begin);
	begin += sizeof(SkipListLevelIndex);
	payload_ = reinterpret_cast<PayloadBlock<N>*>(begin);
	begin += sizeof(PayloadBlock<N>);
}

} // namespace index
} // namespace kernel
