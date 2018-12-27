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

extern std::vector<ResourceFile*> *g_pFabricResourceFile;

class ResourceManager {
    public:
        static void initGlobalVariables();
        static void add(ResourceFile*);
        static const std::vector<ResourceFile*> &list();
};

// ---------------------------------------------------------------------

// RegistryStorage
#define REGISTRY_RESOURCE_FILE( classname ) \
    static classname * pRegistryResourceFile ## classname = new classname(); \

#endif // RESOURCES_MANAGER_H