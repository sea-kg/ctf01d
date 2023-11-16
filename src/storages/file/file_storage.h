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

#ifndef FILE_STORAGE_H
#define FILE_STORAGE_H

#include <storages.h>

class FileStorage : public Storage {
    public:
        FileStorage(int nGameStartUTCInSec, int nGameEndUTCInSec);
        static std::string type() { return "file"; };

        // Storage
        virtual bool applyConfigFromYaml(
            WsjcppYaml &yamlConfig,
            std::vector<Ctf01dTeamDef> &vTeamsConf,
            std::vector<Ctf01dServiceDef> &vServicesConf
        );
        virtual void clean();

    private:
        std::string TAG;
        std::mutex m_mutexFlags;

        int m_nGameStartUTCInSec;
        int m_nGameEndUTCInSec;

        std::string m_sDatabasePath;
        std::vector<Flag *> m_vFlagLives;
        std::vector<Flag *> m_vFlagArchive;
        std::map<std::string, Flag *> m_mapFlagLive;
};

#endif // FILE_STORAGE_H