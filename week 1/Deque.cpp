#include <vector>

using namespace std;

template<typename T>
class Deque {
public:
    Deque() : head({}), tail({}) {}

    const bool Empty() const { return head.empty() && tail.empty(); }

    const size_t Size() const { return head.size() + tail.size(); }

    T &operator[](size_t index) {return (index<head.size())? head[head.size() - 1 - index]:tail[index - head.size()];}

    const T &operator[](size_t index) const {return (index< head.size())?head[head.size() - 1 - index]:tail[index - head.size()];}

    T &At(size_t index) {return (index< head.size())?head.at(head.size() - 1 - index):tail.at(index - head.size());}

    const T &At(size_t index) const {return (index < head.size())?head.at(head.size() - 1 - index):tail.at(index - head.size());}

    T &Front() {return (!head.empty())?head.back():tail.front();}

    const T &Front() const {return (!head.empty())?head.back():tail.front(); }

    T &Back() {return (!tail.empty())?tail.back():head.front();}

    const T &Back() const {return (!tail.empty())?tail.back():head.front();}

    void PushFront(T item) { head.push_back(item); }

    void PushBack(T item) { tail.push_back(item); }

private:
    vector<T> head, tail;
};