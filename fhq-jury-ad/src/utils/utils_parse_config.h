#ifndef UTILS_PARSE_CONF_H
#define UTILS_PARSE_CONF_H

#include <string>
#include <map>

class UtilsParseConfig {
	public:
		UtilsParseConfig(const std::string &sConfigFile);

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

#endif // UTILS_PARSE_CONF_H
