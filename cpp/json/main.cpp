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
 
    int size = root.size();
    for(int i = 0; i < size; ++i) {
        string name = root[i]["name"].asString();
        int len = root[i]["len"].asInt();
        printf("%s: %d\n", name.c_str(), len);
    }
    return 0;
}

