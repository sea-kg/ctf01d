#ifndef WSJCPP_RESOURCES_MANAGER_H
#define WSJCPP_RESOURCES_MANAGER_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

class WsjcppResourceFile {
    public:
        WsjcppResourceFile();
        virtual const std::string &getFilename() const = 0;
        virtual const std::string &getPackAs() const = 0;
        virtual int getBufferSize() const = 0;
        virtual const char *getBuffer() const = 0;
};

extern std::vector<WsjcppResourceFile*> *g_pWsjcppResourceFiles;

class WsjcppResourcesManager {
    public:
        static void initGlobalVariables();
        static void add(WsjcppResourceFile*);
        static const std::vector<WsjcppResourceFile*> &list();
        static bool has(const std::string &sFilename);
        static WsjcppResourceFile* get(const std::string &sFilename);
        static bool make(const std::string &sWorkspace);
        // static bool createFolders(const std::string &sWorkspace);
        // static bool extractFiles(const std::string &sWorkspace);
};

// ---------------------------------------------------------------------
// Registry WsjcppResourceFile
#define REGISTRY_WSJCPP_RESOURCE_FILE( classname ) \
    static classname * pRegistryWsjcppResourceFile ## classname = new classname(); \

#endif // WSJCPP_RESOURCES_MANAGER_H