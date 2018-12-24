#ifndef TS_H
#define TS_H

#include <string>

class TS {

    public:
        static long currentTime_milliseconds();
        static std::string currentTime_filename();
        static std::string currentTime_logformat();
        static std::string formatTimeUTC(int nTimeInSec);
};

#endif // TS_H
