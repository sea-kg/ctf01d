#ifndef LIGHT_HTTP_THREAD_WORKER_H
#define LIGHT_HTTP_THREAD_WORKER_H

#include <ilighthttpdequerequests.h>

class LightHttpThreadWorker {
	public:

		LightHttpThreadWorker(const std::string &sName, ILightHttpDequeRequests *pDeque);

		void start();
		void stop();
		void run();		
	private:
		std::string TAG;
		std::string m_sName;
		ILightHttpDequeRequests *m_pDeque;
		bool m_bStop;
		pthread_t m_serverThread;
};

#endif // LIGHT_HTTP_SERVER_H
