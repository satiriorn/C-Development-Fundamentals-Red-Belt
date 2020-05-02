#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using Id = std::size_t;

template <typename T>
class PriorityCollection
{
public:
    PriorityCollection() = default;
    ~PriorityCollection() = default;

    using Id = std::size_t;

    Id Add(T object){
		Id obj = objects.size();
		objects.push_back({ std::move(object), 0 });
		priority_order.insert({ 0, obj });
		return obj;
	}

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin){
		 while (range_begin != range_end){*(ids_begin++) = this->Add(std::move(*(range_begin++)));}
	}

    bool IsValid(Id id) const{ return ((id < objects.size()) && (objects[id].priority != -1)); }

    const T& Get(Id id) const{ return objects[id].object; }

    void Promote(Id id){
		Object& obj = objects[id];
		priority_order.erase({ obj.priority++, id });
		priority_order.insert({ obj.priority, id });
	}

    std::pair<const T&, int> GetMax() const{
		std::set<std::pair<int, Id>>::const_iterator it = std::prev(priority_order.cend());
		return { objects[it->second].object, it->first };
	}

    std::pair<T, int> PopMax(){
		std::set<std::pair<int, Id>>::iterator it = std::prev(priority_order.end());
		Object& pr = objects[it->second];
		priority_order.erase(it);
		const int priority = pr.priority;
		pr.priority =-1;
		return { std::move(pr.object), priority };
	}
    
private:
    struct Object{
        T object;
        int priority;
    };

    std::vector<Object> objects;
    std::set<std::pair<int, Id>> priority_order;
};

