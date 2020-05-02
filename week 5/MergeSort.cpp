#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	if (range_end - range_begin < 2)
		return;
	vector<typename RandomIt::value_type> elements(make_move_iterator(range_begin), make_move_iterator(range_end));
	auto it1 = begin(elements) + elements.size() / 3;
	auto it2 = it1 + elements.size() / 3;

	MergeSort(std::begin(elements), it1);
	MergeSort(it1, it2);
	MergeSort(it2, std::end(elements));

	vector<typename RandomIt::value_type> tmp;
	
	merge(make_move_iterator(elements.begin()), make_move_iterator(it1), make_move_iterator(it1), make_move_iterator(it2), back_inserter(tmp));
    merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()), make_move_iterator(it2), make_move_iterator(elements.end()), range_begin);
}