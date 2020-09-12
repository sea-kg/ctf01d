#include "wsjcpp_resources_manager.h"

// ---------------------------------------------------------------------

WsjcppResourceFile::WsjcppResourceFile() {
    WsjcppResourcesManager::add(this);
}

// ---------------------------------------------------------------------

std::vector<WsjcppResourceFile*> *g_pWsjcppResourceFiles = nullptr;

// ---------------------------------------------------------------------

void WsjcppResourcesManager::initGlobalVariables() {
    if (g_pWsjcppResourceFiles == nullptr) {
        g_pWsjcppResourceFiles = new std::vector<WsjcppResourceFile*>();
    }
}

// ---------------------------------------------------------------------

void WsjcppResourcesManager::add(WsjcppResourceFile* pStorage) {
    WsjcppResourcesManager::initGlobalVariables();
    g_pWsjcppResourceFiles->push_back(pStorage);
}

// ---------------------------------------------------------------------

bool WsjcppResourcesManager::has(const std::string &sFilename) {
    WsjcppResourcesManager::initGlobalVariables();
    for (int i = 0; i < WsjcppResourcesManager::list().size(); i++) {
        if (WsjcppResourcesManager::list()[i]->getFilename() == sFilename) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

WsjcppResourceFile* WsjcppResourcesManager::get(const std::string &sFilename) {
    WsjcppResourcesManager::initGlobalVariables();
    for (int i = 0; i < WsjcppResourcesManager::list().size(); i++) {
        if (WsjcppResourcesManager::list()[i]->getFilename() == sFilename) {
            return WsjcppResourcesManager::list()[i];
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------

const std::vector<WsjcppResourceFile*> &WsjcppResourcesManager::list() {
    return *g_pWsjcppResourceFiles;
}

// ---------------------------------------------------------------------

/*
bool WsjcppResourcesManager::make(const std::string &sWorkspace) {
    if (!WsjcppResourcesManager::createFolders(sWorkspace)) {
        return false;
    }
    return WsjcppResourcesManager::extractFiles(sWorkspace);
}

// ---------------------------------------------------------------------

bool WsjcppResourcesManager::createFolders(const std::string &sWorkspace) {
    // prepare folders
    std::vector<std::string> vCreateDirs;
    vCreateDirs.push_back(sWorkspace + "/logs");
    vCreateDirs.push_back(sWorkspace + "/teams");
    vCreateDirs.push_back(sWorkspace + "/checkers");
    vCreateDirs.push_back(sWorkspace + "/html");
    vCreateDirs.push_back(sWorkspace + "/html/css");
    vCreateDirs.push_back(sWorkspace + "/html/js");
    vCreateDirs.push_back(sWorkspace + "/html/images");
    vCreateDirs.push_back(sWorkspace + "/html/images/teams");
    vCreateDirs.push_back(sWorkspace + "/html/images/states");

    for(int i = 0; i < vCreateDirs.size(); i++) {
        std::string sPath = vCreateDirs[i];
        // check dir existing
        if (!FS::dirExists(sPath)) {
            // try make dir
            if (!FS::makeDir(sPath)) {
                std::cout << "Could not create folder " << sPath << std::endl;
                return false;
            } else {
                std::cout << "Created folder " << sPath << std::endl;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------

bool WsjcppResourcesManager::extractFiles(const std::string &sWorkspace) {
    // TODO mkdir -p for files
    const std::vector<WsjcppResourceFile*> list = WsjcppResourcesManager::list();
    for(int i = 0; i < list.size(); i++) {
        std::string sFilename = sWorkspace + "/" + list[i]->filename();
        if (!FS::fileExists(sFilename)) {
            if (!FS::writeFile(sFilename, list[i]->buffer(), list[i]->bufferSize())) {
                std::cout << "Could not write file " << sFilename << std::endl;
                return false;
            } else {
                std::cout << "Created file " << sFilename << std::endl;
            }
        }
    }
    return true;
}
*/