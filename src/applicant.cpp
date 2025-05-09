#include "../include/applicant.h"
#include "../include/utils.h"
#include <algorithm>
#include <sstream>
#include <cmath>

Applicant::Applicant(const std::string &id, const std::string &name, int age,
                     const std::string &education, const std::vector<std::string> &skills,
                     double experience, const std::vector<std::string> &preferences)
    : id(id), name(name), age(age), education(education), skills(skills),
      experience(experience), preferences(preferences) {
    calculateScore();
}

void Applicant::calculateScore() {
    double educationScore = 0.0;
    if (education == "High School") educationScore = 2.0;
    else if (education == "Bachelor's") educationScore = 3.0;
    else if (education == "Master's") educationScore = 4.0;
    else if (education == "PhD") educationScore = 5.0;

    double skillScore = skills.size() * 0.5;
    double experienceScore = std::min(experience * 0.5, 5.0);

    double ageFactor = 1.0;
    if (age < 25) ageFactor = 0.8 + (age - 18) * 0.0286;
    else if (age > 40) ageFactor = 1.0 - (age - 40) * 0.02;

    score = (educationScore + skillScore + experienceScore) * ageFactor;
}

void Applicant::display() const {
    Utils::printBox("Applicant Details");
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Age: " << age << "\n";
    std::cout << "Education: " << education << "\n";
    std::cout << "Experience: " << experience << " years\n";
    std::cout << "Skills: " << Utils::join(skills, ", ") << "\n";
    std::cout << "Calculated Score: " << score << "\n";

    std::cout << "Company Preferences: ";
    for (auto i = 0; i < preferences.size(); ++i) {
        if (i != 0) std::cout << " > ";
        std::cout << preferences[i];
    }
    std::cout << "\n";
}

void Applicant::displayShort() const {
    std::cout << "[" << id << "] " << name << " (Score: " << score << ")";
}

std::string Applicant::serialize() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << age << "|" << education << "|"
       << experience << "|" << Utils::join(skills, ",") << "|"
       << Utils::join(preferences, ",");
    return ss.str();
}

Applicant Applicant::deserialize(const std::string &data) {
    auto parts = Utils::split(data, '|');
    if (parts.size() < 7) {
        throw std::runtime_error("Invalid applicant data format");
    }

    std::string id = parts[0];
    std::string name = parts[1];
    int age = std::stoi(parts[2]);
    std::string education = parts[3];
    double experience = std::stod(parts[4]);
    auto skills = Utils::split(parts[5], ',');
    auto preferences = Utils::split(parts[6], ',');

    return Applicant(id, name, age, education, skills, experience, preferences);
}
