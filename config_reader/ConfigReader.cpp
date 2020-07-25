#include "ConfigReader.h"
#include <stdio.h>

ConfigReader::ConfigReader(ConfigProcessorIf* processor) {
    processor_ = processor;
    sectionStarted_ = false;
}

ConfigReader::~ConfigReader() {
}

bool ConfigReader::ParseFile(const string& configFilePath) {
    FILE* f = fopen(configFilePath.c_str(), "r");
    if (!f)
        return false;

    bool res = true;
    char buf[4096];
    while (fgets(buf, 4096, f)) {
        string line = buf;
        if (!line.empty() && *line.rbegin() == '\n')
            line = line.substr(0, line.size() - 1);

        if (line.empty()) {
            continue;
        } else if (line[0] == '#') {
            continue;
        } else if (line[0] == '[') {
            size_t len = line.size();
            if (len <= 2) {
                res = false;
                break;
            }
            if (line[len - 1] != ']') {
                res = false;
                break;
            }
            if (sectionStarted_)
                processor_->OnSectionEnd();
            processor_->OnSectionStart(line.substr(1, len - 2));
            sectionStarted_ = true;
        } else {
            if (!sectionStarted_) {
                res = false;
                break;
            }
            size_t pos = line.find('=');
            if (pos == string::npos || pos == 0) {
                res = false;
                break;
            }
            processor_->OnConfigItem(line.substr(0, pos), line.substr(pos + 1));
        }
    }
    if (res && sectionStarted_)
        processor_->OnSectionEnd();
    sectionStarted_ = false;
    fclose(f);
    return res;
}

