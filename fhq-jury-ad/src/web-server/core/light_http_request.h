#ifndef LIGHT_HTTP_REQUEST_H
#define LIGHT_HTTP_REQUEST_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <map>

class LightHttpRequest {
	public:
        // enum for http responses
        static std::string RESP_OK;
        static std::string RESP_BAD_REQUEST;
		static std::string RESP_FORBIDDEN;
        static std::string RESP_NOT_FOUND;
        static std::string RESP_PAYLOAD_TOO_LARGE;
		static std::string RESP_INTERNAL_SERVER_ERROR;
		static std::string RESP_NOT_IMPLEMENTED;

		LightHttpRequest(
			int nSockFd,
			const std::string &sAddress
		);
		~LightHttpRequest(){};

		int sockFd();
		void parseRequest(const std::string &sRequest);

		
		std::string address();
		std::string requestType();
		std::string requestPath();
		std::string requestHttpVersion();
		void setResponseNoCache();
		void setResponseCacheSec(int nCacheSec);
		void response(const std::string &sFirst);
		void response(const std::string &sFirst, const std::string &sDataType, const std::string &sBody);
		void responseFile(const std::string &sFilePath);
		void responseBuffer(const std::string &sFilePath, const char *pBuffer, const int nBufferSize);
		std::map<std::string,std::string> &requestQueryParams();

	private:
		std::string TAG;

		int m_nSockFd;
		bool m_bClosed; 
		std::string m_sAddress;
		std::string m_sRequestType;
		std::string m_sRequestPath;
		std::map<std::string,std::string> m_sRequestQueryParams;
		std::string m_sRequestHttpVersion;

		std::string m_sResponseCacheControl;
		std::string m_sLastModified;
};

#endif // LIGHT_HTTP_REQUEST_H