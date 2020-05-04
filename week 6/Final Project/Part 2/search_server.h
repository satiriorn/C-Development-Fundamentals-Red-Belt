#pragma once
#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <string_view>
#include <future>
#include <mutex>

template <typename T>
class Synchronized {
public:
	explicit Synchronized(T initial = T()): value(std::move(initial)){}

	struct Access {
		std::lock_guard<std::mutex> guard;
		T& ref_to_value;
	};

	Access GetAccess() {return { std::lock_guard<std::mutex>(m), value };}
private:
	T value;
	std::mutex m;
};

class InvertedIndex {
public:
	struct Results {
		size_t count, docid;
	};

	InvertedIndex() = default;
	explicit InvertedIndex(std::istream& document_input);
	const std::vector<Results>& Lookup(std::string_view word) const;
	const std::deque<std::string>& GetDocuments() const {return docs;}

private:
	std::map<std::string_view, std::vector<Results>> index;
	std::deque<std::string> docs;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(std::istream& document_input): index(InvertedIndex(document_input)){}

	void UpdateDocumentBase(std::istream& document_input);
	void AddQueriesStream(std::istream& query_input, std::ostream& search_results_output);

private:
	Synchronized<InvertedIndex> index;
	std::vector<std::future<void>> futures;
	
	
};

std::vector<std::string_view> SplitIntoWordsView(std::string_view line);
std::string_view ReadToken(std::string_view& sv);