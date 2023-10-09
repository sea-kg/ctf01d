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

#include "http_handler_web_folder.h"
#include <wsjcpp_light_web_server.h>
#include <wsjcpp_core.h>

// ----------------------------------------------------------------------

HttpHandlerWebFolder::HttpHandlerWebFolder(const std::string &sWebFolder)
    : WsjcppLightWebHttpHandlerBase("web-folder") {

    TAG = "HttpHandlerWebFolder";
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebFolder::canHandle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // Log::warn(_tag, "canHandle: " + pRequest->requestPath());
    std::string sRequestPath = pRequest->getRequestPath();
    
    if (sRequestPath == "/") {
        sRequestPath = "/index.html";
    }

    if (sRequestPath == "") {
        WsjcppLog::err(_tag, "Request path is empty");
        return false;
    }

    if (!WsjcppCore::dirExists(m_sWebFolder)) {
        WsjcppLog::warn(_tag, "Directory " + m_sWebFolder + " does not exists");
    }

    std::string sFilePath = m_sWebFolder + sRequestPath; // TODO check /../ in path
    // Log::warn(_tag, "Response Resources " + sFilePath);
    if (!WsjcppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
        // check in resources
        std::string sResPath = "html" + sRequestPath;
        // Log::warn(_tag, "Response Resources " + sResPath);
        return WsjcppResourcesManager::has(sResPath);
    }

    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebFolder::handle(const std::string &sWorkerId, WsjcppLightWebHttpRequest *pRequest){
    std::string _tag = TAG + "-" + sWorkerId;
    std::string sRequestPath = pRequest->getRequestPath();
    sRequestPath = WsjcppCore::doNormalizePath(sRequestPath);

    WsjcppLightWebHttpResponse response(pRequest->getSockFd());

    // Log::warn(_tag, pRequest->requestPath());
    
    if (sRequestPath == "/") {
        sRequestPath = "/index.html";
    }
    
    std::string sFilePath = m_sWebFolder + sRequestPath; 

    if (!WsjcppCore::fileExists(sFilePath)) {
        std::string sResPath = "html" + sRequestPath;
        if (WsjcppResourcesManager::has(sResPath)) {
            // Log::warn(_tag, "Response Resources " + sResPath);
            WsjcppResourceFile *pFile = WsjcppResourcesManager::get(sResPath);
            response.cacheSec(60).ok().sendBuffer(sResPath, pFile->getBuffer(), pFile->getBufferSize());
            return true;
        }
        return false;
    }
    
    // Log::warn(_tag, "Response File " + sFilePath);
    response.cacheSec(60).ok().sendFile(sFilePath);
    return true;
}
