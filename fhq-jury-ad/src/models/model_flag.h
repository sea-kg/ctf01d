#ifndef MODEL_FLAG_H
#define MODEL_FLAG_H

#include <string>

class ModelFlag {
    public:
        ModelFlag();
        void generateRandomFlag(int nTimeFlagLifeInMin);

        void setId(const std::string &sId);
        void generateId();
        std::string id() const;

        void setValue(const std::string &sValue);
        void generateValue();
        std::string value() const;

        void setTeamId(const std::string &sTeamId);
        const std::string &teamId() const;

        void setServiceId(const std::string &sServiceId);
        const std::string &serviceId() const;

        void setTimeStart(long nTimeStart);
        long timeStart() const;

        void setTimeEnd(long nTimeEnd);
        long timeEnd() const;

        void setTeamStole(const std::string &sTeamStole);
        const std::string &teamStole() const;

        void copyFrom(const ModelFlag &flag);

    private:
        std::string m_sId;
        std::string m_sValue;
        std::string m_sTeamId;
        std::string m_sServiceId;
        long m_nTimeStart;
        long m_nTimeEnd;
        std::string m_sTeamStole;
};

#endif // MODEL_FLAG_H
