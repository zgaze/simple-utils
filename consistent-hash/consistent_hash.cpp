/* ***********************************************************************

  > File Name: consistent_hash.h
  > Author: zzy
  > Mail: 942744575@qq.com 
  > Created Time: Fri 17 May 2019 03:58:12 PM CST

 ********************************************************************** */

#include <stdio.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <climits>
#include <gtest/gtest.h>
#include "consistent_hash.h"

using std::vector;
using std::string;

TEST(testCase,test0) {
	std::vector<std::string> ips {"11", "22", "33", "44", "55", "66", "77", "88", "99"};
	ConsistentHash<std::string> c_hash(ips, 10);
	std::string ret;
	std::string k1 = "asdasda";
	c_hash.Get(k1, ret);std::cout << ret << "\n";
	c_hash.Remove("11");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Remove("33");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Remove("44");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Remove("22");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Add("11");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Add("33");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Add("44");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
	c_hash.Add("22");
	EXPECT_EQ(c_hash.Get(k1, ret), 0);std::cout << ret << "\n";
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}

