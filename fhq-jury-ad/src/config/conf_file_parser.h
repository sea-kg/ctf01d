#ifndef CONF_FILE_PARSER_H
#define CONF_FILE_PARSER_H

#include <string>
#include <map>

class ConfFileParser {
    public:
        ConfFileParser(const std::string &sConfigFile);

        bool parseConfig();
        std::string getStringValueFromConfig(const std::string &sParamName, const std::string &defaultValue);
        int getIntValueFromConfig(const std::string &sParamName, int defaultValue);
        bool getBoolValueFromConfig(const std::string &sParamName, bool defaultValue);

    private:
        std::string TAG;
        std::string m_sConfigFile;

        bool fileExists(const std::string &sFilename);
        void string_trim(std::string &sLine);
        std::map<std::string,std::string> m_mapConfigValues;
};

#endif // CONF_FILE_PARSER_H
