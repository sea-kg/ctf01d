#include "light_http_request.h"
#include <utils_logger.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>
#include <regex>        // regex, sregex_token_iterator
#include <stdio.h>
#include <math.h>
#include <fs.h>

bool HandlerRequestHelloWorld::handle(ILightHttpRequest *pRequest){
	if(pRequest->requestPath() == "/"){
		pRequest->response(
            LightHttpRequest::RESP_OK, 
            "text/html", 
            ""
			"<html><body>"
			"<h1>Hello, World! So...</h1>"
			"</body>"
			"</html>");
		return true;
	}
	return false;
}


// ----------------------------------------------------------------------
// LightHttpRequest

// enum for http responses
std::string LightHttpRequest::RESP_OK = "HTTP/1.1 200 OK";
std::string LightHttpRequest::RESP_BAD_REQUEST = "HTTP/1.1 400 Bad Request";
std::string LightHttpRequest::RESP_FORBIDDEN = "HTTP/1.1 403 Forbidden";
std::string LightHttpRequest::RESP_NOT_FOUND = "HTTP/1.1 404 Not Found";
std::string LightHttpRequest::RESP_PAYLOAD_TOO_LARGE = "HTTP/1.1 413 Payload Too Large";
std::string LightHttpRequest::RESP_INTERNAL_SERVER_ERROR = "HTTP/1.1 500 Internal Server Error";
std::string LightHttpRequest::RESP_NOT_IMPLEMENTED = "HTTP/1.1 501 Not Implemented";

LightHttpRequest::LightHttpRequest(int nSockFd, const std::string &sAddress, const std::string &sWebFolder, ILightHttpHandler *pHandler) {
	m_nSockFd = nSockFd;
	m_sAddress = sAddress;
	m_pHandler = pHandler;
	m_sWebFolder = sWebFolder;
	m_bClosed = false;
	TAG = "LightHttpRequest";
}

// ----------------------------------------------------------------------

int LightHttpRequest::sockFd(){
	return m_nSockFd;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestType(){
	return m_sRequestType;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestPath(){
	return m_sRequestPath;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::requestHttpVersion(){
	return m_sRequestHttpVersion;
}

std::map<std::string,std::string> &LightHttpRequest::requestQueryParams() {
	return m_sRequestQueryParams;
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::address(){
	return m_sAddress;
}

// ----------------------------------------------------------------------

void LightHttpRequest::parseRequest(const std::string &sRequest){
    std::istringstream f(sRequest);
	std::string firstLine = "";
    if (getline(f, firstLine, '\n')) {
        // nothing
    }

	if (firstLine.size() > 0) {
		std::istringstream f(firstLine);
		std::vector<std::string> params;
		std::string s;
		while (getline(f, s, ' ')) {
			params.push_back(s);
		}
		if(params.size() > 0){
			m_sRequestType = params[0];
		}

		if(params.size() > 1){
			m_sRequestPath = params[1];
		}

		// TODO m_sRequestPath - need split by ? if exists
		if(params.size() > 2){
			m_sRequestHttpVersion = params[2];
		}
	}

	// parse query
	std::size_t nFound = m_sRequestPath.find("?");
  	if (nFound != std::string::npos) {
		std::string sQuery = m_sRequestPath.substr(nFound+1);
		m_sRequestPath = m_sRequestPath.substr(0, nFound);
		std::istringstream f(sQuery);
		std::string sParam;
		while (getline(f, sParam, '&')) {
			std::size_t nFound2 = sParam.find("=");
			std::string sValue = sParam.substr(nFound2+1);
			std::string sName = sParam.substr(0, nFound2);
			m_sRequestQueryParams[sName] = sValue;
		}
	}
}

// ----------------------------------------------------------------------

bool LightHttpRequest::handle(const std::string &sWorkerId, const std::string &sRequest){
	TAG = "LightHttpRequest-" + sWorkerId;
	this->parseRequest(sRequest);
	if(this->requestType() != "GET"){
		this->response(LightHttpRequest::RESP_NOT_IMPLEMENTED);
		return false;
	}

	if(m_pHandler != NULL){
		if(m_pHandler->handle(this)){
			return true;
		}
		std::string sFilePath = m_sWebFolder + this->requestPath(); // TODO check /../ in path
		if (FS::fileExists(sFilePath)){ // TODO check the file exists not dir
			// std::cout << "File exists! " << sFilePath << "\n";
			this->responseFile(sFilePath);
			return true;
		} else {
			Log::err(TAG, "File not exists! " + sFilePath);
		}
		
		this->response(LightHttpRequest::RESP_NOT_FOUND);
		return false;
	}
	this->response(LightHttpRequest::RESP_INTERNAL_SERVER_ERROR);
	return false;
}

// ----------------------------------------------------------------------

void LightHttpRequest::response(const std::string &sFirst){
	this->response(sFirst, "text/html", "");
}

// ----------------------------------------------------------------------

std::string LightHttpRequest::currentTime(){
    // milleseconds
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }

    // datetime
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X.", &tstruct);
    return std::string(buf) + std::to_string(millisec);
}

// ----------------------------------------------------------------------

void LightHttpRequest::response(const std::string &sFirst, const std::string &sDataType, const std::string &sBody){
	std::string sResponse = sFirst + "\r\n"
		"Date: Tue, 14 Aug 2018 08:10:42 GMT\r\n" // TODO generate data
		"Server: fhq-jury-ad\r\n" // TODO generate data
		"Cache-Control: no-cache, no-store, must-revalidate\r\n"
		"Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n" // TODO generate data
		"Content-Length: " + std::to_string(sBody.length()) + "\r\n" // TODO generate data
		"Content-Type: " + sDataType + "\r\n"
		"Connection: Closed\r\n"
		"\r\n" + sBody;
	
	if(m_bClosed) {
		Log::warn(TAG, "Already sended response");
		return;
	}
	m_bClosed = true;

	send(m_nSockFd, sResponse.c_str(), sResponse.length(),0);
	close(m_nSockFd);
}

// ----------------------------------------------------------------------

void LightHttpRequest::responseFile(const std::string &sFilePath){
	
	// TODO cache: check file in cache 
	std::string sFileExt = sFilePath.substr(sFilePath.find_last_of(".") + 1);

	std::string sType = "application/octet-stream";
	if (sFileExt == "json"){
		sType = "application/json";
	} else if (sFileExt == "css") {
		sType = "text/css"; 
	} else if (sFileExt == "js") {
		sType = "text/javascript";
	} else if (sFileExt == "html") {
		sType = "text/html";
	} else if (sFileExt == "gif") {
		sType = "image/gif";
	} else if (sFileExt == "ico") {
		sType = "image/x-icon";
	} else if (sFileExt == "xml") {
		sType = "application/xml";
	} else if (sFileExt == "png") {
		sType = "application/xml";
	} else if (sFileExt == "jpg" || sFileExt == "jpeg") {
		sType = "image/jpeg";
	} else if (sFileExt == "svg") {
		sType = "image/svg+xml";
	}

	// read data from file
	std::ifstream f(sFilePath, std::ios::binary | std::ios::ate);
	std::streamsize nSize = f.tellg();
	f.seekg(0, std::ios::beg);
	char *pData = new char[nSize];
	// std::vector<char> buffer(size);
	if (nSize > 10*1024*1024){
		this->response(LightHttpRequest::RESP_PAYLOAD_TOO_LARGE);
		delete[] pData;
		return;
	}

	if (!f.read(pData, nSize)) {
		this->response(LightHttpRequest::RESP_NOT_FOUND);
		delete[] pData;
		return;
		// std::cout << sFilePath << "\n filesize: " << nSize << " bytes\n";
	}

	std::string sResponse = LightHttpRequest::RESP_OK + "\r\n"
		"Date: Tue, 14 Aug 2018 08:10:42 GMT\r\n" // TODO generate data
		"Server: fhq-jury-ad\r\n" // TODO generate data
		"Cache-Control: no-cache, no-store, must-revalidate\r\n"
		"Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n" // TODO generate data
		"Content-Length: " + std::to_string(nSize) + "\r\n" // TODO generate data
		"Content-Type: " + sType + "\r\n" // TODO generate data
		"Connection: Closed\r\n" // TODO generate data
		"\r\n";

	if(m_bClosed) {
		Log::warn(TAG, "Already sended response");
		delete[] pData;
		return;
	}
	m_bClosed = true;
	#if __APPLE__
		send(m_nSockFd, sResponse.c_str(), sResponse.length(),SO_NOSIGPIPE);
		send(m_nSockFd, pData, nSize, SO_NOSIGPIPE);
	// #if
	// TARGET_OS_MAC 

	#else
		send(m_nSockFd, sResponse.c_str(), sResponse.length(),MSG_CONFIRM);
		send(m_nSockFd, pData, nSize, MSG_CONFIRM);
	#endif
	
	close(m_nSockFd);
	delete[] pData;
}
