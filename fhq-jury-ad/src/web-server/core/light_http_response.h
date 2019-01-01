#ifndef LIGHT_HTTP_RESPONSE_H
#define LIGHT_HTTP_RESPONSE_H

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

class LightHttpResponse {
	public:
        // enum for http responses
        static std::string RESP_OK;
        static std::string RESP_BAD_REQUEST;
		static std::string RESP_FORBIDDEN;
        static std::string RESP_NOT_FOUND;
        static std::string RESP_PAYLOAD_TOO_LARGE;
		static std::string RESP_INTERNAL_SERVER_ERROR;
		static std::string RESP_NOT_IMPLEMENTED;
};

#endif // LIGHT_HTTP_RESPONSE_H