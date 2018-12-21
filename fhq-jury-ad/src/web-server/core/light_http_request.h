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
#include <ilighthttprequest.h>
#include <ilighthttphandler.h>

// example of request
class HandlerRequestHelloWorld : ILightHttpHandler {
	public:
		virtual bool handle(ILightHttpRequest *pRequest);
};

class LightHttpRequest : ILightHttpRequest {
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
			const std::string &sAddress,
			const std::string &sWebFolder,
			ILightHttpHandler *pHandler
		);
		~LightHttpRequest(){};
		bool handle(const std::string &sWorkerId, const std::string &sRequest);
		int sockFd();

		// ILightHttpRequest
		virtual std::string address();
		virtual std::string requestType();
		virtual std::string requestPath();
		virtual std::string requestHttpVersion();
		virtual void response(const std::string &sFirst);
		virtual void response(const std::string &sFirst, const std::string &sDataType, const std::string &sBody);
		virtual std::map<std::string,std::string> &requestQueryParams();

	private:
		std::string TAG;

		// virtual ~LightHttpRequest();
		void parseRequest(const std::string &sRequest);
		std::string currentTime();

		void responseFile(const std::string &sFilePath);

		int m_nSockFd;
		bool m_bClosed; 
		ILightHttpHandler *m_pHandler;
		std::string m_sAddress;
		std::string m_sRequestType;
		std::string m_sRequestPath;
		std::map<std::string,std::string> m_sRequestQueryParams;
		std::string m_sRequestHttpVersion;
		std::string m_sWebFolder;
		
};

#endif // LIGHT_HTTP_REQUEST_H