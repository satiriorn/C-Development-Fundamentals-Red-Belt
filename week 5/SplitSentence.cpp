#include "test_runner.h"
#include <vector>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    Sentence<Token> tmp;
    vector<Sentence<Token>> result;
    result.reserve(tokens.size());
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if (it == prev(tokens.end()) || (it->IsEndSentencePunctuation() && !next(it)->IsEndSentencePunctuation())){
            tmp.push_back(move(*it));
            result.push_back(move(tmp));
        }
        else
            tmp.push_back(move(*it));
    }

    if (result.empty()&&!tmp.empty() || result.back() != tmp&&!tmp.empty())
            result.push_back(move(tmp));
    result.shrink_to_fit();
    return result;
}