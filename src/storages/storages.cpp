/**********************************************************************************
 *          Project
 *  _______ _________ _______  _______  __    ______
 * (  ____ \\__   __/(  ____ \(  __   )/  \  (  __  \
 * | (    \/   ) (   | (    \/| (  )  |\/) ) | (  \  )
 * | |         | |   | (__    | | /   |  | | | |   ) |
 * | |         | |   |  __)   | (/ /) |  | | | |   | |
 * | |         | |   | (      |   / | |  | | | |   ) |
 * | (____/\   | |   | )      |  (__) |__) (_| (__/  )
 * (_______/   )_(   |/       (_______)\____/(______/
 *
 * MIT License
 * Copyright (c) 2018-2023 Evgenii Sopov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***********************************************************************************/

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

