#include <resources_manager.h>
#include <fs.h>

// ---------------------------------------------------------------------

ResourceFile::ResourceFile() {
    ResourcesManager::add(this);
}

// ---------------------------------------------------------------------

std::vector<ResourceFile*> *g_pListResourceFiles = nullptr;

// ---------------------------------------------------------------------

void ResourcesManager::initGlobalVariables() {
    if (g_pListResourceFiles == nullptr) {
        g_pListResourceFiles = new std::vector<ResourceFile*>();
    }
}

// ---------------------------------------------------------------------

void ResourcesManager::add(ResourceFile* pStorage) {
    ResourcesManager::initGlobalVariables();
    g_pListResourceFiles->push_back(pStorage);
}

// ---------------------------------------------------------------------

bool ResourcesManager::has(const std::string &sFilename) {
    ResourcesManager::initGlobalVariables();
    for (int i = 0; i < ResourcesManager::list().size(); i++) {
        if (ResourcesManager::list()[i]->filename() == sFilename) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------

ResourceFile* ResourcesManager::get(const std::string &sFilename) {
    ResourcesManager::initGlobalVariables();
    for (int i = 0; i < ResourcesManager::list().size(); i++) {
        if (ResourcesManager::list()[i]->filename() == sFilename) {
            return ResourcesManager::list()[i];
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------

const std::vector<ResourceFile*> &ResourcesManager::list() {
    return *g_pListResourceFiles;
}

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

bool ResourcesManager::extractFiles(const std::string &sWorkspace) {
    // TODO mkdir -p for files
    const std::vector<ResourceFile*> list = ResourcesManager::list();
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
