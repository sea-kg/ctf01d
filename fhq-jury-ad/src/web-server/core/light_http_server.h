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
#include <deque>
#include <mutex>
#include <light_http_request.h>
#include <ilighthttpdequerequests.h>
#include <light_http_thread_worker.h>

class LightHttpServer : public ILightHttpDequeRequests {
	public:

		LightHttpServer();
		void start(int nPort, const std::string &sWebFolder, ILightHttpHandler *pHandler);
		void stop();
		
		// ILightHttpDequeRequests
		virtual LightHttpRequest *popRequest();

	private:
		std::string TAG;
		std::mutex m_mtxDequeRequests;
		std::deque<LightHttpRequest *> m_dequeRequests;
		int m_nMaxWorkers;
		std::vector<LightHttpThreadWorker *> m_vWorkers;

		int m_nSockFd;
		ILightHttpHandler *m_pHandler;
		std::string m_sWebFolder;
		struct sockaddr_in m_serverAddress;	
};

#endif // LIGHT_HTTP_SERVER_H
