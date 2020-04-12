#ifndef EMPLOY_FLAGS_H
#define EMPLOY_FLAGS_H

#include <wsjcpp_employees.h>
#include <string>
#include <mutex>
#include <fstream>

// ---------------------------------------------------------------------

class Flag {
    public:
        Flag();
        void generateRandomFlag(int nTimeFlagLifeInMin, const std::string &sTeamId, const std::string &sServiceId);

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

        void copyFrom(const Flag &flag);

    private:
        std::string m_sId;
        std::string m_sValue;
        std::string m_sTeamId;
        std::string m_sServiceId;
        long m_nTimeStart;
        long m_nTimeEnd;
        std::string m_sTeamStole;
};

// ---------------------------------------------------------------------

class TableFlagsAttempt {
    public:
        TableFlagsAttempt(const std::string &sDir);
        ~TableFlagsAttempt();
        void append(const std::string &sTeamId, const std::string &sFlag);
        long count();

    private:
        bool doRotateFilename();
        std::string TAG;
        std::string m_sDir;
        std::string m_sFilepathCount;
        std::string m_sFilepathData;
        std::mutex m_mutex;
        long m_nCount;
        long m_nRoteteWhen;
        std::ofstream m_ofsFlagsAttempts;
};

// ---------------------------------------------------------------------

class EmployFlags : public WsjcppEmployBase {
    public:
        EmployFlags();
        static std::string name() { return "EmployFlags"; }
        virtual bool init() override;
        virtual bool deinit() override;

        // set directory
        void setDirectory(const std::string &sDirectory);

        // cleanup cache and files
        void clear();

        // add flag attempt
        void insertFlagAttempt(const std::string &sTeamId, const std::string &sFlag);

        // count of flag attempts for init scoreboard
        int numberOfFlagAttempts(const std::string &sTeamId);

    private:
        std::string TAG;
        std::string m_sDirectory;
        std::map<std::string, TableFlagsAttempt *> m_mapFlagAttemps;
};


#endif // EMPLOY_FLAGS_H
