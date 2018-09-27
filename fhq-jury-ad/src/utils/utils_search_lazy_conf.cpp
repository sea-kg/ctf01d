#include "utils_search_lazy_conf.h"
#include "utils_logger.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>
#include <fstream>
#include <regex>        // regex, sregex_token_iterator
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 

// ----------------------------------------------------------------------

SearchLazyConf::SearchLazyConf(int nJuryPort) {
	TAG = "SearchLazyConf";
    m_nPort = 17345;
    m_nJuryPort = nJuryPort;
}

// ----------------------------------------------------------------------

bool SearchLazyConf::scan() {
    Log::info(TAG, "Start scanning...");
    m_vTeams.clear();

    std::vector<std::string> vIpLocalAddresses;
    this->listCurrentIpAddresses(vIpLocalAddresses);
    if (vIpLocalAddresses.size() == 0) {
        Log::err(TAG, "Not found local addresses with mask 255.255.255.0");
        return false;
    }
    for (unsigned int iN = 0; iN < vIpLocalAddresses.size(); iN++) {
        std::string sJuryIpAddress = vIpLocalAddresses[iN];
        std::vector<std::string> vIpAddressTmpl;
        std::istringstream f(sJuryIpAddress);
        std::string s = "";
        while (getline(f, s, '.')) {
            vIpAddressTmpl.push_back(s);
        }

        if (vIpAddressTmpl.size() != 4) {
            Log::err(TAG, "Wrong format of jury ip address ");
            return false;
        }
        int nMinA = 1;
        int nMaxA = 255;

        // tests
        // nMinA = 128;
        // nMaxA = 135;

        for (int i = nMinA; i < nMaxA; i++) {
            vIpAddressTmpl[3] = std::to_string(i);

            std::string sIpAddress = ""
                + vIpAddressTmpl[0] + "."
                + vIpAddressTmpl[1] + "."
                + vIpAddressTmpl[2] + "."
                + vIpAddressTmpl[3];

            this->check(sJuryIpAddress, sIpAddress);
        }
    }
    return true;
}

// ----------------------------------------------------------------------

std::vector<ModelTeamConf> SearchLazyConf::getFoundTeams() {
    return m_vTeams;
}

// ----------------------------------------------------------------------

void SearchLazyConf::check(const std::string &sJuryIpAddress, const std::string &sIpAddress) {
    
    Log::info(TAG, "Connecting to " + sIpAddress + "... ");

	m_nSockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(m_nSockFd <= 0){
		Log::err(TAG, "Failed to establish socket connection");
		return;
	}

    fcntl(m_nSockFd, F_SETFL, O_NONBLOCK);

 	memset(&m_serverAddress, 0, sizeof(m_serverAddress));
	m_serverAddress.sin_family = AF_INET;
	m_serverAddress.sin_addr.s_addr = inet_addr( sIpAddress.c_str() );
	m_serverAddress.sin_port = htons(m_nPort);
    
    if (connect(m_nSockFd , (struct sockaddr *)&m_serverAddress , sizeof(m_serverAddress)) < 0) {
        // Log::err(TAG, "FAIL " + sIpAddress + ":" + std::to_string(m_nPort));
        // return false;
    }

    fd_set fdset;
    struct timeval tv;

    FD_ZERO(&fdset);
    FD_SET(m_nSockFd, &fdset);
    tv.tv_sec = 0;
    tv.tv_usec = 150*1000; // 150 ms

    if (select(m_nSockFd + 1, NULL, &fdset, NULL, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(m_nSockFd, SOL_SOCKET, SO_ERROR, &so_error, &len);

        int max_package_size = 4096;

        if (so_error == 0) {
            char msg[max_package_size];
            std::string sRequest = "";
            
            usleep(10000);
            // read data from socket
            int nWait = 0;
            while (nWait < 1000) { // problem can be here
                int n = recv(m_nSockFd, msg, max_package_size, 0);
                if (n == -1) {
                    break;
                }
                if (n == 0) {
                    //close(nSockFd);
                    break;
                }
                msg[n]=0;
                sRequest += std::string(msg);
                usleep(10000);
                nWait++;
            }
            
            nlohmann::json req = nlohmann::json::parse(sRequest);
            Log::ok(TAG, "OK " + sRequest);
            std::string sVersion = req["lazy-conf"];
            if (sVersion != "v1") {
                Log::err(TAG, "Unknown protocol");
                return;
            }
            std::string sTeamName = req["team_name"];
            int nTeamNum = m_vTeams.size() + 1;
            ModelTeamConf teamConf;
            teamConf.setId("team" + std::to_string(nTeamNum));
            teamConf.setNum(nTeamNum);
            teamConf.setName(sTeamName);
            teamConf.setActive(true);
            teamConf.setIpAddress(sIpAddress);
            m_vTeams.push_back(teamConf);
            nlohmann::json resp;
            resp["host"] = sJuryIpAddress;
            resp["port"] = m_nJuryPort;
            resp["teamnum"] = nTeamNum;
            std::string sResponse = resp.dump();
            send(m_nSockFd, sResponse.c_str(), sResponse.length(),MSG_CONFIRM);
            Log::ok(TAG, "OK " + sIpAddress + " (team" + std::to_string(nTeamNum) + ": " + sTeamName + ") ");
            close(m_nSockFd);
            return;
        }
    }
    Log::err(TAG, "FAIL " + sIpAddress);
    return;
}

// ----------------------------------------------------------------------

void SearchLazyConf::listCurrentIpAddresses(std::vector<std::string>& vIpAddrs) {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;      

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr;
            char netmaskBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, netmaskBuffer, INET_ADDRSTRLEN);

            std::string sIfName(ifa->ifa_name);
            std::string sAddress(addressBuffer);
            std::string sNetmask(netmaskBuffer);
            
            if (sNetmask == "255.255.255.0") {
                vIpAddrs.push_back(sAddress);
                Log::ok(TAG, "Found jury address " + sAddress);
            } else {
                Log::warn(TAG, "Ignored address " + sAddress + " (with mask: " + sNetmask + ")");
            }
            // std::cout << sIfName << " [" << sNetmask << "] " << sAddress << "\n";
            // printf("'%s': %s\n", ifa->ifa_name, addressBuffer); 
         } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

            std::string sIfName(ifa->ifa_name);
            std::string sAddress(addressBuffer);

            Log::warn(TAG, "Ignored ipv6 address " + sAddress);
            // printf("'%s': %s\n", ifa->ifa_name, addressBuffer); 
        } 
    }
    if (ifAddrStruct!=NULL) 
        freeifaddrs(ifAddrStruct);//remember to free ifAddrStruct
    return;
}