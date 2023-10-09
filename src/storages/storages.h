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

#ifndef STORAGES_H
#define STORAGES_H

#include <map>
#include <string>
#include <vector>
#include <storage.h>
#include <wsjcpp_core.h>

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