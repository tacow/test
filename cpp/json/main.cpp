#include <stdio.h>
#include <string>
#include <json/json.h>
 
using namespace std;
using namespace Json;

#define BOOL_TO_STRING(b) ((b) ? "true" : "false")

int main() {
    string content = "[{\"name\": \"长安街\", \"len\": 10000}, {\"name\": \"阜石路\", \"len\": 5000}, {\"name\": \"莲石路\"}]";

    Reader reader;
    Value root;
    if (!reader.parse(content, root)) {
        printf("解析失败\n");
        return 0;
    }
 
    UInt size = root.size();
    for(UInt i = 0; i < size; ++i) {
        Value& item = root[i];
        printf("Has \"name\" %s\n", BOOL_TO_STRING(item.isMember("name")));
        printf("Has \"length\" %s\n", BOOL_TO_STRING(item.isMember("len")));

        string name = item["name"].asString();
        int len = item["len"].asInt();
        printf("%s: %d\n\n", name.c_str(), len);
    }
    return 0;
}

