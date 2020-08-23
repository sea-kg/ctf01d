#ifndef WSJCPP_YAML_H
#define WSJCPP_YAML_H

#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ---------------------------------------------------------------------

enum WsjcppYamlItemType {
    WSJCPP_YAML_ITEM_UNDEFINED,
    WSJCPP_YAML_ITEM_EMPTY,
    WSJCPP_YAML_ITEM_ARRAY,
    WSJCPP_YAML_ITEM_MAP,
    WSJCPP_YAML_ITEM_VALUE
};

// ---------------------------------------------------------------------

class WsjcppYamlPlaceInFile {
    public:
        WsjcppYamlPlaceInFile();
        WsjcppYamlPlaceInFile(const std::string &sFilename, int nNumberOfLine, const std::string &sLine);

        std::string getFilename() const;
        void setFilename(const std::string &sFilename);

        int getNumberOfLine() const;
        void setNumberOfLine(int nNumberOfLine);

        std::string getLine() const;
        void setLine(const std::string &sLine);
        std::string getForLogFormat();

    private:
        std::string m_sFilename;
        int m_nNumberOfLine;
        std::string m_sLine;
};

// ---------------------------------------------------------------------
/*!
	\brief Class for keep data of yaml node

	Basic class for yaml tree
*/

class WsjcppYamlItem { // TODO: rename to node
    public:
        WsjcppYamlItem(
            WsjcppYamlItem *pParent,
            const WsjcppYamlPlaceInFile &placeInFile,
            WsjcppYamlItemType nItemType
        );
        ~WsjcppYamlItem();
        WsjcppYamlItem *getParent();

        WsjcppYamlPlaceInFile getPlaceInFile();
        void setPlaceInFile(const WsjcppYamlPlaceInFile &placeInFile);

        void setComment(const std::string &sComment);
        std::string getComment();

        void setName(const std::string &sName, bool bHasQuotes);
        std::string getName();
        bool hasNameDoubleQuotes();

        bool isEmpty();
        void doEmpty();

        bool isUndefined();
        void doArray();
        void doMap();
        void doValue();

        bool isMap();
        bool hasElement(const std::string &sName);
        WsjcppYamlItem *getElement(const std::string &sName);
        bool setElement(const std::string &sName, WsjcppYamlItem *pItem);
        bool removeElement(const std::string &sName);
        std::vector<std::string> getKeys();

        bool setElementValue(const std::string &sName, bool bHasNameQuotes, const std::string &sValue, bool bHasValueQuotes);
        bool createElementMap(const std::string &sName, bool bHasNameQuotes);
        WsjcppYamlItem *createElementMap();
        bool createElementArray(const std::string &sName, bool bHasNameQuotes);

        bool isArray();
        int getLength();
        WsjcppYamlItem *getElement(int i);
        bool appendElement(WsjcppYamlItem *pItem);
        bool appendElementValue(const std::string &sValue, bool bHasValueQuotes);
        bool removeElement(int i);

        bool isValue();
        std::string getValue();
        void setValue(const std::string &sValue, bool bHasQuotes);
        bool hasValueDoubleQuotes();

        std::string toString(std::string sIntent = "");
        std::string getItemTypeAsString();

        WsjcppYamlItem &operator[](int idx) { return *(this->getElement(idx)); }
        WsjcppYamlItem &operator[](const std::string &sName) { return *(this->getElement(sName)); }

        std::string getForLogFormat();

    private:
        std::string TAG;
        WsjcppYamlItem *m_pParent;
        WsjcppYamlPlaceInFile m_placeInFile;
        WsjcppYamlItemType m_nItemType;
        std::vector<WsjcppYamlItem *> m_vObjects;
        std::string m_sValue; // if it is not array or map
        bool m_bValueHasDoubleQuotes;
        std::string m_sName;
        bool m_bNameHasDoubleQuotes;
        std::string m_sComment;
};

// ---------------------------------------------------------------------

enum WsjcppYamlParserLineStates {
    NO,
    VALUE,
    COMMENT,
    STRING,
    ESCAPING
};

// ---------------------------------------------------------------------

class WsjcppYamlParsebleLine {
    public:
        WsjcppYamlParsebleLine(int nLine);
        WsjcppYamlParsebleLine();
        int getLineNumber();

        std::string getPrefix();
        int getIntent(); // prefix length
        bool isArrayItem();
        std::string getComment();
        std::string getName();
        bool hasNameDoubleQuotes();
        bool isEmptyName();
        std::string getValue();
        bool hasValueDoubleQuotes();
        bool isEmptyValue();

        void parseLine(const std::string &sLine);

    private:
        std::string TAG;
        int m_nLine;

        std::string m_sPrefix;
        bool m_bArrayItem;
        std::string m_sComment;
        std::string m_sName;
        std::string m_sValue;
        bool m_bNameHasQuotes;
        bool m_bValueHasQuotes;

        std::string removeStringDoubleQuotes(const std::string &sValue);
};

// ---------------------------------------------------------------------

class WsjcppYamlParserStatus {
    public:
        int nIntent;
        WsjcppYamlItem *pCurItem;
        WsjcppYamlParsebleLine line;
        WsjcppYamlPlaceInFile placeInFile;
        void logUnknownLine(const std::string &sPrefix);
};

// ---------------------------------------------------------------------

class WsjcppYaml {
    public:
        WsjcppYaml();
        ~WsjcppYaml();
        bool loadFromFile(const std::string &sFileName);
        bool saveToFile(const std::string &sFileName);
        bool loadFromString(const std::string &sBuffer);
        bool loadFromString(std::string &sBuffer);
        bool saveToString(std::string &sBuffer);

        WsjcppYamlItem *getRoot();
        WsjcppYamlItem &operator[](int idx) { return *(getRoot()->getElement(idx)); }
        WsjcppYamlItem &operator[](const std::string &sName) { return *(getRoot()->getElement(sName)); }

    private:
        std::string TAG;
        
        std::vector<std::string> splitToLines(const std::string &sBuffer);
        bool parse(const std::string &sFileName, const std::string &sBuffer);
        void process_sameIntent_hasName_emptyValue_arrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_hasName_emptyValue_noArrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_hasName_hasValue_arrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_hasName_hasValue_noArrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_emptyName_hasValue_arrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_emptyName_hasValue_noArrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_emptyName_emptyValue_arrayItem(WsjcppYamlParserStatus &st);
        void process_sameIntent_emptyName_emptyValue_noArrayItem(WsjcppYamlParserStatus &st);

        std::vector<std::string> m_sLines;
        WsjcppYamlItem *m_pRoot;
};

#endif // WSJCPP_YAML_H

