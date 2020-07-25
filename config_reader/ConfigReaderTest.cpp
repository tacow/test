#include "ConfigReader.h"
#include <stdio.h>

class ConfigProcessor : public ConfigProcessorIf {
public:
    void OnSectionStart(const string& sectionName);
    void OnSectionEnd();

    void OnConfigItem(const string& key, const string& value);
};


void ConfigProcessor::OnSectionStart(const string& sectionName) {
    printf("Section start: %s\n", sectionName.c_str());
}

void ConfigProcessor::OnSectionEnd() {
    printf("Section end\n");
}

void ConfigProcessor::OnConfigItem(const string& key, const string& value) {
    printf("Config: [%s] => [%s]\n", key.c_str(), value.c_str());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [config file]\n", argv[0]);
        return 0;
    }

    ConfigProcessor processor;
    ConfigReader reader(&processor);
	bool res = reader.ParseFile(argv[1]);
    printf(res ? "Parse succeed\n" : "Parse failed\n");
    return 0;
}

