#include "search_server.h"
#include "iterator_range.h"
#include "parse.h"
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>
#include <iostream>

std::string_view ReadToken(std::string_view& sv) {
	StripLeftWhitespaces(sv);
	auto result = sv.substr(0, sv.find(' '));
	sv.remove_prefix((sv.find(' ') != sv.npos) ? sv.find(' ') : sv.size());
	return result;
}

std::vector<std::string_view> SplitIntoWordsView(std::string_view line) {
	std::vector<std::string_view> result;
	for (std::string_view word = ReadToken(line); !word.empty(); word = ReadToken(line))
		result.push_back(word);
	return result;
}

InvertedIndex::InvertedIndex(std::istream& document_input) {
	for (std::string document; getline(document_input, document); ) {
		docs.push_back(move(document));

		size_t docid = docs.size() - 1;
		for (std::string_view word : SplitIntoWordsView(docs.back())) {
			auto& word_docs = index[word];
			if (word_docs.empty() || word_docs.back().docid != docid) 
				word_docs.push_back({ 1, docid });
			else 
				word_docs.back().count++;
		}
	}
}

const std::vector<InvertedIndex::Results>& InvertedIndex::Lookup(std::string_view word) const {
	static const std::vector<Results> empty_result;
	return (index.find(word) != index.end()) ? index.find(word)->second : empty_result;
}

void UpdateDocumentBaseAsync(std::istream& document_input, Synchronized<InvertedIndex>& index) {
	InvertedIndex new_index(document_input);
	std::swap(index.GetAccess().ref_to_value, new_index);
}

void AddQueriesStreamAsync(std::istream& query_input, std::ostream& search_results_output, Synchronized<InvertedIndex>& index_handle) {
	std::vector<size_t> docid_count;
	std::vector<int64_t> docids;
	for (std::string current_query; getline(query_input, current_query); ) {
		{
			auto access = index_handle.GetAccess().ref_to_value;
			const size_t docs_count = access.GetDocuments().size();
			docid_count.assign(docs_count, 0);
			docids.resize(docs_count);
			for (std::string_view word : SplitIntoWordsView(current_query))
				for (const auto& result : access.Lookup(word))
					docid_count[result.docid] += result.count;
		}

		iota(docids.begin(), docids.end(), 0);
		partial_sort(
			docids.begin(),
			Head(docids, 5).end(),
			docids.end(),
			[&docid_count](int64_t lhs, int64_t rhs) {
			return std::make_pair(docid_count[lhs], -lhs) > std::make_pair(docid_count[rhs], -rhs);
		}
		);

		search_results_output << current_query << ':';
		for (size_t docid : Head(docids, 5)) {
			const size_t hitcount = docid_count[docid];
			if (hitcount == 0) break;

			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << hitcount << '}';
		}
		search_results_output << '\n';
	}
}

void SearchServer::UpdateDocumentBase(std::istream& document_input) {
	futures.push_back(async(UpdateDocumentBaseAsync, std::ref(document_input), std::ref(index)));
}

void SearchServer::AddQueriesStream(std::istream& query_input, std::ostream& search_results_output) {
	futures.push_back(async(AddQueriesStreamAsync, std::ref(query_input), std::ref(search_results_output), std::ref(index)));
}


