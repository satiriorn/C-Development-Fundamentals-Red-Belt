#include <algorithm>
#include <set>
#include <vector>
#include <string>

using namespace std;
class Learner {
public:

  int Learn(const vector<string>& words) {
    int counter = dict.size();
    for (const string & w : words) 
      dict.insert(w);
    return dict.size() - counter;
  }

  vector<string> KnownWords() {return {dict.begin(), dict.end()};}
private:
  set<string> dict;
};