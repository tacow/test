#include <stdio.h>
#include <string>
#include <json/json.h>
 
using namespace std;
using namespace Json;

int main() {
    string content = "[{\"name\": \"长安街\", \"len\": 10000}, {\"name\": \"阜石路\", \"len\": 5000}]";

    Reader reader;
    Value root;
    if (!reader.parse(content, root)) {
        printf("解析失败\n");
        return 0;
    }
 
    UInt size = root.size();
    for(UInt i = 0; i < size; ++i) {
        Value& item = root[i];
        string name = item["name"].asString();
        int len = item["len"].asInt();
        printf("%s: %d\n", name.c_str(), len);
    }
    return 0;
}

