#ifndef SERVICE_STATUS_CELL_H
#define SERVICE_STATUS_CELL_H

#include <string>
#include <map>
#include <mutex>

class ServiceStatusCell {
    public:
        // enum for service status
        static std::string SERVICE_UP;
        static std::string SERVICE_DOWN;
        static std::string SERVICE_MUMBLE;
        static std::string SERVICE_CORRUPT;
        static std::string SERVICE_SHIT;
        static std::string SERVICE_WAIT;

        ServiceStatusCell(const std::string &sServiceId);
        const std::string &serviceId();

        void setDefence(int nDefence);
        int defence();

        void setAttack(int nAttack);
        int attack();

        void setSLA(double nSLA);
        double sla();

        void setStatus(const std::string &sStatus);
        std::string status();

    private:
        std::mutex m_mutexServiceStatus;
        std::string m_sServiceId;
        std::string m_sStatus; // may be char[10] ?
        double m_nUptime;
        int m_nDefence;
        int m_nAttack;
};

#endif // SERVICE_STATUS_CELL_H
