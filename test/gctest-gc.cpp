/*
 * test-gc.cpp
 *
 *  Created on: Jun 16, 2019
 *      Author: iconmaster
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <thread>

#include "ccl/gc.hpp"

using namespace std;
using namespace ccl;

struct _GcTest;
using Test = Gc<_GcTest>;

struct _GcTest : public GcAble {
	int id;
	list<Test> children;
	
	_GcTest(int id) : id{id} {}
	~_GcTest() {}
	
	void gcGetChildren(std::function<void(GcNodeRef)> addChild) override {
		for (Test& c : children) {
			addChild(c);
		}
	}
};

BOOST_AUTO_TEST_SUITE(gc);

BOOST_AUTO_TEST_CASE(singleThread) {
	gcSoftLimit = gcHardLimit = 1024;
	
	BOOST_CHECK_NO_THROW(
		GcNode::runMajorGC();
		unsigned i = 0;
		
		while (i < 100000) {
			Test a{i++};
			Test b{i++};
			Test c{i++};
			
			a->children.emplace_front(b);
			a->children.emplace_front(c);
			b->children.emplace_front(a);
		}
	);
}

BOOST_AUTO_TEST_CASE(multiThread) {
	gcSoftLimit = gcHardLimit = 1024;
	
	BOOST_CHECK_NO_THROW(
		GcNode::runMajorGC();
		list<thread> threads;
		unsigned i = 0;
		
		for (int n = 0; n < 8; n++) {
			threads.emplace_back([&]() {
				while (i < 100000) {
					Test a{i++};
					Test b{i++};
					Test c{i++};
					
					a->children.emplace_front(b);
					a->children.emplace_front(c);
					b->children.emplace_front(a);
				}
			});
		}
		
		for (thread& thread : threads) {
			thread.join();
		}
	);
}

BOOST_AUTO_TEST_SUITE_END();
