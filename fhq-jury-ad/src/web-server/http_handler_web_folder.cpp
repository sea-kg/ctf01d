#include "http_handler_web_folder.h"
#include <utils_logger.h>
#include <fs.h>
#include <resources_manager.h>
#include <light_http_response.h>

// ----------------------------------------------------------------------

HttpHandlerWebFolder::HttpHandlerWebFolder(const std::string &sWebFolder)
    : LightHttpHandlerBase("web-folder") {

    TAG = "HttpHandlerWebFolder";
    m_sWebFolder = sWebFolder;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebFolder::canHandle(const std::string &sWorkerId, LightHttpRequest *pRequest) {
    std::string _tag = TAG + "-" + sWorkerId;
    // Log::warn(_tag, "canHandle: " + pRequest->requestPath());

    if (pRequest->requestPath() == "") {
        Log::err(_tag, "Request path is empty");
        return false;
    }

    if (!FS::dirExists(m_sWebFolder)) {
        Log::err(_tag, "Directory " + m_sWebFolder + " does not exists");
        return false;
    }

    // TODO redesing
    if (pRequest->requestPath() == "/") {
        return false;
    }

    /*if (pRequest->requestPath() == "/" && FS::fileExists(m_sWebFolder + "/index.html")) {
        return true;
    }*/

    std::string sFilePath = m_sWebFolder + pRequest->requestPath(); // TODO check /../ in path
    Log::warn(_tag, "Response Resources " + sFilePath);
    if (!FS::fileExists(sFilePath)) { // TODO check the file exists not dir
        // check in resources
        std::string sResPath = "html" + pRequest->requestPath();
        Log::warn(_tag, "Response Resources " + sResPath);
        return ResourcesManager::has(sResPath);
    }

    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebFolder::handle(const std::string &sWorkerId, LightHttpRequest *pRequest){
    std::string _tag = TAG + "-" + sWorkerId;
    Log::warn(_tag, pRequest->requestPath());

    /*std::string sIndexHtml = m_sWebFolder + "/index.html";
    if (pRequest->requestPath() == "/" && FS::fileExists(sIndexHtml)) {
        pRequest->responseFile(sIndexHtml);
        return true;
    }*/
    
    std::string sFilePath = m_sWebFolder + pRequest->requestPath(); // TODO check /../ in path
    if (!FS::fileExists(sFilePath)) {
        std::string sResPath = "html" + pRequest->requestPath();

        if (ResourcesManager::has(sResPath)) {
            Log::warn(_tag, "Response Resources " + sResPath);
            ResourceFile *pFile = ResourcesManager::get(sResPath);
            pRequest->responseBuffer(sResPath, pFile->buffer(), pFile->bufferSize());
            return true;
        }
        return false;
    }

    // TODO 1. if file exists and last date change more that in cache so need update
    // TODO 2. if file not exists but in in resources - response them
    
    Log::warn(_tag, "Response File " + sFilePath);
    pRequest->responseFile(sFilePath);
    return true;
}