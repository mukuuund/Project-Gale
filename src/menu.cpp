#include "../include/menu.h"
#include "../include/utils.h"
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <cstdlib> // for system()

Menu::Menu() {
    // Create data directory if it doesn't exist
    #ifdef _WIN32
    system("mkdir data 2> nul");
    #else
    system("mkdir -p data");
    #endif

    loadData();
}

void Menu::run() {
    mainMenu();
}

void Menu::mainMenu() {
    while (true) {
        Utils::clearScreen();
        Utils::printAsciiArt();
        Utils::printHeader("Main Menu");

        std::vector<std::string> options = {
            "Applicant Management",
            "Employer Management",
            "Matching System",
            "Exit"
        };

        Utils::showMenuOptions(options);

        int choice = Utils::getIntInput("Enter your choice: ", 1, options.size());

        switch (choice) {
            case 1: applicantMenu(); break;
            case 2: employerMenu(); break;
            case 3: matchingMenu(); break;
            case 4:
                saveData();
                Utils::printSuccess("Data saved successfully. Exiting...");
                return;
        }
    }
}

void Menu::applicantMenu() {
    while (true) {
        Utils::clearScreen();
        Utils::printHeader("Applicant Management");

        std::vector<std::string> options = {
            "Add Applicant",
            "View Applicants",
            "Edit Applicant",
            "Delete Applicant",
            "Back to Main Menu"
        };

        Utils::showMenuOptions(options);

        int choice = Utils::getIntInput("Enter your choice: ", 1, options.size());

        switch (choice) {
            case 1: addApplicant(); break;
            case 2: viewApplicants(); break;
            case 3: editApplicant(); break;
            case 4: deleteApplicant(); break;
            case 5: return;
        }
    }
}

void Menu::employerMenu() {
    while (true) {
        Utils::clearScreen();
        Utils::printHeader("Employer Management");

        std::vector<std::string> options = {
            "Add Employer",
            "View Employers",
            "Edit Employer",
            "Delete Employer",
            "Back to Main Menu"
        };

        Utils::showMenuOptions(options);

        int choice = Utils::getIntInput("Enter your choice: ", 1, options.size());

        switch (choice) {
            case 1: addEmployer(); break;
            case 2: viewEmployers(); break;
            case 3: editEmployer(); break;
            case 4: deleteEmployer(); break;
            case 5: return;
        }
    }
}

void Menu::matchingMenu() {
    while (true) {
        Utils::clearScreen();
        Utils::printHeader("Matching System");

        std::vector<std::string> options = {
            "Run Matching Algorithm",
            "View Current Matches",
            "Back to Main Menu"
        };

        Utils::showMenuOptions(options);

        int choice = Utils::getIntInput("Enter your choice: ", 1, options.size());

        switch (choice) {
            case 1: runMatching(); break;
            case 2: viewMatches(); break;
            case 3: return;
        }
    }
}

void Menu::addApplicant() {
    Utils::clearScreen();
    Utils::printHeader("Add New Applicant");

    std::string id = "A" + std::to_string(applicants.size() + 1);
    std::string name = Utils::getStringInput("Enter applicant name: ");
    int age = Utils::getIntInput("Enter applicant age: ", 18, 70);

    std::cout << "Education Levels:\n";
    std::cout << "1. High School\n";
    std::cout << "2. Bachelor's\n";
    std::cout << "3. Master's\n";
    std::cout << "4. PhD\n";
    int eduChoice = Utils::getIntInput("Select education level: ", 1, 4);
    std::string education;
    switch (eduChoice) {
        case 1: education = "High School"; break;
        case 2: education = "Bachelor's"; break;
        case 3: education = "Master's"; break;
        case 4: education = "PhD"; break;
    }

    double experience = Utils::getDoubleInput("Enter years of experience: ", 0, 50);

    std::cout << "Enter skills (comma separated): ";
    std::string skillsInput = Utils::getStringInput("");
    auto skills = Utils::split(skillsInput, ',');

    // Handle employer preferences
    std::vector<std::string> preferences;
    if (!employers.empty()) {
        std::cout << "\nAvailable Employers:\n";
        for (auto i = 0; i < employers.size(); ++i) {
            std::cout << i+1 << ". ";
            employers[i].displayShort();
            std::cout << "\n";
        }

        std::cout << "\nEnter employer preferences (comma separated numbers, or leave blank for none): ";
        std::string prefInput = Utils::getStringInput("");
        if (!prefInput.empty()) {
            auto prefNumbers = Utils::split(prefInput, ',');
            for (const auto &numStr : prefNumbers) {
                try {
                    int num = std::stoi(numStr);
                    if (num > 0 && num <= static_cast<int>(employers.size())) {
                        preferences.push_back(employers[num-1].getId());
                    }
                } catch (...) {
                    // Ignore invalid numbers
                }
            }
        }
    } else {
        Utils::printInfo("No employers available - preferences will be empty. You can edit them later.");
    }

    Applicant newApplicant(id, name, age, education, skills, experience, preferences);
    applicants.push_back(newApplicant);
    saveData();

    Utils::printSuccess("Applicant added and saved successfully!");
    Utils::pause();
}

void Menu::viewApplicants() {
    Utils::clearScreen();
    Utils::printHeader("Applicant List");

    if (applicants.empty()) {
        Utils::printInfo("No applicants available.");
        Utils::pause();
        return;
    }

    for (auto i = 0; i < applicants.size(); ++i) {
        std::cout << i+1 << ". ";
        applicants[i].displayShort();
        std::cout << "\n";
    }

    int choice = Utils::getIntInput("\nEnter applicant number to view details (0 to go back): ", 0, applicants.size());
    if (choice > 0) {
        Utils::clearScreen();
        applicants[choice-1].display();
        Utils::pause();
    }
}

void Menu::editApplicant() {
    int index = displayItems(applicants, "Select Applicant to Edit");
    if (index == -1) return;

    Applicant &applicant = applicants[index];

    Utils::clearScreen();
    applicant.display();
    std::cout << "\n";

    std::cout << "What would you like to edit?\n";
    std::cout << "1. Name\n";
    std::cout << "2. Age\n";
    std::cout << "3. Education\n";
    std::cout << "4. Skills\n";
    std::cout << "5. Experience\n";
    std::cout << "6. Preferences\n";
    std::cout << "7. Cancel\n";

    int choice = Utils::getIntInput("Enter your choice: ", 1, 7);

    switch (choice) {
        case 1: {
            std::string name = Utils::getStringInput("Enter new name: ");
            applicant.setName(name);
            break;
        }
        case 2: {
            int age = Utils::getIntInput("Enter new age: ", 18, 70);
            const_cast<Applicant&>(applicant).setAge(age);
            break;
        }
        case 3: {
            std::cout << "Education Levels:\n";
            std::cout << "1. High School\n";
            std::cout << "2. Bachelor's\n";
            std::cout << "3. Master's\n";
            std::cout << "4. PhD\n";
            int eduChoice = Utils::getIntInput("Select education level: ", 1, 4);
            std::string education;
            switch (eduChoice) {
                case 1: education = "High School"; break;
                case 2: education = "Bachelor's"; break;
                case 3: education = "Master's"; break;
                case 4: education = "PhD"; break;
            }
            const_cast<Applicant&>(applicant).setEducation(education);
            break;
        }
        case 4: {
            std::cout << "Enter skills (comma separated): ";
            std::string skillsInput = Utils::getStringInput("");
            auto skills = Utils::split(skillsInput, ',');
            const_cast<Applicant&>(applicant).setSkills(skills);
            break;
        }
        case 5: {
            double experience = Utils::getDoubleInput("Enter years of experience: ", 0, 50);
            const_cast<Applicant&>(applicant).setExperience(experience);
            break;
        }
        case 6: {
            std::vector<std::string> preferences;
            if (!employers.empty()) {
                std::cout << "\nAvailable Employers:\n";
                for (auto i = 0; i < employers.size(); ++i) {
                    std::cout << i+1 << ". ";
                    employers[i].displayShort();
                    std::cout << "\n";
                }

                std::cout << "\nEnter employer preferences (comma separated numbers): ";
                std::string prefInput = Utils::getStringInput("");
                auto prefNumbers = Utils::split(prefInput, ',');
                for (const auto &numStr : prefNumbers) {
                    try {
                        int num = std::stoi(numStr);
                        if (num > 0 && num <= static_cast<int>(employers.size())) {
                            preferences.push_back(employers[num-1].getId());
                        }
                    } catch (...) {
                        // Ignore invalid numbers
                    }
                }
            } else {
                Utils::printInfo("No employers available to set preferences.");
            }
            applicant.setPreferences(preferences);
            break;
        }
        case 7: return;
    }

    applicant.calculateScore();
    saveData();
    Utils::printSuccess("Applicant updated and saved successfully!");
    Utils::pause();
}

void Menu::deleteApplicant() {
    int index = displayItems(applicants, "Select Applicant to Delete");
    if (index == -1) return;

    applicants.erase(applicants.begin() + index);
    saveData();
    Utils::printSuccess("Applicant deleted and changes saved!");
    Utils::pause();
}

void Menu::addEmployer() {
    Utils::clearScreen();
    Utils::printHeader("Add New Employer");

    std::string id = "E" + std::to_string(employers.size() + 1);
    std::string name = Utils::getStringInput("Enter employer name: ");
    std::string industry = Utils::getStringInput("Enter industry: ");

    std::cout << "Enter required skills (comma separated): ";
    std::string skillsInput = Utils::getStringInput("");
    auto requiredSkills = Utils::split(skillsInput, ',');

    double minScore = Utils::getDoubleInput("Enter minimum required score: ", 0, 10);
    int positions = Utils::getIntInput("Enter number of positions available: ", 1, 100);

    // Handle applicant preferences
    std::vector<std::string> preferences;
    if (!applicants.empty()) {
        std::cout << "\nAvailable Applicants:\n";
        for (auto i = 0; i < applicants.size(); ++i) {
            std::cout << i+1 << ". ";
            applicants[i].displayShort();
            std::cout << "\n";
        }

        std::cout << "\nEnter applicant preferences (comma separated numbers, or leave blank for none): ";
        std::string prefInput = Utils::getStringInput("");
        if (!prefInput.empty()) {
            auto prefNumbers = Utils::split(prefInput, ',');
            for (const auto &numStr : prefNumbers) {
                try {
                    int num = std::stoi(numStr);
                    if (num > 0 && num <= static_cast<int>(applicants.size())) {
                        preferences.push_back(applicants[num-1].getId());
                    }
                } catch (...) {
                    // Ignore invalid numbers
                }
            }
        }
    } else {
        Utils::printInfo("No applicants available - preferences will be empty. You can edit them later.");
    }

    Employer newEmployer(id, name, industry, requiredSkills, minScore, positions, preferences);
    employers.push_back(newEmployer);
    saveData();

    Utils::printSuccess("Employer added and saved successfully!");
    Utils::pause();
}

void Menu::viewEmployers() {
    Utils::clearScreen();
    Utils::printHeader("Employer List");

    if (employers.empty()) {
        Utils::printInfo("No employers available.");
        Utils::pause();
        return;
    }

    for (auto i = 0; i < employers.size(); ++i) {
        std::cout << i+1 << ". ";
        employers[i].displayShort();
        std::cout << "\n";
    }

    int choice = Utils::getIntInput("\nEnter employer number to view details (0 to go back): ", 0, employers.size());
    if (choice > 0) {
        Utils::clearScreen();
        employers[choice-1].display();
        Utils::pause();
    }
}

void Menu::editEmployer() {
    int index = displayItems(employers, "Select Employer to Edit");
    if (index == -1) return;

    Employer &employer = employers[index];

    Utils::clearScreen();
    employer.display();
    std::cout << "\n";

    std::cout << "What would you like to edit?\n";
    std::cout << "1. Name\n";
    std::cout << "2. Industry\n";
    std::cout << "3. Required Skills\n";
    std::cout << "4. Minimum Score\n";
    std::cout << "5. Positions Available\n";
    std::cout << "6. Preferences\n";
    std::cout << "7. Cancel\n";

    int choice = Utils::getIntInput("Enter your choice: ", 1, 7);

    switch (choice) {
        case 1: {
            std::string name = Utils::getStringInput("Enter new name: ");
            employer.setName(name);
            break;
        }
        case 2: {
            std::string industry = Utils::getStringInput("Enter new industry: ");
            const_cast<Employer&>(employer).setIndustry(industry);
            break;
        }
        case 3: {
            std::cout << "Enter required skills (comma separated): ";
            std::string skillsInput = Utils::getStringInput("");
            auto skills = Utils::split(skillsInput, ',');
            const_cast<Employer&>(employer).setRequiredSkills(skills);
            break;
        }
        case 4: {
            double minScore = Utils::getDoubleInput("Enter new minimum score: ", 0, 10);
            employer.setMinScoreRequired(minScore);
            break;
        }
        case 5: {
            int positions = Utils::getIntInput("Enter new number of positions: ", 1, 100);
            employer.setPositionsAvailable(positions);
            break;
        }
        case 6: {
            std::vector<std::string> preferences;
            if (!applicants.empty()) {
                std::cout << "\nAvailable Applicants:\n";
                for (auto i = 0; i < applicants.size(); ++i) {
                    std::cout << i+1 << ". ";
                    applicants[i].displayShort();
                    std::cout << "\n";
                }

                std::cout << "\nEnter applicant preferences (comma separated numbers): ";
                std::string prefInput = Utils::getStringInput("");
                auto prefNumbers = Utils::split(prefInput, ',');
                for (const auto &numStr : prefNumbers) {
                    try {
                        int num = std::stoi(numStr);
                        if (num > 0 && num <= static_cast<int>(applicants.size())) {
                            preferences.push_back(applicants[num-1].getId());
                        }
                    } catch (...) {
                        // Ignore invalid numbers
                    }
                }
            } else {
                Utils::printInfo("No applicants available to set preferences.");
            }
            employer.setPreferences(preferences);
            break;
        }
        case 7: return;
    }

    saveData();
    Utils::printSuccess("Employer updated and saved successfully!");
    Utils::pause();
}

void Menu::deleteEmployer() {
    int index = displayItems(employers, "Select Employer to Delete");
    if (index == -1) return;

    employers.erase(employers.begin() + index);
    saveData();
    Utils::printSuccess("Employer deleted and changes saved!");
    Utils::pause();
}

void Menu::runMatching() {
    if (applicants.empty() || employers.empty()) {
        Utils::printError("Need at least one applicant and one employer to run matching.");
        Utils::pause();
        return;
    }

    MatchingSystem matchingSystem(applicants, employers);
    matchingSystem.runGaleShapley();
    matchingSystem.displayMatches();
    matchingSystem.saveMatchesToFile("data/matches.txt");

    Utils::printSuccess("\nMatching completed successfully! Results saved to file.");
    Utils::pause();
}

void Menu::viewMatches() {
    std::ifstream inFile("data/matches.txt");
    if (!inFile) {
        Utils::printError("No matching results available. Please run the matching algorithm first.");
        Utils::pause();
        return;
    }

    Utils::clearScreen();
    Utils::printHeader("Current Matches");

    std::unordered_map<std::string, Applicant> applicantMap;
    for (const auto &applicant : applicants) {
        applicantMap[applicant.getId()] = applicant;
    }

    std::unordered_map<std::string, Employer> employerMap;
    for (const auto &employer : employers) {
        employerMap[employer.getId()] = employer;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        auto parts = Utils::split(line, '|');
        if (parts.size() != 2) continue;

        std::string employerId = parts[0];
        auto applicantIds = Utils::split(parts[1], ',');

        if (employerMap.find(employerId) == employerMap.end()) continue;

        const auto &employer = employerMap[employerId];
        std::cout << "\n" << Utils::BOLD << "Employer: " << Utils::RESET
                  << employer.getName() << " (ID: " << employerId << ")\n";
        std::cout << "Matched Applicants:\n";

        std::vector<std::string> headers = {"Name", "Score", "Skills"};
        Utils::printTableHeader(headers);

        for (const auto &applicantId : applicantIds) {
            if (applicantMap.find(applicantId) == applicantMap.end()) continue;

            const auto &applicant = applicantMap[applicantId];
            std::vector<std::string> row = {
                applicant.getName(),
                std::to_string(applicant.getScore()),
                Utils::join(applicant.getSkills(), ", ")
            };
            Utils::printTableRow(row);
        }
    }

    Utils::pause();
}

void Menu::loadData() {
    // Load applicants
    std::ifstream applicantFile("data/applicants.txt");
    if (applicantFile) {
        std::string line;
        while (std::getline(applicantFile, line)) {
            try {
                applicants.push_back(Applicant::deserialize(line));
            } catch (...) {
                // Skip invalid entries
            }
        }
        applicantFile.close();
    }

    // Load employers
    std::ifstream employerFile("data/employers.txt");
    if (employerFile) {
        std::string line;
        while (std::getline(employerFile, line)) {
            try {
                employers.push_back(Employer::deserialize(line));
            } catch (...) {
                // Skip invalid entries
            }
        }
        employerFile.close();
    }
}

void Menu::saveData() {
    // Save applicants
    std::ofstream applicantFile("data/applicants.txt");
    if (applicantFile.is_open()) {
        for (const auto &applicant : applicants) {
            applicantFile << applicant.serialize() << "\n";
        }
        applicantFile.close();
    } else {
        Utils::printError("Failed to save applicants data!");
    }

    // Save employers
    std::ofstream employerFile("data/employers.txt");
    if (employerFile.is_open()) {
        for (const auto &employer : employers) {
            employerFile << employer.serialize() << "\n";
        }
        employerFile.close();
    } else {
        Utils::printError("Failed to save employers data!");
    }
}

template<typename T>
int Menu::displayItems(const std::vector<T> &items, const std::string &title) {
    Utils::clearScreen();
    Utils::printHeader(title);

    if (items.empty()) {
        Utils::printInfo("No items available.");
        Utils::pause();
        return -1;
    }

    for (auto i = 0; i < items.size(); ++i) {
        std::cout << i+1 << ". ";
        items[i].displayShort();
        std::cout << "\n";
    }

    int choice = Utils::getIntInput("\nEnter your choice (0 to cancel): ", 0, items.size());
    return (choice > 0) ? choice-1 : -1;
}
