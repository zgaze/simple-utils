#include "payload_block_v1.h"


using namespace kernel::index;

int main() {
	PayloadBlock<48*6>* block = new PayloadBlock<48*6>();
	block->min_ = 0.0;
	block->max_ =  100.0;
	block->next_ = -1;
	block->size_ = 6;
	block->payload_size_ = 6 * sizeof(int64_t);
	char* begin = &block->payloads_[0];
	for (int i = 0; i < block->size_; ++i) {
		Payload* p_begin = reinterpret_cast<Payload*>(reinterpret_cast<char*>(begin));
		int32_t x = i + 1;
		p_begin->doc_id = 1L * x;
		p_begin->score = 1.0 * x;
		begin += sizeof(int64_t) * 2;
		int64_t* d1 = (int64_t*)begin;
		*d1 = 100 * x + 1;
		begin += sizeof(int64_t);
		int64_t* d2 = (int64_t*)begin;
		*d2 = 100 * x + 2;
		begin += sizeof(int64_t);
		int64_t* d3 = (int64_t*)begin;
		*d3 = 100 * x + 3;
		begin += sizeof(int64_t);
		int64_t* d4 = (int64_t*)begin;
		*d4 = 100 * x + 4;
		begin += sizeof(int64_t);
	}
	int64_t key = 3;
	Payload* res = block->FindKeyFromBlock((const char*)&key, sizeof(int64_t), 4.0, 6 * sizeof(int64_t));
	if (res != nullptr) {
		printf("data->doc_id %d, score:%f\n", res->doc_id, res->score );
	} else {
		printf(" not found\n");
	}
	return 0;
}
