
#include "wsjcpp_yaml.h"
#include <wsjcpp_core.h>

// ---------------------------------------------------------------------
// WsjcppYamlPlaceInFile

WsjcppYamlPlaceInFile::WsjcppYamlPlaceInFile() {
    m_sFilename = "";
    m_nNumberOfLine = 0;
    m_sLine = "";
}

// ---------------------------------------------------------------------

WsjcppYamlPlaceInFile::WsjcppYamlPlaceInFile(const std::string &sFilename, int nNumberOfLine, const std::string &sLine) {
    m_sFilename = sFilename;
    m_nNumberOfLine = nNumberOfLine;
    m_sLine = sLine;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlPlaceInFile::getFilename() const {
    return m_sFilename;
}

// ---------------------------------------------------------------------

void WsjcppYamlPlaceInFile::setFilename(const std::string &sFilename) {
    m_sFilename = sFilename;
}

// ---------------------------------------------------------------------

int WsjcppYamlPlaceInFile::getNumberOfLine() const {
    return m_nNumberOfLine;
}

// ---------------------------------------------------------------------

void WsjcppYamlPlaceInFile::setNumberOfLine(int nNumberOfLine) {
    m_nNumberOfLine = nNumberOfLine;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlPlaceInFile::getLine() const {
    return m_sLine;
}

// ---------------------------------------------------------------------

void WsjcppYamlPlaceInFile::setLine(const std::string &sLine) {
    m_sLine = sLine;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlPlaceInFile::getForLogFormat() {
    return "(" + m_sFilename + ":" + std::to_string(m_nNumberOfLine) + "): " + m_sLine;
}

// ---------------------------------------------------------------------
// WsjcppYamlItem

WsjcppYamlItem::WsjcppYamlItem(
    WsjcppYamlItem *pParent, 
    const WsjcppYamlPlaceInFile &placeInFile,
    WsjcppYamlItemType nItemType
) {
    m_pParent = pParent;
    m_placeInFile.setFilename(placeInFile.getFilename());
    m_placeInFile.setLine(placeInFile.getLine());
    m_placeInFile.setNumberOfLine(placeInFile.getNumberOfLine());
    m_nItemType = nItemType;
    m_bValueHasDoubleQuotes = false;
    m_bNameHasDoubleQuotes = false;
    TAG = "WsjcppYamlNode";
}

// ---------------------------------------------------------------------

WsjcppYamlItem::~WsjcppYamlItem() {
    m_vObjects.clear();
}

// ---------------------------------------------------------------------

WsjcppYamlItem *WsjcppYamlItem::getParent() {
    return m_pParent;
}

// ---------------------------------------------------------------------

WsjcppYamlPlaceInFile WsjcppYamlItem::getPlaceInFile() {
    return m_placeInFile;
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::setPlaceInFile(const WsjcppYamlPlaceInFile &placeInFile) {
    m_placeInFile.setFilename(placeInFile.getFilename());
    m_placeInFile.setLine(placeInFile.getLine());
    m_placeInFile.setNumberOfLine(placeInFile.getNumberOfLine());
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::setComment(const std::string &sComment) {
    m_sComment = sComment;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlItem::getComment() {
    return m_sComment;
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::setName(const std::string &sName, bool bHasQuotes) {
    m_sName = sName;
    m_bNameHasDoubleQuotes = bHasQuotes;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlItem::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::hasNameDoubleQuotes() {
    return m_bNameHasDoubleQuotes;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::isEmpty() {
    return m_nItemType == WSJCPP_YAML_ITEM_EMPTY;
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::doEmpty() {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_ITEM_EMPTY;
    } else {
        WsjcppLog::throw_err(TAG, "Element already defined as '" + this->getItemTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::isUndefined() {
    return m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED;
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::doArray() {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_ITEM_ARRAY;
    } else {
        WsjcppLog::throw_err(TAG, "Element already defined as '" + this->getItemTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::doMap() {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_ITEM_MAP;
    } else {
        WsjcppLog::throw_err(TAG, "Element already defined as '" + this->getItemTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::doValue() {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_ITEM_VALUE;
    } else {
        WsjcppLog::throw_err(TAG, "Element already defined as '" + this->getItemTypeAsString() + "'");
    }
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::isMap() {
    return m_nItemType == WSJCPP_YAML_ITEM_MAP;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::hasElement(const std::string &sName) {
    if (m_nItemType != WSJCPP_YAML_ITEM_MAP) {
        WsjcppLog::throw_err(TAG, "hasElement('" + sName + "'): Element must be map");
    }
    for (int i = 0; i < m_vObjects.size(); i++) {
        if (m_vObjects[i]->getName() == sName) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

WsjcppYamlItem *WsjcppYamlItem::getElement(const std::string &sName) {
    if (m_nItemType != WSJCPP_YAML_ITEM_MAP) {
        WsjcppLog::throw_err(TAG, "getElement: Element must be map");
    }
    
    for (int i = 0; i < m_vObjects.size(); i++) {
        std::string sObjectName = m_vObjects[i]->getName();
        if (m_vObjects[i]->getName() == sName) {
            return m_vObjects[i];
        }
    }
    WsjcppLog::throw_err(TAG, "Element '" + sName + "' not found for " + this->getForLogFormat());  
    return nullptr;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::setElement(const std::string &sName, WsjcppYamlItem *pItem) {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_ITEM_MAP; // change item type to map on first element  
    }

    if (m_nItemType != WSJCPP_YAML_ITEM_MAP) {
        WsjcppLog::throw_err(TAG, "setElement, Element must be 'map' for " + pItem->getPlaceInFile().getForLogFormat());
    }
    
    if (this->hasElement(sName)) { // TODO remove previous element
        WsjcppLog::throw_err(TAG, "setElement: Map already has element with this name: '" + sName + "'");
    }
    m_vObjects.push_back(pItem); // TODO create clone
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::removeElement(const std::string &sName) {
    if (m_nItemType != WSJCPP_YAML_ITEM_MAP) {
        WsjcppLog::throw_err(TAG, "removeElement: Element must be map");
    }
    // TODO erase
    return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppYamlItem::getKeys() {
    if (m_nItemType != WSJCPP_YAML_ITEM_MAP) {
        WsjcppLog::throw_err(TAG, "getKeys: Element must be map");
    }
    std::vector<std::string> vKeys;
    for (int i = 0; i < m_vObjects.size(); i++) {
        WsjcppYamlItem *pItem = m_vObjects[i];
        if (pItem->isValue() || pItem->isMap() || pItem->isArray()) {
            std::string sName = pItem->getName();
            vKeys.push_back(sName);
        }
    }
    return vKeys;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::setElementValue(const std::string &sName, bool bHasNameQuotes, const std::string &sValue, bool bHasValueQuotes) {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        m_nItemType = WSJCPP_YAML_ITEM_MAP; // change item type to map on first element  
    }

    if (m_nItemType != WSJCPP_YAML_ITEM_MAP) {
        WsjcppLog::throw_err(TAG, "setElement, Element must be 'map' for " + this->getPlaceInFile().getForLogFormat());
    }
    
    if (this->hasElement(sName)) {
        WsjcppYamlItem *pItem = this->getElement(sName);
        pItem->setValue(sValue, bHasValueQuotes);
    } else {
        WsjcppYamlPlaceInFile pl;
        WsjcppYamlItem *pNewItem = new WsjcppYamlItem(this, pl, WsjcppYamlItemType::WSJCPP_YAML_ITEM_VALUE);
        pNewItem->setName(sName, bHasNameQuotes);
        pNewItem->setValue(sValue, bHasValueQuotes);
        this->setElement(sName, pNewItem);
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::createElementMap(const std::string &sName, bool bHasNameQuotes) {
    if (m_nItemType != WSJCPP_YAML_ITEM_MAP ) {
        WsjcppLog::throw_err(TAG, "createElementMap, Element must be 'map' for " + this->getPlaceInFile().getForLogFormat());
    }
    if (this->hasElement(sName)) {
        return false; // already exists
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlItem *pNewItem = new WsjcppYamlItem(this, pl, WsjcppYamlItemType::WSJCPP_YAML_ITEM_MAP);
    pNewItem->setName(sName, bHasNameQuotes);
    this->setElement(sName, pNewItem);
    return true;
}

// ---------------------------------------------------------------------

WsjcppYamlItem *WsjcppYamlItem::createElementMap() {
    if (m_nItemType != WSJCPP_YAML_ITEM_ARRAY ) {
        WsjcppLog::throw_err(TAG, "createElementMap, Element must be 'array' for " + this->getPlaceInFile().getForLogFormat());
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlItem *pNewItem = new WsjcppYamlItem(this, pl, WsjcppYamlItemType::WSJCPP_YAML_ITEM_MAP);
    this->appendElement(pNewItem);
    return pNewItem;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::createElementArray(const std::string &sName, bool bHasNameQuotes) {
    if (m_nItemType != WSJCPP_YAML_ITEM_MAP ) {
        WsjcppLog::throw_err(TAG, "createElementArray, Element must be 'map' for " + this->getPlaceInFile().getForLogFormat());
    }
    if (this->hasElement(sName)) {
        return false;
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlItem *pNewItem = new WsjcppYamlItem(this, pl, WsjcppYamlItemType::WSJCPP_YAML_ITEM_ARRAY);
    pNewItem->setName(sName, bHasNameQuotes);
    this->setElement(sName, pNewItem);
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::isArray() {
    return m_nItemType == WSJCPP_YAML_ITEM_ARRAY;
}

// ---------------------------------------------------------------------

int WsjcppYamlItem::getLength() {
    if (m_nItemType != WSJCPP_YAML_ITEM_ARRAY) {
        WsjcppLog::throw_err(TAG, "getLength, Element must be array for " + this->getForLogFormat());
    }
    int nCount = 0;
    for (int i = 0; i < m_vObjects.size(); i++) {
        if (!m_vObjects[i]->isEmpty()) {
            nCount++;
        }
    }
    return nCount;
}

// ---------------------------------------------------------------------

WsjcppYamlItem *WsjcppYamlItem::getElement(int i) {
    if (m_nItemType != WSJCPP_YAML_ITEM_ARRAY) {
        WsjcppLog::throw_err(TAG, "getElement, Element must be array");
    }
    int nCounter = -1;
    WsjcppYamlItem *pItem = nullptr;
    for (int n = 0; n < m_vObjects.size(); n++) {
        if (!m_vObjects[n]->isEmpty()) {
            nCounter++;
            if (nCounter == i) {
                pItem = m_vObjects[n];
                break; 
            }
        }
    }
    if (pItem == nullptr) {
        WsjcppLog::throw_err(TAG, "getElement(" + std::to_string(i) +  "), Out of range in array for '" + this->getPlaceInFile().getLine() + "'");
    }
    return pItem;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::appendElement(WsjcppYamlItem *pItem) {
    if (pItem->isEmpty()) {
        m_vObjects.push_back(pItem); // TODO clone object
        return true;
    }
    if (m_nItemType != WSJCPP_YAML_ITEM_ARRAY) {
        WsjcppLog::throw_err(TAG, "appendElement, Element must be array for " + this->getForLogFormat());
    }
    m_vObjects.push_back(pItem); // TODO clone object
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::appendElementValue(const std::string &sValue, bool bHasValueQuotes) {
    if (m_nItemType != WSJCPP_YAML_ITEM_ARRAY) {
        WsjcppLog::throw_err(TAG, "appendElementValue, Element must be array for " + this->getForLogFormat());
    }
    WsjcppYamlPlaceInFile pl;
    WsjcppYamlItem *pNewItem = new WsjcppYamlItem(this, pl, WsjcppYamlItemType::WSJCPP_YAML_ITEM_VALUE);
    pNewItem->setValue(sValue, bHasValueQuotes);
    return this->appendElement(pNewItem);
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::removeElement(int i) {
    if (m_nItemType != WSJCPP_YAML_ITEM_ARRAY) {
        WsjcppLog::throw_err(TAG, "appendElement, Element must be array for " + this->getForLogFormat());
    }
    int nCounter = -1;
    WsjcppYamlItem *pItem = nullptr;
    for (int n = 0; n < m_vObjects.size(); n++) {
        if (!m_vObjects[n]->isEmpty()) {
            nCounter++;
            if (nCounter == i) {
                pItem = m_vObjects[n];
                break; 
            }
        }
    }
    if (pItem == nullptr) {
        WsjcppLog::throw_err(TAG, "getElement(" + std::to_string(i) +  "), Out of range in array for '" + this->getPlaceInFile().getLine() + "'");
    }
    std::vector<WsjcppYamlItem *>::iterator it;
    for (it = m_vObjects.begin(); it != m_vObjects.end(); ++it) {
        if (*it == pItem) {
            m_vObjects.erase(it);
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::isValue() {
    return m_nItemType == WSJCPP_YAML_ITEM_VALUE;
}

// ---------------------------------------------------------------------

std::string  WsjcppYamlItem::getValue() {
    if (m_nItemType != WSJCPP_YAML_ITEM_VALUE) {
        WsjcppLog::throw_err(TAG, "getValue, Element must be value for " + this->getForLogFormat());
    }
    return m_sValue;
}

// ---------------------------------------------------------------------

void WsjcppYamlItem::setValue(const std::string &sValue, bool bHasQuotes) {
    if (m_nItemType != WSJCPP_YAML_ITEM_VALUE) {
        WsjcppLog::throw_err(TAG, "setValue, Element must be value for " + this->getForLogFormat());
    }
    m_bValueHasDoubleQuotes = bHasQuotes;
    m_sValue = sValue;
}

// ---------------------------------------------------------------------

bool WsjcppYamlItem::hasValueDoubleQuotes() {
    return m_bValueHasDoubleQuotes;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlItem::toString(std::string sIntent) {
    std::string sRet = "";
    if (this->isValue()) {
        if (m_bValueHasDoubleQuotes) {
            sRet = "\"" + m_sValue + "\"";
        } else {
            sRet = m_sValue;
        }
        if (m_sComment.length() > 0) {
            if (sRet.length() > 0) {
                sRet += " ";
            }
            sRet += "# " + m_sComment;
        }
    } else if (this->isEmpty()) {
        if (m_sComment.length() > 0) {
            sRet += sIntent + "# " + m_sComment;
        }
    } else if (this->isArray()) {
        for (int i = 0; i < m_vObjects.size(); i++) {
            WsjcppYamlItem *pItem = m_vObjects[i];
            if (pItem->isEmpty()) {
                sRet += sIntent + pItem->toString();
            } else if (pItem->isMap()) {
                std::string s = pItem->toString(sIntent + "  ");
                WsjcppCore::trim(s);
                sRet += sIntent + "- " + s;
            } else {
                sRet += sIntent + "- " + pItem->toString();
            }
            sRet += "\n";
        }
    } else if (this->isMap()) {
        for (int i = 0; i < m_vObjects.size(); i++) {
            WsjcppYamlItem *pItem = m_vObjects[i];
            if (pItem->isEmpty() ) {
                sRet += sIntent + pItem->toString();
                sRet += "\n";
            } else if (pItem->isArray() || pItem->isMap()) {
                if (pItem->hasNameDoubleQuotes()) {
                    sRet += sIntent + "\"" + pItem->getName() + "\":";
                } else {
                    sRet += sIntent + pItem->getName() + ":";
                }
                if (pItem->getComment().length() > 0) {
                    sRet += " # " + pItem->getComment(); 
                }
                sRet += "\n";
                sRet += pItem->toString(sIntent + "  ");
            } else {
                if (pItem->hasNameDoubleQuotes()) {
                    sRet += sIntent + "\"" + pItem->getName() + "\": " + pItem->toString();
                } else {
                    sRet += sIntent + pItem->getName() + ": " + pItem->toString();
                }
                sRet += "\n";
            }
        }
    } else {
        sRet = "TODO: undefined";
    }
    if (sIntent == "") {
        WsjcppCore::trim(sRet);
    }
    return sRet;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlItem::getItemTypeAsString() {
    if (m_nItemType == WSJCPP_YAML_ITEM_UNDEFINED) {
        return "undefined";
    } else if (m_nItemType == WSJCPP_YAML_ITEM_ARRAY) {
        return "array";
    } else if (m_nItemType == WSJCPP_YAML_ITEM_MAP) {
        return "map";
    } else if (m_nItemType == WSJCPP_YAML_ITEM_VALUE) {
        return "value";
    }
    return "unknown";
}

// ---------------------------------------------------------------------

std::string WsjcppYamlItem::getForLogFormat() {
    return m_placeInFile.getForLogFormat();
}

// ---------------------------------------------------------------------

WsjcppYamlParsebleLine::WsjcppYamlParsebleLine(int nLine) {
    TAG = "WsjcppYamlParsebleLine(line:" + std::to_string(nLine) + ")";
    m_nLine = nLine;
}

// ---------------------------------------------------------------------

WsjcppYamlParsebleLine::WsjcppYamlParsebleLine() 
: WsjcppYamlParsebleLine(-1) {

}

// ---------------------------------------------------------------------

int WsjcppYamlParsebleLine::getLineNumber() {
    return m_nLine;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getPrefix() {
    return m_sPrefix;
}

// ---------------------------------------------------------------------

int WsjcppYamlParsebleLine::getIntent() {
    return m_sPrefix.length();
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isArrayItem() {
    return m_bArrayItem;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getComment() {
    return m_sComment;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getName() {
    return m_sName;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::hasNameDoubleQuotes() {
    return m_bNameHasQuotes;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isEmptyName() {
    return m_sName.length() == 0;
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::getValue() {
    return m_sValue;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::hasValueDoubleQuotes() {
    return m_bValueHasQuotes;
}

// ---------------------------------------------------------------------

bool WsjcppYamlParsebleLine::isEmptyValue() {
    return m_sValue.length() == 0;
}

// ---------------------------------------------------------------------

void WsjcppYamlParsebleLine::parseLine(const std::string &sLine) {
    // reset variables
    m_bArrayItem = false;
    m_sPrefix = "";
    m_sComment = "";
    m_sName = "";
    m_sValue = "";
    m_bNameHasQuotes = false;
    m_bValueHasQuotes = false;

    WsjcppYamlParserLineStates state = WsjcppYamlParserLineStates::NO;
    for (int i = 0; i < sLine.length(); i++) {
        char c = sLine[i];
        if ((c == ' ' || c == '\t') && state == WsjcppYamlParserLineStates::NO) {
            m_sPrefix += c;
        } else if (c == '#' && (state == WsjcppYamlParserLineStates::NO || state == WsjcppYamlParserLineStates::VALUE)) {
            state = WsjcppYamlParserLineStates::COMMENT;
        } else if (state == WsjcppYamlParserLineStates::COMMENT) {
            if (c != '\r') {
                m_sComment += c;
            }
        } else if (c == '-' && state == WsjcppYamlParserLineStates::NO) {
            m_bArrayItem = true;
            state = WsjcppYamlParserLineStates::VALUE;
        } else if ((c != ' ' && c != '\t') && state == WsjcppYamlParserLineStates::NO) {
            state = WsjcppYamlParserLineStates::VALUE;
            m_sValue += c;
            if (c == '"') {
                state = WsjcppYamlParserLineStates::STRING;
            }
        } else if (c == '"' && state == WsjcppYamlParserLineStates::VALUE) {
            state = WsjcppYamlParserLineStates::STRING;
            m_sValue += c;
        } else if (c == '\\' && state == WsjcppYamlParserLineStates::STRING) {
            state = WsjcppYamlParserLineStates::ESCAPING;
            m_sValue += c;
        } else if (state == WsjcppYamlParserLineStates::ESCAPING) {
            state = WsjcppYamlParserLineStates::STRING;
            m_sValue += c;
        } else if (c == '"' && state == WsjcppYamlParserLineStates::STRING) {
            state = WsjcppYamlParserLineStates::VALUE;
            m_sValue += c;
        } else if (c == ':' && state == WsjcppYamlParserLineStates::VALUE) {
            if (m_sName.length() == 0) {
                m_sName = m_sValue;
                m_sValue = ""; // reset value it was param name
            } else {
                m_sValue += c;
            }
        } else if (state == WsjcppYamlParserLineStates::STRING) {
            m_sValue += c;
        } else if (state == WsjcppYamlParserLineStates::VALUE) {
            m_sValue += c;
        } else {
            // skip
        }
    }

    if (state == WsjcppYamlParserLineStates::STRING 
      || state == WsjcppYamlParserLineStates::ESCAPING
    ) {
        WsjcppLog::throw_err(TAG, "wrong format");
    }

    // split name and value
    /*for (int i = 0; i < m_sNameAndValue.size(); i++) {
        char c = m_sNameAndValue[i];
        if (c == ':' && m_sName.size() == 0) {
            m_sName = m_sValue;
            m_sValue = "";
        } else {
            m_sValue += c;
        }
    }*/
    
    WsjcppCore::trim(m_sName);
    if (m_sName.length() > 0 && m_sName[0] == '"') {
        m_bNameHasQuotes = true;
        m_sName = removeStringDoubleQuotes(m_sName);
    }

    WsjcppCore::trim(m_sValue);
    if (m_sValue.length() > 0 && m_sValue[0] == '"') {
        m_bValueHasQuotes = true;
        m_sValue = removeStringDoubleQuotes(m_sValue);
    }

    WsjcppCore::trim(m_sComment);
}

// ---------------------------------------------------------------------

std::string WsjcppYamlParsebleLine::removeStringDoubleQuotes(const std::string &sValue) {
    if (sValue.size() > 0 && sValue[0] != '"') {
        return sValue;
    }
    int nStartPos = 1;
    int nEndPos = sValue.size()-1;
    std::string sRet = "";
    bool bEscape = false;
    for (int i = nStartPos; i < nEndPos; i++) {
        char c = sValue[i];
        if (bEscape) {
            if (c == 'n') {
                sRet += '\n';
            } else if (c == 'r') {
                sRet += '\r';
            } else {
                sRet += c;
            }
        } else if (c == '\\') {
            bEscape = true;
        } else {
            sRet += c;
        }
    }
    return sRet;
}

// ---------------------------------------------------------------------
// WsjcppYamlParserStatus

void WsjcppYamlParserStatus::logUnknownLine(const std::string &sPrefix) {
    WsjcppLog::warn(sPrefix, "Unknown line (" + std::to_string(placeInFile.getNumberOfLine()) + "): '" + placeInFile.getLine() + "' \n"
        + "Current Intent: " + std::to_string(nIntent) +  "\n"
        + "Current Item(line: " + std::to_string(pCurItem->getPlaceInFile().getNumberOfLine()) + "): '" + pCurItem->getPlaceInFile().getLine() + "'"
        + "Current Item(filename: " + pCurItem->getPlaceInFile().getFilename() + "'"
    );
}

// ---------------------------------------------------------------------
// WsjcppYaml

WsjcppYaml::WsjcppYaml() {
    m_pRoot = new WsjcppYamlItem(nullptr, WsjcppYamlPlaceInFile(), WSJCPP_YAML_ITEM_MAP);
}

// ---------------------------------------------------------------------

WsjcppYaml::~WsjcppYaml() {
    delete m_pRoot;
}

// ---------------------------------------------------------------------

bool WsjcppYaml::loadFromFile(const std::string &sFileName) {
    std::string sTextContent;
    if (!WsjcppCore::readTextFile(sFileName, sTextContent)) {
        return false;    
    }
    return parse(sFileName, sTextContent);
}

// ---------------------------------------------------------------------

bool WsjcppYaml::saveToFile(const std::string &sFileName) {
    std::string sBuffer = m_pRoot->toString();
    if (!WsjcppCore::writeFile(sFileName, sBuffer)) {
        return false;    
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppYaml::loadFromString(const std::string &sBuffer) {
    return false;
}

// ---------------------------------------------------------------------

bool WsjcppYaml::loadFromString(std::string &sBuffer) {
    return parse("", sBuffer);
}

// ---------------------------------------------------------------------

bool WsjcppYaml::saveToString(std::string &sBuffer) { // TODO move to WsjcppCore
    sBuffer = m_pRoot->toString();
    return true;
}

// ---------------------------------------------------------------------

WsjcppYamlItem *WsjcppYaml::getRoot() {
    return m_pRoot;
}

// ---------------------------------------------------------------------

std::vector<std::string> WsjcppYaml::splitToLines(const std::string &sBuffer) {
    std::vector<std::string> vLines;
    std::string sLine = "";
    int nSize = sBuffer.length();
    for (int i = 0; i < nSize; i++) {
        char c = sBuffer[i];
        if (c == '\n') {
            vLines.push_back(sLine);
            sLine = "";
        } else {
            sLine += c;
        }
    }
    if (sLine.length() > 0) {
        vLines.push_back(sLine);
    }
    return vLines;
}

// ---------------------------------------------------------------------

bool WsjcppYaml::parse(const std::string &sFileName, const std::string &sBuffer) {
    std::vector<std::string> vLines = this->splitToLines(sBuffer);
    WsjcppYamlParserStatus st;
    st.pCurItem = m_pRoot; // TODO recreate again new root element
    st.placeInFile.setFilename(sFileName);
    st.nIntent = 0;
    m_pRoot->setPlaceInFile(st.placeInFile);

    for (int nLine = 0; nLine < vLines.size(); nLine++) {
        st.placeInFile.setLine(vLines[nLine]);
        // WsjcppLog::info(TAG, "Line(" + std::to_string(nLine) + ") '" + st.sLine + "'");
        st.placeInFile.setNumberOfLine(nLine);
        st.line = WsjcppYamlParsebleLine(nLine);
        st.line.parseLine(st.placeInFile.getLine());
        
        bool isEmptyName = st.line.isEmptyName();
        bool isEmptyValue = st.line.isEmptyValue();
        bool isArrayItem = st.line.isArrayItem();
        int nLineIntent = st.line.getIntent();
        int nDiffIntent = nLineIntent - st.nIntent;

        // TODO check comment
        /*if (isEmptyName && isEmptyValue && isArrayItem) {
            continue;
        }*/

        while (nDiffIntent < 0) {
            st.pCurItem = st.pCurItem->getParent();
            st.nIntent = st.nIntent - 2;
            nDiffIntent = nLineIntent - st.nIntent;
            if (st.pCurItem == nullptr) {
                WsjcppLog::throw_err(TAG, "cur item is nullptr");
            }
        }

        if (nDiffIntent == 0) {
            if (st.line.isEmptyName()) {
                if ( ! isEmptyValue && isArrayItem) {
                    process_sameIntent_emptyName_hasValue_arrayItem(st);
                } else if (! isEmptyValue && ! isArrayItem) {
                    process_sameIntent_emptyName_hasValue_noArrayItem(st);
                } else if (isEmptyValue && isArrayItem) {
                    process_sameIntent_emptyName_emptyValue_arrayItem(st);
                } else if (isEmptyValue && ! isArrayItem) {
                    process_sameIntent_emptyName_emptyValue_noArrayItem(st);
                } else {
                    st.logUnknownLine(TAG);
                }
            } else if ( ! st.line.isEmptyName()) {
                if ( ! isEmptyValue && isArrayItem) {
                    process_sameIntent_hasName_hasValue_arrayItem(st);
                } else if ( ! isEmptyValue && ! isArrayItem) {
                    process_sameIntent_hasName_hasValue_noArrayItem(st);
                } else if (isEmptyValue && isArrayItem) {
                    process_sameIntent_hasName_emptyValue_arrayItem(st);
                } else if (isEmptyValue && ! isArrayItem) {
                    process_sameIntent_hasName_emptyValue_noArrayItem(st);
                } else {
                    st.logUnknownLine(TAG);
                }
            } else {
                st.logUnknownLine(TAG);
            }
        } else {
            st.logUnknownLine(TAG);
        }
    }
    return true;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_hasName_emptyValue_arrayItem(WsjcppYamlParserStatus &st) {
    st.logUnknownLine("process_sameIntent_hasName_emptyValue_arrayItem");
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_hasName_emptyValue_noArrayItem(WsjcppYamlParserStatus &st) {
    WsjcppYamlItem *pItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile, 
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_UNDEFINED
    );
    if (st.line.hasValueDoubleQuotes()) {
        pItem->doValue();
        pItem->setValue(st.line.getValue(), st.line.hasValueDoubleQuotes());
    }
    pItem->setName(st.line.getName(), st.line.hasNameDoubleQuotes());
    pItem->setComment(st.line.getComment());
    st.pCurItem->setElement(st.line.getName(), pItem);
    st.pCurItem = pItem;
    st.nIntent = st.nIntent + 2;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_hasName_hasValue_arrayItem(WsjcppYamlParserStatus &st) {
    if (st.pCurItem->isUndefined()) {
        st.pCurItem->doArray();
    }
    WsjcppYamlItem *pMapItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile, 
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_MAP
    );
    st.pCurItem->appendElement(pMapItem);
    st.pCurItem = pMapItem;
    st.nIntent = st.nIntent + 2;

    WsjcppYamlItem *pItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile, 
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_VALUE
    );
    pItem->setComment(st.line.getComment());
    pItem->setValue(st.line.getValue(), st.line.hasValueDoubleQuotes());
    pItem->setName(st.line.getName(), st.line.hasNameDoubleQuotes());
    pMapItem->setElement(st.line.getName(), pItem);
    st.pCurItem = pItem;
    st.nIntent = st.nIntent + 2;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_hasName_hasValue_noArrayItem(WsjcppYamlParserStatus &st) {
    WsjcppYamlItem *pItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile, 
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_VALUE
    );
    pItem->setComment(st.line.getComment());
    pItem->setValue(st.line.getValue(), st.line.hasValueDoubleQuotes());
    pItem->setName(st.line.getName(), st.line.hasNameDoubleQuotes());
    st.pCurItem->setElement(st.line.getName(), pItem);
    st.pCurItem = pItem;
    st.nIntent = st.nIntent + 2;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_emptyName_hasValue_arrayItem(WsjcppYamlParserStatus &st) {
    if (st.pCurItem->isUndefined()) {
        st.pCurItem->doArray();
    }
    WsjcppYamlItem *pItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile,
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_VALUE
    );
    pItem->setComment(st.line.getComment());
    pItem->setValue(st.line.getValue(), st.line.hasValueDoubleQuotes());
    st.pCurItem->appendElement(pItem);
    st.pCurItem = pItem;
    st.nIntent = st.nIntent + 2;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_emptyName_hasValue_noArrayItem(WsjcppYamlParserStatus &st) {
    st.logUnknownLine("TODO process_sameIntent_emptyName_hasValue_noArrayItem");
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_emptyName_emptyValue_arrayItem(WsjcppYamlParserStatus &st) {
    if (st.pCurItem->isUndefined()) {
        st.pCurItem->doArray();
    }
    WsjcppYamlItem *pItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile, 
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_VALUE
    );
    pItem->setComment(st.line.getComment());
    pItem->setValue(st.line.getValue(), st.line.hasValueDoubleQuotes());
    st.pCurItem->appendElement(pItem);
    st.pCurItem = pItem;
    st.nIntent = st.nIntent + 2;
}

// ---------------------------------------------------------------------

void WsjcppYaml::process_sameIntent_emptyName_emptyValue_noArrayItem(WsjcppYamlParserStatus &st) {
    WsjcppYamlItem *pItem = new WsjcppYamlItem(
        st.pCurItem, st.placeInFile,
        WsjcppYamlItemType::WSJCPP_YAML_ITEM_EMPTY
    );
    pItem->setComment(st.line.getComment());
    st.pCurItem->appendElement(pItem);
}

// ---------------------------------------------------------------------

