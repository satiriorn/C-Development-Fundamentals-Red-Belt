#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <utility>
#include <future>
#include <algorithm>
#include <list>
#include <iterator>
using namespace std;

const int max_counter = 1e4;

struct Stats {
    std::map<std::string, int> word_frequences;
    void operator += (const Stats& other);
};

void Stats::operator += (const Stats& other){
    for (auto& [key, value] : other.word_frequences)
        word_frequences[key] += value;
}

std::vector<std::string> SplitIntoWords(const std::string& line){
    std::vector<std::string> result;
    std::string::const_iterator it1 = line.cbegin();
    std::string::const_iterator it2 = std::find(it1, line.cend(), ' ');
    result.reserve(2000000u);

    while (it2 != line.cend()){
        result.push_back(std::string(it1, it2));
        it1 = std::next(it2);
        it2 = std::find(it1, line.cend(), ' ');
    }
    result.push_back(std::string(it1, it2));
    result.shrink_to_fit();
    return result;
}

Stats ExploreLine(const std::set<std::string>& key_words, const std::string& line) {
    Stats result;
    for (const std::string& i : SplitIntoWords(line))
        if (key_words.count(i) != 0)
            result.word_frequences[i]++;
    return result;
}

Stats ExploreKeyWordsSingleThread(const std::set<std::string>& key_words, std::istream& input){
    Stats result;
    for (std::string line; std::getline(input, line);) 
        result += ExploreLine(key_words, line);
    return result;
}

Stats ExploreKeyWords(const std::set<std::string>& key_words, std::istream& input) {
    Stats result;
    std::list<std::string> lines;
    std::list<std::future<Stats>> futures;
    int counter = 0;
    lines.push_front(std::string());
    for (std::string line; std::getline(input, line);counter++){
        lines.push_front(std::string(std::move(lines.front()) + ' ' + std::move(line)));
        if (counter == max_counter){
            futures.push_back(std::async(ExploreLine, std::ref(key_words), std::ref(lines.front())));
            lines.push_front(std::string());
            counter = 0;
        }
    }

    if (!lines.front().empty())
        futures.push_back(std::async(ExploreLine, std::ref(key_words), std::ref(lines.front())));

    for (auto& i : futures)
        result += i.get();
    return result;
}


void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
