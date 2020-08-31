#ifndef CTF01D_FLAG_H
#define CTF01D_FLAG_H

#include <string>

class Ctf01dFlag {
    public:
        Ctf01dFlag();
        void generateRandomFlag(int nTimeFlagLifeInMin, const std::string &sTeamId, const std::string &sServiceId);

        void generateId();
        void setId(const std::string &sId);
        std::string getId() const;

        void generateValue();
        void setValue(const std::string &sValue);
        std::string getValue() const;

        void setTeamId(const std::string &sTeamId);
        const std::string &getTeamId() const;

        void setServiceId(const std::string &sServiceId);
        const std::string &getServiceId() const;

        void setTimeStartInMs(long nTimeStart);
        long getTimeStartInMs() const;

        void setTimeEndInMs(long nTimeEnd);
        long getTimeEndInMs() const;

        void copyFrom(const Ctf01dFlag &flag);

    private:
        std::string m_sId;
        std::string m_sValue;
        std::string m_sTeamId;
        std::string m_sServiceId;
        long m_nTimeStartInMs;
        long m_nTimeEndInMs;
};

#endif // CTF01D_FLAG_H
