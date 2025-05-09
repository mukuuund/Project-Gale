#ifndef MATCHING_H
#define MATCHING_H

#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include "applicant.h"
#include "employer.h"

class MatchingSystem {
private:
    std::vector<Applicant> applicants;
    std::vector<Employer> employers;
    std::map<std::string, std::vector<std::string>> matches;

public:
    MatchingSystem(const std::vector<Applicant> &applicants,
                  const std::vector<Employer> &employers);

    void runGaleShapley();
    std::map<std::string, std::vector<std::string>> getMatches() const;
    void displayMatches() const;
    void saveMatchesToFile(const std::string &filename) const;
};

#endif // MATCHING_H
