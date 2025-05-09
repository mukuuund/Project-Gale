#ifndef EMPLOYER_H
#define EMPLOYER_H

#include <string>
#include <vector>

class Employer {
private:
    std::string id;
    std::string name;
    std::string industry;
    std::vector<std::string> requiredSkills;
    double minScoreRequired;
    int positionsAvailable;
    std::vector<std::string> preferences;

public:
    // Default constructor
    Employer() : id(""), name(""), industry(""), minScoreRequired(0.0), positionsAvailable(0) {}

    Employer(const std::string &id, const std::string &name, const std::string &industry,
             const std::vector<std::string> &requiredSkills, double minScoreRequired,
             int positionsAvailable, const std::vector<std::string> &preferences);

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getIndustry() const { return industry; }
    std::vector<std::string> getRequiredSkills() const { return requiredSkills; }
    double getMinScoreRequired() const { return minScoreRequired; }
    int getPositionsAvailable() const { return positionsAvailable; }
    std::vector<std::string> getPreferences() const { return preferences; }

    // Setters
    void setName(const std::string &newName) { name = newName; }
    void setPreferences(const std::vector<std::string> &newPrefs) { preferences = newPrefs; }
    void setPositionsAvailable(int positions) { positionsAvailable = positions; }

    bool isQualified(double applicantScore) const {
        return applicantScore >= minScoreRequired;
    }

    void display() const;
    void displayShort() const;


    //new setters
    void setIndustry(const std::string &newIndustry) { industry = newIndustry; }
    void setRequiredSkills(const std::vector<std::string> &newSkills) { requiredSkills = newSkills; }
    void setMinScoreRequired(double newScore) { minScoreRequired = newScore; }

    std::string serialize() const;
    static Employer deserialize(const std::string &data);
};

#endif // EMPLOYER_H
