#ifndef ILIGHT_HTTP_DEQUE_REQUESTS_H
#define ILIGHT_HTTP_DEQUE_REQUESTS_H

#include <string>
#include <vector>
#include <map>
#include <light_http_request.h>

class ILightHttpDequeRequests {
	public:
		virtual ~ILightHttpDequeRequests(){};
		virtual LightHttpRequest * popRequest() = 0;
};

#endif // ILIGHT_HTTP_DEQUE_REQUESTS_H