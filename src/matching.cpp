#include "../include/matching.h"
#include "../include/utils.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>

MatchingSystem::MatchingSystem(const std::vector<Applicant> &applicants,
                               const std::vector<Employer>  &employers)
  : applicants(applicants), employers(employers)
{}

void MatchingSystem::runGaleShapley() {

    std::vector<Applicant*> freeApplicants;
    freeApplicants.reserve(applicants.size());
    for (auto &a : applicants) {
        freeApplicants.push_back(&a);
    }


    std::unordered_map<std::string, Employer*> employerMap;
    for (auto &e : employers) {
        employerMap[e.getId()] = &e;
    }
    std::unordered_map<std::string, Applicant*> applicantMap;
    for (auto &a : applicants) {
        applicantMap[a.getId()] = &a;
    }


    std::unordered_map<std::string, size_t> nextProposal;
    for (auto &a : applicants) {
        nextProposal[a.getId()] = 0;
    }


    std::unordered_map<std::string, std::vector<std::string>> tempMatches;
    for (auto &e : employers) {
        tempMatches[e.getId()] = {};
    }


    while (!freeApplicants.empty()) {
        Applicant *app = freeApplicants.back();
        freeApplicants.pop_back();

        const auto &prefs = app->getPreferences();
        size_t &idx = nextProposal[app->getId()];
        if (idx >= prefs.size()) {

            continue;
        }


        std::string empId = prefs[idx++];
        Employer *emp = employerMap[empId];


        if (!emp->isQualified(app->getScore())) {
            freeApplicants.push_back(app);
            continue;
        }


        auto &matchList = tempMatches[empId];
        matchList.push_back(app->getId());


        if (matchList.size() <= static_cast<size_t>(emp->getPositionsAvailable())) {
            continue;
        }


        size_t worstIndex = 0;
        double worstScore = applicantMap[matchList[0]]->getScore();
        for (size_t i = 1; i < matchList.size(); ++i) {
            double s = applicantMap[matchList[i]]->getScore();
            if (s < worstScore) {
                worstScore = s;
                worstIndex = i;
            }
        }
        const std::string worstId = matchList[worstIndex];

        if (app->getId() == worstId) {

            matchList.pop_back();
            freeApplicants.push_back(app);
        } else {

            matchList.erase(matchList.begin() + worstIndex);
            matchList.push_back(app->getId());
            freeApplicants.push_back(applicantMap[worstId]);
        }
    }


    matches = std::map<std::string, std::vector<std::string>>(tempMatches.begin(),
                                                               tempMatches.end());
}

void MatchingSystem::displayMatches() const {
    Utils::printMatchHeader("JOB MATCHING RESULTS");


    std::unordered_map<std::string, Applicant> applicantMap;
    for (auto &a : applicants) applicantMap[a.getId()] = a;
    std::unordered_map<std::string, Employer> employerMap;
    for (auto &e : employers) employerMap[e.getId()] = e;

    bool anyMatches = false;
    for (auto &m : matches) {
        const auto &appIds = m.second;
        if (appIds.empty()) continue;
        anyMatches = true;

        const auto &emp = employerMap[m.first];
        std::cout << Utils::BOLD << Utils::CYAN
                  << "► " << emp.getName()
                  << " (" << appIds.size() << "/"
                  << emp.getPositionsAvailable() << " positions)"
                  << Utils::RESET << "\n";

        for (auto &aid : appIds) {
            const auto &app = applicantMap[aid];
            double ratio = app.getScore() / emp.getMinScoreRequired();
            std::string quality;
            if      (ratio >= 1.2) quality = "★★★★★ (Excellent)";
            else if (ratio >= 1.0) quality = "★★★★ (Good)";
            else if (ratio >= 0.8) quality = "★★★ (Fair)";
            else                   quality = "★★ (Poor)";

            std::string color = Utils::getStatusColor(app.getScore(),
                                                      emp.getMinScoreRequired());

            std::cout << "  • " << Utils::BOLD << app.getName() << Utils::RESET
                      << " (Score: " << color << app.getScore() << Utils::RESET
                      << ", Needs: " << emp.getMinScoreRequired() << ")\n"
                      << "    Skills: " << Utils::join(app.getSkills(), ", ") << "\n"
                      << "    Match Quality: " << color << quality
                      << Utils::RESET << "\n\n";
        }
    }

    if (!anyMatches) {
        Utils::printBox("No Matches Found");
        std::cout << Utils::RED
                  << "  ✗ No successful matches were made.\n"
                  << "  Possible reasons:\n"
                  << "  1. Scores below requirements\n"
                  << "  2. Preference conflicts\n"
                  << "  3. Not enough slots\n"
                  << Utils::RESET;
    }


    std::vector<std::pair<Applicant, std::string>> unmatched;
    for (auto &a : applicants) {
        bool matched = std::any_of(
            matches.begin(), matches.end(),
            [&](auto &pr){
                return std::find(pr.second.begin(), pr.second.end(),
                                 a.getId()) != pr.second.end();
            });
        if (matched) continue;

        bool qualifiedForAny = std::any_of(
            employers.begin(), employers.end(),
            [&](auto &e){ return e.isQualified(a.getScore()); });

        std::string reason;
        if (a.getPreferences().empty())  reason = "No preferences specified";
        else if (!qualifiedForAny)       reason = "Score below all requirements";
        else                              reason = "Preference conflict/limits";
        unmatched.emplace_back(a, reason);
    }

    if (!unmatched.empty()) {
        Utils::printBox("Unmatched Applicants Analysis");
        Utils::printTableHeader({"Name", "Score", "Reason"});
        for (auto &p : unmatched) {
            std::string color = Utils::getStatusColor(p.first.getScore(), 0);
            Utils::printTableRow({
                p.first.getName(),
                color + std::to_string(p.first.getScore()) + Utils::RESET,
                p.second
            });
        }
        std::cout << "\n";
    }


    std::vector<Employer> unfilled;
    for (auto &e : employers) {
        auto it = matches.find(e.getId());
        size_t filled = (it == matches.end() ? 0 : it->second.size());
        if (filled < static_cast<size_t>(e.getPositionsAvailable())) {
            unfilled.push_back(e);
        }
    }
    if (!unfilled.empty()) {
        Utils::printBox("Employers With Unfilled Positions");
        for (auto &e : unfilled) {
            auto it = matches.find(e.getId());
            size_t filled = (it == matches.end() ? 0 : it->second.size());
            std::cout << "  • " << e.getName() << ": "
                      << filled << "/" << e.getPositionsAvailable()
                      << " positions filled\n"
                      << "    Minimum required score: "
                      << e.getMinScoreRequired() << "\n\n";
        }
    }
}

std::map<std::string, std::vector<std::string>>
MatchingSystem::getMatches() const {
    return matches;
}

void MatchingSystem::saveMatchesToFile(const std::string &filename) const {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Unable to open file for writing matches");
    }
    for (auto &m : matches) {
        out << m.first << "|" << Utils::join(m.second, ",") << "\n";
    }
}
