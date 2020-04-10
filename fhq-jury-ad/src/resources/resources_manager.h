#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <string>

class ResourcesManager {
    public:
        static bool make(const std::string &sWorkspace);
        static bool createFolders(const std::string &sWorkspace);
        static bool extractFiles(const std::string &sWorkspace);
};

// ---------------------------------------------------------------------

#endif // RESOURCES_MANAGER_H