#include "http_handler_web_folder.h"
#include <wsjcpp_resources_manager.h>
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
