#ifndef MENU_H
#define MENU_H

#include <vector>
#include <functional>
#include <unordered_map>
#include "applicant.h"
#include "employer.h"
#include "matching.h"

class Menu {
public:
    Menu();
    void run();

private:
    std::vector<Applicant> applicants;
    std::vector<Employer> employers;

    void mainMenu();
    void applicantMenu();
    void employerMenu();
    void matchingMenu();

    void addApplicant();
    void viewApplicants();
    void editApplicant();
    void deleteApplicant();

    void addEmployer();
    void viewEmployers();
    void editEmployer();
    void deleteEmployer();

    void runMatching();
    void viewMatches();

    void loadData();
    void saveData();

    template<typename T>
    int displayItems(const std::vector<T> &items, const std::string &title);
};

#endif // MENU_H
