#ifndef ILIGHT_HTTP_REQUEST_H
#define ILIGHT_HTTP_REQUEST_H

#include <string>
#include <vector>
#include <map>

#define MAXPACKETSIZE 4096

class ILightHttpRequest {
	public:
		virtual ~ILightHttpRequest(){};
		virtual std::string address() = 0;
		virtual std::string requestType() = 0;
		virtual std::string requestPath() = 0;
		virtual std::string requestHttpVersion() = 0;
		virtual void response(const std::string &sFirst) = 0;
		virtual void response(const std::string &sFirst, const std::string &sDataType, const std::string &sBody) = 0;
		virtual std::map<std::string,std::string> &requestQueryParams() = 0;
};

#endif // ILIGHT_HTTP_REQUEST_H