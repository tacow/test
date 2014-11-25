#include <stdio.h>
#include <string>
#include <json/json.h>
 
using namespace std;
using namespace Json;

int main() {
    string content = "[{\"name\": \"������\", \"len\": 10000}, {\"name\": \"��ʯ·\", \"len\": 5000}]";

    Reader reader;
    Value root;
    if (!reader.parse(content, root)) {
        printf("����ʧ��\n");
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

