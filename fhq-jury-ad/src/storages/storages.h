#ifndef STORAGES_H
#define STORAGES_H

#include <map>
#include <string>
#include <vector>
#include <storage.h>
#include <utils_logger.h>

// ---------------------------------------------------------------------

class IFabricStorage {
    public:
        virtual Storage *create(int nGameStartUTCInSec, int nGameEndUTCInSec) = 0;
};

// ---------------------------------------------------------------------

extern std::map<std::string, IFabricStorage*> *g_pFabricStorages;

class Storages {
    public:
        static void initGlobalVariables();
        static void add(const std::string &sType, IFabricStorage* pStorage);
        static std::vector<std::string> list();
        static bool support(const std::string &sType);
        static Storage* create(const std::string &sType, int nGameStartUTCInSec, int nGameEndUTCInSec);
};

// ---------------------------------------------------------------------

template<class T>
class FabricStorage : IFabricStorage {
    public:
        FabricStorage() {
            Storages::add(T::type(), this);
        };
        virtual Storage *create(int nGameStartUTCInSec, int nGameEndUTCInSec) {
            return new T(nGameStartUTCInSec, nGameEndUTCInSec);
        };
};

// ---------------------------------------------------------------------
// RegistryStorage
#define REGISTRY_STORAGE( classname ) \
    static FabricStorage<classname> * pRegistryFabric ## classname = new FabricStorage<classname>(); \

// ---------------------------------------------------------------------


#endif // STORAGES_H