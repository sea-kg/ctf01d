#ifndef ILIGHT_HTTP_HANDLER_H
#define ILIGHT_HTTP_HANDLER_H

#include <string>
#include <vector>
#include <map>
#include <ilighthttprequest.h>

class ILightHttpHandler {
	public:
		virtual bool handle(ILightHttpRequest *pRequest) = 0;
};

#endif // ILIGHT_HTTP_HANDLER_H