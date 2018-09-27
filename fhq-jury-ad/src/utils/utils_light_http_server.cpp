#include "utils_light_http_server.h"
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


// ----------------------------------------------------------------------
// newRequest

void* newRequest(void *arg) {
	// Log::info("newRequest", "");
	LightHttpRequest *info = (LightHttpRequest *)arg;
	int nSockFd = info->sockFd();
	int n;
	// int newsockfd = (long)arg;
	char msg[MAXPACKETSIZE];
	memset(msg, 0, MAXPACKETSIZE);

	pthread_detach(pthread_self());
	std::string sRequest;
	
	// std::cout << nSockFd  << ": address = " << info->address() << "\n";

	// read data from socket
	while(1) { // problem can be here
		// std::cout << nSockFd  << ": wait recv...\n";
		n = recv(nSockFd, msg, MAXPACKETSIZE, 0);
		// std::cout << "N: " << n << std::endl;
		if (n == -1) {
			std::cout << nSockFd  << ": error read... \n";
			break;
		}
		if (n == 0) {
		   //close(nSockFd);
		   break;
		}
		msg[n]=0;
		//send(newsockfd,msg,n,0);
		sRequest = std::string(msg);
		// stop reading
		int len = sRequest.length();
		// TODO 
		if(len > 4 && ((sRequest[len-1] == '\n' && sRequest[len-2] == '\r' && sRequest[len-3] == '\n' && sRequest[len-4] == '\r')
				|| (sRequest[len-1] == '\n' && sRequest[len-2] == '\n'))
		){
			// std::cout << nSockFd  << ": end of request\n";
			break;
		}
		// usleep(100);
    }
	// std::cout << nSockFd  << ": request >>>> \n" << sRequest << "\n <<<<< request\n";
	info->handle(sRequest);
	delete info;
	return 0;
}

// ----------------------------------------------------------------------
// LightHttpServer

LightHttpServer::LightHttpServer() {
	TAG = "LightHttpServer";
}

// ----------------------------------------------------------------------

void LightHttpServer::start(int nPort, const std::string &sWebFolder, ILightHttpHandler *pHandler) {
	m_pHandler = pHandler;
	m_sWebFolder = sWebFolder;
	m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_nSockFd <= 0){
		Log::err(TAG, "Failed to establish socket connection");
		return;
	}
	int enable = 1;
	if (setsockopt(m_nSockFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		Log::err(TAG, "setsockopt(SO_REUSEADDR) failed");
		return;
	}

 	memset(&m_serverAddress, 0, sizeof(m_serverAddress));
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serverAddress.sin_port = htons(nPort);
	if(bind(m_nSockFd, (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == -1){
		Log::err(TAG, "Error binding to port " + std::to_string(nPort));
		return;
	}
 	listen(m_nSockFd, 5);
	Log::info("LightHttpServer", "Light Http Server started on " + std::to_string(nPort) + " port.");

	// pthread_create(&m_serverThread, NULL, &newRequest, (void *)&m_queueRequests);

	std::string str;
	while(1) { // or problem can be here
		struct sockaddr_in clientAddress;
		socklen_t sosize  = sizeof(clientAddress);
		int nSockFd = accept(m_nSockFd,(struct sockaddr*)&clientAddress,&sosize);
		std::string sAddress = inet_ntoa(clientAddress.sin_addr);
		LightHttpRequest *info = new LightHttpRequest(nSockFd, sAddress, m_sWebFolder, m_pHandler);
		// info will be removed inside a thread
		// m_queueRequests.push(info);

		pthread_create(&m_serverThread, NULL, &newRequest, (void *)info);
		// std::cout << "wait \n";
		usleep(100);
	}
}

void LightHttpServer::stop() {
	close(m_nSockFd);
	// close(newsockfd);
} 
