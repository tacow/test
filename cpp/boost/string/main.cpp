#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

void CaseTest() {
    string str("hello, world");
    cout << str << endl;
    to_upper(str);
    cout << str << endl;
}

void TrimTest() {
    string str("  __abc  def##  ");
    cout << str << endl;
    trim(str);
    cout << str << endl;
}

void StartEndTest() {
    string str("abcdef");
    cout << starts_with(str, "abc") << " " << starts_with(str, "def") << endl;
    cout << ends_with(str, "abc") << " " << ends_with(str, "def") << endl;
}

void ReplaceTest() {
    string str("abc1 abc2 abc3");
    cout << str << endl;
    replace_first(str, "abc", "def");
    cout << str << endl;
    replace_all(str, "abc", "def");
    cout << str << endl;
}

void SplitTest() {
    string str("abc,def,ghi,jkl;mno,pqr;stu,vwx;yz");
    cout << str << endl;
    vector<string> tokens;
    split(tokens, str, is_any_of(",;"));
    for(size_t i = 0; i < tokens.size(); ++i)
        cout << tokens[i] << endl;
}

int main() {
    CaseTest();
    cout << "--------------------" << endl;
    TrimTest();
    cout << "--------------------" << endl;
    StartEndTest();
    cout << "--------------------" << endl;
    ReplaceTest();
    cout << "--------------------" << endl;
    SplitTest();

    return 0;
}

