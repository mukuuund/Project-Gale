#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace Utils {
    // Color codes
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BOLD = "\033[1m";

    // Display functions
    void printHeader(const std::string &title);
    void printMatchHeader(const std::string &title);
    void printSuccess(const std::string &message);
    void printError(const std::string &message);
    void printInfo(const std::string &message);
    void printTableHeader(const std::vector<std::string> &headers);
    void printTableRow(const std::vector<std::string> &values);
    void printBox(const std::string &content);
    void printMatchResult(const std::string &employer, const std::vector<std::string> &applicants);
    void printAsciiArt();
    void showMenuOptions(const std::vector<std::string> &options);

    // Utility functions
    std::string trim(const std::string &str);
    std::vector<std::string> split(const std::string &str, char delimiter);
    std::string join(const std::vector<std::string> &vec, const std::string &delimiter);
    std::string getStatusColor(double score, double required);

    // Input functions
    int getIntInput(const std::string &prompt, int min, int max);
    std::string getStringInput(const std::string &prompt);
    double getDoubleInput(const std::string &prompt, double min, double max);

    // System functions
    void clearScreen();
    void pause();
};

#endif // UTILS_H
