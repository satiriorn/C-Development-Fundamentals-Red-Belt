#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

std::vector<std::string> SplitIntoWords(const std::string& line) {
	std::istringstream words_input(line);
  return { std::istream_iterator<std::string>(words_input), std::istream_iterator<std::string>()};
}

SearchServer::SearchServer(std::istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
  InvertedIndex new_index;

  for (std::string current_document; getline(document_input, current_document); ) {
    new_index.Add(move(current_document));
  }

  index = std::move(new_index);
}

void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) {
	std::vector<std::pair<size_t, size_t>> docid_count(50000, { 0,0 });
	for (std::string current_query; getline(query_input, current_query); ) {
		const auto words = SplitIntoWords(current_query);
		docid_count.assign(50000, { 0,0 });
		for (const auto& word : words) {
			std::vector<std::pair<size_t, size_t>> lookup_vector = index.Lookup(word);
			for (int i = 0; i < lookup_vector.size(); ++i) {
				docid_count[lookup_vector[i].first].first = lookup_vector[i].first;
				docid_count[lookup_vector[i].first].second += lookup_vector[i].second;
			}
		}
		partial_sort(
			begin(docid_count),
			begin(docid_count) + 5,
			end(docid_count),
			[](auto &left, auto &right) {
			return (left.second > right.second) || (left.second == right.second && left.first < right.first); }
		);
		search_results_output << current_query << ':';
		for (size_t i = 0; i < 5; ++i) {
			if (docid_count[i].second != 0) {
				search_results_output << " {"
					<< "docid: " << docid_count[i].first << ", "
					<< "hitcount: " << docid_count[i].second << '}';
			}
		}
		search_results_output << std::endl;
	}
}

void InvertedIndex::Add(const std::string& document) {
	docs.push_back(document);
	const size_t docid = docs.size() - 1;
	for (const auto& word : SplitIntoWords(document)) {
		if (index[word].empty()) 
			index[word].reserve(5000);
		if (docid >= index[word].size()) 
			index[word].push_back({ docid, 1 });
		else 
			index[word][docid].second++;
	}
}

std::vector<std::pair<size_t, size_t>> InvertedIndex::Lookup(const std::string& word) const {
	if (index.count(word) != 0) 
		return index.at(word);
	else 
		return {};
}
