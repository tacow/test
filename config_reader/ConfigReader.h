#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>

using namespace std;

class ConfigProcessorIf {
public:
    virtual void OnSectionStart(const string& sectionName) = 0;
    virtual void OnSectionEnd() = 0;

    virtual void OnConfigItem(const string& key, const string& value) = 0;
};

class ConfigReader {
public:
    ConfigReader(ConfigProcessorIf* processor);
    ~ConfigReader();

    bool ParseFile(const string& configFilePath);

private:
    ConfigProcessorIf* processor_;

    bool sectionStarted_;
};

#endif

