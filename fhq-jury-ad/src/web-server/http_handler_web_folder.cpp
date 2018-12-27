#include "http_handler_web_folder.h"
#include <utils_logger.h>
#include <fs.h>

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
    if (!FS::fileExists(sFilePath)) { // TODO check the file exists not dir
        return false;
    }

    return true;
}

// ----------------------------------------------------------------------

bool HttpHandlerWebFolder::handle(const std::string &sWorkerId, LightHttpRequest *pRequest){
    std::string _tag = TAG + "-" + sWorkerId;
    // Log::warn(_tag, pRequest->requestPath());

    /*std::string sIndexHtml = m_sWebFolder + "/index.html";
    if (pRequest->requestPath() == "/" && FS::fileExists(sIndexHtml)) {
        pRequest->responseFile(sIndexHtml);
        return true;
    }*/

    std::string sFilePath = m_sWebFolder + pRequest->requestPath(); // TODO check /../ in path
    if (!FS::fileExists(sFilePath)) { // TODO check the file exists not dir
        return false;
    }
    // Log::warn(TAG, "Rersponse File " + sFilePath);
    pRequest->responseFile(sFilePath);
    return true;
}