#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate() {
		if (!res.empty()) {
			T *ptr = res.front();
			Set.insert(ptr);
			res.pop();
			return ptr;
		}

		T *ptr = new T;
		Set.insert(ptr);
		return ptr;
	}
	T* TryAllocate() {
		if (!res.empty()) {
			T *ptr = res.front();
			Set.insert(ptr);
			res.pop();
			return ptr;
		}
		return nullptr;
	}

	void Deallocate(T* obj) {
		if (Set.count(obj) != 0) {
			res.push(obj);
			Set.erase(obj);
		}
		else 
			throw invalid_argument("");
	}

	~ObjectPool() {
		for (auto &item : Set) 
			delete item;
		while (!res.empty()) {
			delete res.front();
			res.pop();
		}
	}

private:
	set<T*> Set;
	queue <T*> res;
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);
	return 0;
}
