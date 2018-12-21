#include "light_http_server.h"
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
// LightHttpServer

LightHttpServer::LightHttpServer() {
	TAG = "LightHttpServer";
	m_nMaxWorkers = 4;
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

	for (int i = 0; i < m_nMaxWorkers; i++) {
		m_vWorkers.push_back(new LightHttpThreadWorker("worker" + std::to_string(i), this));
	}

	for (int i = 0; i < m_vWorkers.size(); i++) {
		m_vWorkers[i]->start();
	}

	std::string str;
	while(1) { // or problem can be here
		struct sockaddr_in clientAddress;
		socklen_t sosize  = sizeof(clientAddress);
		int nSockFd = accept(m_nSockFd,(struct sockaddr*)&clientAddress,&sosize);
		std::string sAddress = inet_ntoa(clientAddress.sin_addr);
		LightHttpRequest *pInfo = new LightHttpRequest(nSockFd, sAddress, m_sWebFolder, m_pHandler);
		// info will be removed inside a thread
		m_dequeRequests.push_front(pInfo);

		// pthread_create(&m_serverThread, NULL, &newRequest, (void *)pInfo);
		// std::cout << "wait \n";
		usleep(100);
	}
}

// ----------------------------------------------------------------------

LightHttpRequest *LightHttpServer::popRequest() {
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	LightHttpRequest *pRequest = nullptr;
	int nSize = m_dequeRequests.size();
	if (nSize > 0) {
		pRequest = m_dequeRequests.back();
		m_dequeRequests.pop_back();
	}
	return pRequest;
}

// ----------------------------------------------------------------------

void LightHttpServer::stop() {
	std::lock_guard<std::mutex> guard(this->m_mtxDequeRequests);
	while (m_dequeRequests.size() > 0) {
		delete m_dequeRequests.back();
		m_dequeRequests.pop_back();
	}
	for (int i = 0; i < m_vWorkers.size(); i++) {
		m_vWorkers[i]->stop();
	}
	close(m_nSockFd);
	// close(newsockfd);
} 
