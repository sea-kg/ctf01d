#include "light_http_response.h"
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
#include <fs.h>
#include <ts.h>

// ----------------------------------------------------------------------
// LightHttpResponse

// enum for http responses
std::string LightHttpResponse::RESP_OK = "HTTP/1.1 200 OK";
std::string LightHttpResponse::RESP_BAD_REQUEST = "HTTP/1.1 400 Bad Request";
std::string LightHttpResponse::RESP_FORBIDDEN = "HTTP/1.1 403 Forbidden";
std::string LightHttpResponse::RESP_NOT_FOUND = "HTTP/1.1 404 Not Found";
std::string LightHttpResponse::RESP_PAYLOAD_TOO_LARGE = "HTTP/1.1 413 Payload Too Large";
std::string LightHttpResponse::RESP_INTERNAL_SERVER_ERROR = "HTTP/1.1 500 Internal Server Error";
std::string LightHttpResponse::RESP_NOT_IMPLEMENTED = "HTTP/1.1 501 Not Implemented";

// ----------------------------------------------------------------------
