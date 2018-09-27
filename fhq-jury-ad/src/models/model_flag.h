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

        void setTeamNum(int nTeamNum);
        int teamNum() const;

        void setServiceNum(int nServiceNum);
        int serviceNum() const;

        void setTimeStart(long nTimeStart);
        long timeStart() const;

        void setTimeEnd(long nTimeEnd);
        long timeEnd() const;

        void setTeamStole(int nTeamStole);
        int teamStole() const;

        void copyFrom(const ModelFlag &flag);

    private:
        std::string m_sId;
        std::string m_sValue;
        int m_nTeamNum;
        int m_nServiceNum;
        long m_nTimeStart;
        long m_nTimeEnd;
        int m_nTeamStole;
};

#endif // MODEL_FLAG_H
