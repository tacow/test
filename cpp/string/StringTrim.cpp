#include <string>

using namespace std;

template<class T>
string Trim(const string& str, T space)
{
    size_t sPos = str.find_first_not_of(space);
    if (sPos == string::npos)
        return string();

    size_t ePos = str.find_last_not_of(space);
    if (sPos == 0 && ePos + 1 == str.size())
        return str;
    return str.substr(sPos, ePos + 1 - sPos);
}

void TestTrim(const char* buf)
{
    string str = Trim(buf, ' ');
    printf("Trim(\"%s\") = \"%s\"\n", buf, str.c_str());

    string str2 = Trim(buf, " \t\r\n");
    printf("TrimEx(\"%s\") = \"%s\"\n", buf, str2.c_str());
}

int main()
{
    TestTrim("");
    TestTrim("  \n        ");
    TestTrim("   abc\n");
    TestTrim("def \n  ");
    TestTrim(" \t  ghi   ");
    TestTrim("jkl");
    return 0;
}
