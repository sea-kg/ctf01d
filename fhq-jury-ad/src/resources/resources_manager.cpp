#include <resources_manager.h>

// ---------------------------------------------------------------------

ResourceFile::ResourceFile() {
    ResourceManager::add(this);
}

// ---------------------------------------------------------------------

std::vector<ResourceFile*> *g_pFabricResourceFile = nullptr;

// ---------------------------------------------------------------------

void ResourceManager::initGlobalVariables() {
    if (g_pFabricResourceFile == nullptr) {
        g_pFabricResourceFile = new std::vector<ResourceFile*>();
    }
}

// ---------------------------------------------------------------------

void ResourceManager::add(ResourceFile* pStorage) {
    ResourceManager::initGlobalVariables();
    g_pFabricResourceFile->push_back(pStorage);
}

// ---------------------------------------------------------------------

const std::vector<ResourceFile*> &ResourceManager::list() {
    return *g_pFabricResourceFile;
}

// ---------------------------------------------------------------------