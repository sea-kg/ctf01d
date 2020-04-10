#include "storages.h"
#include <wsjcpp_core.h>
#include <algorithm>

// ---------------------------------------------------------------------

std::map<std::string, IFabricStorage*> *g_pFabricStorages = NULL;

// ---------------------------------------------------------------------

void Storages::initGlobalVariables(){
    if(g_pFabricStorages == NULL){
        // Log::info(std::string(), "Create employees map");
        g_pFabricStorages = new std::map<std::string, IFabricStorage*>();
    }
}

// ---------------------------------------------------------------------

void Storages::add(const std::string &sType, IFabricStorage* pStorage){
    Storages::initGlobalVariables();
    const std::string TAG = "addStorage";
    if (g_pFabricStorages->count(sType) > 0) {
        WsjcppLog::err(TAG, sType + " - storage already registered");
    } else {
        g_pFabricStorages->insert(std::pair<std::string, IFabricStorage*>(sType,pStorage));
        // Log::info(sType, "Registered");
    }
}

// ---------------------------------------------------------------------

std::vector<std::string> Storages::list() {
    Storages::initGlobalVariables();
    std::vector<std::string> vResult;
    std::map<std::string, IFabricStorage*>::iterator it;
    for (it = g_pFabricStorages->begin(); it != g_pFabricStorages->end(); it++) {
        vResult.push_back(it->first);
    }
    return vResult;
}

// ---------------------------------------------------------------------

bool Storages::support(const std::string &sType) {
    Storages::initGlobalVariables();
    return g_pFabricStorages->count(sType) != 0;
}

// ---------------------------------------------------------------------

Storage* Storages::create(const std::string &sType, int nGameStartUTCInSec, int nGameEndUTCInSec) {
    Storages::initGlobalVariables();
    std::string TAG = "findStorage";
    IFabricStorage *pFabricStorage = nullptr;
    if(g_pFabricStorages->count(sType)){
        pFabricStorage = g_pFabricStorages->at(sType);
        return pFabricStorage->create(nGameStartUTCInSec, nGameEndUTCInSec);
    }
    return nullptr;
}

// ---------------------------------------------------------------------

