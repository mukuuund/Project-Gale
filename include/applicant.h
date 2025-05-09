#ifndef APPLICANT_H
#define APPLICANT_H

#include <string>
#include <vector>
#include <map>

class Applicant {
private:
    std::string id;
    std::string name;
    int age;
    std::string education;
    std::vector<std::string> skills;
    double experience;
    std::vector<std::string> preferences;
    double score;

public:
    // Default constructor
    Applicant() : id(""), name(""), age(0), education(""), experience(0.0), score(0.0) {}

    Applicant(const std::string &id, const std::string &name, int age,
              const std::string &education, const std::vector<std::string> &skills,
              double experience, const std::vector<std::string> &preferences);

    // Getters
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getEducation() const { return education; }
    std::vector<std::string> getSkills() const { return skills; }
    double getExperience() const { return experience; }
    std::vector<std::string> getPreferences() const { return preferences; }
    double getScore() const { return score; }

    // Setters
    void setName(const std::string &newName) { name = newName; }
    void setPreferences(const std::vector<std::string> &newPrefs) { preferences = newPrefs; }
    void calculateScore();

    // New Setters
    void setAge(int newAge) { age = newAge; }
    void setEducation(const std::string &newEdu) { education = newEdu; }
    void setSkills(const std::vector<std::string> &newSkills) { skills = newSkills; }
    void setExperience(double newExp) { experience = newExp; }

    // Display
    void display() const;
    void displayShort() const;

    // File operations
    std::string serialize() const;
    static Applicant deserialize(const std::string &data);
};

#endif // APPLICANT_H
