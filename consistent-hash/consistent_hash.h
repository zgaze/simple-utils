/* ***********************************************************************

  > File Name: consistent_hash.h
  > Author: zzy
  > Mail: 942744575@qq.com 
  > Created Time: Fri 17 May 2019 03:58:50 PM CST

 ********************************************************************** */

#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include <stdint.h>
#include <map>
#include <functional>
#include <vector>
#include <string>

template<typename T>
class ConsistentHash {
public:
	ConsistentHash(std::vector<T> nodes, int replicas) :
		replicas_num_(replicas)
	{
		for (auto & node : nodes) {
			Add(node);
		}
	}
	~ConsistentHash()
	{
	}
	
	void Add(T node) {
		for (int i = 0; i < replicas_num_; ++i) {
			circle_.insert({(uint32_t)hash_func_(node + std::to_string(i)), node});
		}
	}

	void Remove(T node) {
		for (int i = 0; i < replicas_num_; ++i) {
			circle_.erase((uint32_t)hash_func_(node + std::to_string(i)));
		}
	}

	int Get(std::string & key, T & node){
		if (circle_.empty()) {
			return -1;
		}
		uint32_t hash = hash_func_(key);
		auto iter = circle_.lower_bound(hash);
		if (iter == circle_.end()) { // 
			iter = circle_.begin();
		}
		node = iter->second;
		return 0;
	}
private:
	std::map<int32_t, T> circle_;
	int32_t replicas_num_;     // 每个server多少个虚拟节点
	std::hash<std::string> hash_func_;
	//std::function<int32_t()> hash_func_;
};

#endif // CONSISTENT_HASH_H

