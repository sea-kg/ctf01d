#include "resources_manager.h"
#include <wsjcpp_core.h>
#include <wsjcpp_resources_manager.h>

// ---------------------------------------------------------------------

bool ResourcesManager::make(const std::string &sWorkspace) {
    if (!ResourcesManager::createFolders(sWorkspace)) {
        return false;
    }
    return ResourcesManager::extractFiles(sWorkspace);
}

// ---------------------------------------------------------------------

bool ResourcesManager::createFolders(const std::string &sWorkspace) {
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
        if (!WsjcppCore::dirExists(sPath)) {
            // try make dir
            if (!WsjcppCore::makeDir(sPath)) {
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

bool ResourcesManager::extractFiles(const std::string &sWorkspace) {
    // TODO mkdir -p for files
    std::vector<WsjcppResourceFile*> list = WsjcppResourcesManager::list();
    for(int i = 0; i < list.size(); i++) {
        std::string sFilename = sWorkspace + "/" + list[i]->getFilename();
        if (!WsjcppCore::fileExists(sFilename)) {
            if (!WsjcppCore::writeFile(sFilename, list[i]->getBuffer(), list[i]->getBufferSize())) {
                std::cout << "Could not write file " << sFilename << std::endl;
                return false;
            } else {
                std::cout << "Created file " << sFilename << std::endl;
            }
        }
    }
    return true;
}
