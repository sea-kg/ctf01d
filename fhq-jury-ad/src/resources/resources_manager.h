#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

class ResourceFile {
    public:
        ResourceFile();
        virtual const std::string &filename() = 0;
        virtual const int bufferSize() = 0;
        virtual const char *buffer() = 0;
};

extern std::vector<ResourceFile*> *g_pListResourceFiles;

class ResourcesManager {
    public:
        static void initGlobalVariables();
        static void add(ResourceFile*);
        static const std::vector<ResourceFile*> &list();
        static bool has(const std::string &sFilename);
        static ResourceFile* get(const std::string &sFilename);
        static bool make(const std::string &sWorkspace);
        static bool createFolders(const std::string &sWorkspace);
        static bool extractFiles(const std::string &sWorkspace);
};

// ---------------------------------------------------------------------

// RegistryStorage
#define REGISTRY_RESOURCE_FILE( classname ) \
    static classname * pRegistryResourceFile ## classname = new classname(); \

#endif // RESOURCES_MANAGER_H