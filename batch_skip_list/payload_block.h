#pragma once

#include <stdint.h>
#include <stdio.h>
#include <vector>


namespace kernel {
namespace index {


template<int N>
class Payload final {
	static_assert(N % 8 == 0 && N <= 256, "");
	int64_t doc_id;
	char data_[N];
};

template<int M, int N>
class PayloadBlock final {
	double min_;
	double max_;
	int32_t next_;
	int32_t size_;
	Payload<N> payload_[M]; 
};


template<int M, int... Ns>
struct PayloadBlockFactoryN {
	static PayloadBlock<M, 0>* create(int n) {
		return nullptr;
	}
};

template<int M, int FirstN, int... RestN>
struct PayloadBlockFactoryN<M, FirstN, RestN...> {
	static PayloadBlock<M, 0>* create(int n) {
		if (n == FirstN) return new PayloadBlock<M, FirstN>();
		return PayloadBlockFactoryN<M, RestN...>::create(n);
	}
};

template<int... Ms>
struct PayloadBlockFactoryM {
	static PayloadBlock<0, 0>* create(int m, int n) {
		return nullptr;
	}
};

template<int FirstM, int... RestM>
struct PayloadBlockFactoryM<FirstM, RestM...> {
	static PayloadBlock<0, 0>* create(int m, int n) {
		if (m == FirstM) return PayloadBlockFactoryN<FirstM, 8, 16, 32, 64, 128>::create(n);
		return PayloadBlockFactoryM<RestM...>::create(m, n);
	}
};


} // namespace index
} // namespace kernel 

