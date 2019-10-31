#ifndef SERVICE_STATUS_CELL_H
#define SERVICE_STATUS_CELL_H

#include <string>
#include <map>
#include <mutex>
#include <service.h>

class ServiceStatusCell {
    public:
        // enum for service status
        static std::string SERVICE_UP;
        static std::string SERVICE_DOWN;
        static std::string SERVICE_MUMBLE;
        static std::string SERVICE_CORRUPT;
        static std::string SERVICE_SHIT;
        static std::string SERVICE_WAIT;
        static std::string SERVICE_COFFEEBREAK;

        ServiceStatusCell(const Service &serviceConf, int nGameStartinSec, int nGameEndInSec);
        const std::string &serviceId();

        void setDefence(int nDefence);
        int defence();

        void setAttack(int nAttack);
        int attack();

        void setFlagsPutted(int nFlagsPutted);
        int flagsPutted();

        int getUptimeInSec();
        void setUpPointTime(int nUpPointTimeInSec);

        void setStatus(const std::string &sStatus);
        std::string status();

        double calculateScore();

    private:
        std::string TAG;
        std::mutex m_mutexServiceStatus;
        Service m_serviceConf;
        std::string m_sServiceId;
        std::string m_sStatus; // may be char[10] ?
        int m_nDefence;
        int m_nAttack;

        // for SLA / uptime
        double m_nUptime;
        int m_nGameStartInSec;
        int m_nGameEndInSec;
        int m_nFlagsPutted;
};

#endif // SERVICE_STATUS_CELL_H
