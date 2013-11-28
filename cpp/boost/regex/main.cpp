#include <string>
#include <iostream>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

void MatchTest() {
    string str(" abc   def ");
    regex re("\\s(abc\\s+def)\\s");
    cout << regex_match(str, re) << endl;
}

void SearchTest() {
    string str(" abc   def ");
    regex re("([a-z]+)\\s+([a-z]+)");
    smatch m;
    if (regex_search(str, m, re)) {
        for(size_t i = 0; i < m.size(); ++i)
            cout << m[i].str() << endl;
    }
}

void ReplaceTest() {
    string str(" #abc#   #def# ");
    regex re("#[a-z]+#");
    cout << regex_replace(str, re, "(replaced)") << endl;
}

int main() {
    MatchTest();
    SearchTest();
    ReplaceTest();
    return 0;
}

