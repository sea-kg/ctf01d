#ifndef MODEL_SERVICE_STATUS_H
#define MODEL_SERVICE_STATUS_H

#include <string>
#include <map>
#include <mutex>

class ModelServiceStatus {
    public:
        // enum for service status
        static std::string SERVICE_UP;
        static std::string SERVICE_DOWN;
        static std::string SERVICE_MUMBLE;
        static std::string SERVICE_CORRUPT;
        static std::string SERVICE_SHIT;
        static std::string SERVICE_WAIT;

        ModelServiceStatus(int nServiceNum);
        int serviceNum();

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
        int m_nServiceNum;
        std::string m_sStatus; // may be char[10] ?
        double m_nSLA;
        int m_nDefence;
        int m_nAttack;
};

#endif // MODEL_SERVICE_STATUS_H
