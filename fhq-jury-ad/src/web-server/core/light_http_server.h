#ifndef LIGHT_HTTP_SERVER_H
#define LIGHT_HTTP_SERVER_H

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
#include <queue>
#include "light_http_request.h"

class LightHttpServer {
	public:

		LightHttpServer();
		void start(int nPort, const std::string &sWebFolder, ILightHttpHandler *pHandler);
		void stop();
		int n, pid;

	private:
		std::string TAG;
		std::queue<LightHttpRequest *> m_queueRequests;

		pthread_t m_serverThread;
		pthread_t m_serverThread1;
		pthread_t m_serverThread2;
		pthread_t m_serverThread3;
		pthread_t m_serverThread4;

		int m_nSockFd;
		ILightHttpHandler *m_pHandler;
		std::string m_sWebFolder;
		struct sockaddr_in m_serverAddress;	
};

#endif // LIGHT_HTTP_SERVER_H
