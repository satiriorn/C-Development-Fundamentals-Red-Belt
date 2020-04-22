#pragma once

#include <cstdlib>

template <typename T>
class SimpleVector {
public:
	SimpleVector() : v(nullptr), size(0), space(0), End(nullptr){};
	explicit SimpleVector(size_t s) {
		v = new T[s];
		size = space = s;
		End = v + size;
	}
	~SimpleVector() {delete[]v;}

	T& operator[](size_t index) { return v[index]; }

	T* begin() { return &v[0]; }
	T* end() { return End; }

	const T* begin() const { return &v[0]; }
	const T* end()const { return End; }

	size_t Size() const { return size; }

	size_t Capacity() const { return space; }

	void PushBack(const T& value) {
		if (Size() == 0) 
			v = new T[++space];
		else if (Size() == Capacity()) {
			T* OldVector = v;
			space *= 2;
			v = new T[space];
			for (size_t i = 0; i < Size(); ++i)
				v[i] = OldVector[i];
			delete[] OldVector;
		}
		v[size++] = value;
		End = v + size;
	}

private:
	T *v,*End;
	size_t size, space;
};