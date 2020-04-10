// parser for *.conf on c++

#include "conf_file_parser.h"
#include <sys/stat.h>
#include <fstream>
#include <utils_logger.h>
#include <regex>        // regex, sregex_token_iterator
#include <cstdlib>

ConfFileParser::ConfFileParser(const std::string &sConfigFile) {
    m_sConfigFile = sConfigFile;
}

// ---------------------------------------------------------------------

void ConfFileParser::string_trim(std::string &sLine){
	// trim trailing spaces
	std::size_t endpos = sLine.find_last_not_of(" \t");
	std::size_t startpos = sLine.find_first_not_of(" \t");
	if( std::string::npos != endpos ) {
		sLine = sLine.substr( 0, endpos+1 );
		sLine = sLine.substr( startpos );
	} else {
		sLine.erase(std::remove(std::begin(sLine), std::end(sLine), ' '), std::end(sLine));
	}

	// trim leading spaces
	std::size_t nStartPos = sLine.find_first_not_of(" \t");
	if( std::string::npos != nStartPos ) {
		sLine = sLine.substr( nStartPos );
	}
}

// ---------------------------------------------------------------------

bool ConfFileParser::fileExists(const std::string &sFilename){
	struct stat buffer;   
	return (stat (sFilename.c_str(), &buffer) == 0); 
}

// ---------------------------------------------------------------------

bool ConfFileParser::parseConfig(){
	std::ifstream isConfigFile( m_sConfigFile );
	int nLineNumber = 0;
	for( std::string sLine; getline( isConfigFile, sLine ); ){
		nLineNumber++;
		std::string sOrigLine = sLine;
		std::size_t nFoundComment = sLine.find("#");
		if (nFoundComment != std::string::npos){
			// remove all after #
			sLine.erase (sLine.begin() + nFoundComment, sLine.end());
		}
		this->string_trim(sLine);
		if(sLine == ""){ // skip empty strings
			continue;
		}
		
		// std::cout << "Line (" << nLineNumber << "): [" << sLine << "]" << std::endl;
		
		std::size_t nFoundEqualChar = sLine.find("=");
		if (nFoundEqualChar != std::string::npos){
			// split name of param and value
			std::string sParamName = sLine;
			std::string sParamValue = sLine;
			
			sParamName.erase (sParamName.begin() + nFoundEqualChar, sParamName.end());
			sParamValue.erase (sParamValue.begin(), sParamValue.begin() + nFoundEqualChar + 1);
			this->string_trim(sParamName);
			this->string_trim(sParamValue);
			
			// std::cout << " [" << sParamName << "]  => [" << sParamValue << "]" << std::endl;
			
			if(m_mapConfigValues.count(sParamName)){
				Log::warn(TAG, "Ignoring duplicate of option line(" + std::to_string(nLineNumber) + ") in config: " + m_sConfigFile);
			}else{
				m_mapConfigValues.insert(std::pair<std::string,std::string>(sParamName, sParamValue));	
			}
		}else{
			Log::warn(TAG, "Ignoring invalid line(" + std::to_string(nLineNumber) + ") in config: " + m_sConfigFile);
		}
		
	}
	return true;
}

// ---------------------------------------------------------------------

std::string ConfFileParser::getStringValueFromConfig(const std::string &sParamName, const std::string &defaultValue){
	std::string sResult = defaultValue;

    if(m_mapConfigValues.count(sParamName)){
        sResult = m_mapConfigValues.at(sParamName);
    }else{
        Log::warn(TAG, sParamName + " - not found in " + m_sConfigFile + "\n\t Will be used default value: " + defaultValue);
    }
	return sResult;
}

// ---------------------------------------------------------------------

int ConfFileParser::getIntValueFromConfig(const std::string &sParamName, int defaultValue){
    int nResult = defaultValue;
    if (m_mapConfigValues.count(sParamName)) {
        std::string sParamValue = m_mapConfigValues.at(sParamName);
		nResult = std::atoi(sParamValue.c_str());
    } else {
        Log::warn(TAG, sParamName + " - not found in " + m_sConfigFile + "\n\t Will be used default value: " + std::to_string(defaultValue));
    }
    return nResult;
}

// ---------------------------------------------------------------------

bool ConfFileParser::getBoolValueFromConfig(const std::string &sParamName, bool defaultValue){
    bool bResult = defaultValue;

    if( m_mapConfigValues.count(sParamName)) {
        std::string sParamValue = m_mapConfigValues.at(sParamName);
		this->string_trim(sParamValue);
        std::transform(sParamValue.begin(), sParamValue.end(), sParamValue.begin(), ::tolower);
        if (sParamValue != "yes" && sParamValue != "no") {
			Log::err(TAG, sParamName + " - wrong value (expected 'yes' or 'no') in " + m_sConfigFile + "\n\t Will be used default value: " + (defaultValue ? "yes" : "no"));
		} else {
			bResult = sParamValue == "yes";
		}

    } else {
        Log::warn(TAG, sParamName + " - not found in " + m_sConfigFile + "\n\t Will be used default value: " + (defaultValue ? "yes" : "no"));
    }
    return bResult;
}