#include "../include/employer.h"
#include "../include/utils.h"
#include <sstream>

Employer::Employer(const std::string &id, const std::string &name, const std::string &industry,
                   const std::vector<std::string> &requiredSkills, double minScoreRequired,
                   int positionsAvailable, const std::vector<std::string> &preferences)
    : id(id), name(name), industry(industry), requiredSkills(requiredSkills),
      minScoreRequired(minScoreRequired), positionsAvailable(positionsAvailable),
      preferences(preferences) {}

void Employer::display() const {
    Utils::printBox("Employer Details");
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Industry: " << industry << "\n";
    std::cout << "Required Skills: " << Utils::join(requiredSkills, ", ") << "\n";
    std::cout << "Minimum Score Required: " << minScoreRequired << "\n";
    std::cout << "Positions Available: " << positionsAvailable << "\n";

    std::cout << "Applicant Preferences: ";
    for (auto i = 0; i < preferences.size(); ++i) {
        if (i != 0) std::cout << " > ";
        std::cout << preferences[i];
    }
    std::cout << "\n";
}

void Employer::displayShort() const {
    std::cout << "[" << id << "] " << name << " (Min Score: " << minScoreRequired
              << ", Positions: " << positionsAvailable << ")";
}

std::string Employer::serialize() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << industry << "|"
       << Utils::join(requiredSkills, ",") << "|"
       << minScoreRequired << "|" << positionsAvailable << "|"
       << Utils::join(preferences, ",");
    return ss.str();
}

Employer Employer::deserialize(const std::string &data) {
    auto parts = Utils::split(data, '|');
    if (parts.size() < 7) {
        throw std::runtime_error("Invalid employer data format");
    }

    std::string id = parts[0];
    std::string name = parts[1];
    std::string industry = parts[2];
    auto requiredSkills = Utils::split(parts[3], ',');
    double minScoreRequired = std::stod(parts[4]);
    int positionsAvailable = std::stoi(parts[5]);
    auto preferences = Utils::split(parts[6], ',');

    return Employer(id, name, industry, requiredSkills, minScoreRequired,
                   positionsAvailable, preferences);
}
