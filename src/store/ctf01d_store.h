
#include <string>
#include <vector>
#include "json.hpp"

class Ctf01dStoreTeamInfo {
    public:
        Ctf01dStoreTeamInfo();
        bool load(
            const nlohmann::json &jsonTeamInfo, 
            const std::string &sBaseUrl
        );

        const std::string &getUuid() const;
        const std::string &getId() const;
        const std::string &getName() const;
        const std::string &getLogo() const;
        bool contains(std::vector<std::string> vTerms);

    private:
        bool findStringIC(const std::string & strHaystack, const std::string & strNeedle);

        std::string TAG;
        std::string m_sUuid;
        std::string m_sId;
        std::string m_sName;
        std::string m_sLogo;
};

class Ctf01dStore {
    public:
        Ctf01dStore();
        bool init();

        std::vector<Ctf01dStoreTeamInfo> searchTeam(std::vector<std::string> vTerms);
    private:
        std::string TAG;
        std::string extractBaseUrlPath(const std::string &sUrl);

        std::string m_sCtf01dStoreCacheDir;
        std::vector<Ctf01dStoreTeamInfo> m_vTeams;
};