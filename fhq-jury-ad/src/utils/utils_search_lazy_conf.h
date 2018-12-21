#ifndef SEARCH_LAZY_CONF_H
#define SEARCH_LAZY_CONF_H

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
#include <team.h>

#define MAXPACKETSIZE 4096

class SearchLazyConf {
	public:
		SearchLazyConf(int nJuryPort);
        bool scan();
        std::vector<Team> getFoundTeams();

	private:
        void listCurrentIpAddresses(std::vector<std::string>& vIpAddrs);
        void check(const std::string &sJuryIpAddress, const std::string &sIpAddress);

		std::string TAG;
        int m_nPort;
        int m_nJuryPort;
        int m_nSockFd;
		struct sockaddr_in m_serverAddress;	
        std::vector<Team> m_vTeams;
};

#endif // SEARCH_LAZY_CONF_H
