#include "../include/utils.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <sstream>
#include <iomanip>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void Utils::printHeader(const std::string &title) {
    std::string border(title.length() + 8, '═');
    std::cout << BOLD << BLUE << "╔" << border << "╗" << RESET << "\n";
    std::cout << BOLD << BLUE << "║    " << title << "    ║" << RESET << "\n";
    std::cout << BOLD << BLUE << "╚" << border << "╝" << RESET << "\n\n";
}

void Utils::printMatchHeader(const std::string &title) {
    std::string border(title.length() + 8, '=');
    std::cout << BOLD << GREEN << "\n " << border << "\n";
    std::cout << " ||  " << title << "  ||\n";
    std::cout << " " << border << "\n\n" << RESET;
}

void Utils::printSuccess(const std::string &message) {
    std::cout << BOLD << GREEN << "✓ " << message << RESET << "\n";
}

void Utils::printError(const std::string &message) {
    std::cout << BOLD << RED << "✗ " << message << RESET << "\n";
}

void Utils::printInfo(const std::string &message) {
    std::cout << BOLD << CYAN << "ℹ " << message << RESET << "\n";
}

void Utils::printTableHeader(const std::vector<std::string> &headers) {
    std::cout << BOLD << MAGENTA;
    for (const auto &header : headers) {
        std::cout << std::setw(20) << std::left << header;
    }
    std::cout << RESET << "\n";
    std::cout << std::string(headers.size() * 20, '-') << "\n";
}

void Utils::printTableRow(const std::vector<std::string> &values) {
    for (const auto &value : values) {
        std::cout << std::setw(20) << std::left << value;
    }
    std::cout << "\n";
}

void Utils::printBox(const std::string &content) {
    std::string border(content.length() + 4, '─');
    std::cout << BOLD << YELLOW << "┌" << border << "┐" << RESET << "\n";
    std::cout << BOLD << YELLOW << "│  " << content << "  │" << RESET << "\n";
    std::cout << BOLD << YELLOW << "└" << border << "┘" << RESET << "\n";
}

void Utils::printMatchResult(const std::string &employer, const std::vector<std::string> &applicants) {
    std::cout << BOLD << CYAN << "► " << employer << RESET << "\n";
    for (const auto &app : applicants) {
        std::cout << "  • " << app << "\n";
    }
    std::cout << "\n";
}

void Utils::printAsciiArt() {
    std::cout << BOLD << BLUE << R"(

   _____          _      ______             _____ _    _          _____  _      ________     __
  / ____|   /\   | |    |  ____|           / ____| |  | |   /\   |  __ \| |    |  ____\ \   / /
 | |  __   /  \  | |    | |__     ______  | (___ | |__| |  /  \  | |__) | |    | |__   \ \_/ /
 | | |_ | / /\ \ | |    |  __|   |______|  \___ \|  __  | / /\ \ |  ___/| |    |  __|   \   /
 | |__| |/ ____ \| |____| |____            ____) | |  | |/ ____ \| |    | |____| |____   | |
  \_____/_/    \_\______|______|          |_____/|_|  |_/_/    \_\_|    |______|______|  |_|


    )" << RESET << "\n\n";
}

void Utils::showMenuOptions(const std::vector<std::string> &options) {
    std::cout << BOLD << MAGENTA << "╔══════════════════════════╗" << RESET << "\n";
    std::cout << "\n";
    for (auto i = 0; i < options.size(); ++i) {
        std::cout << BOLD << MAGENTA << RESET
                  << std::setw(2) <<" "<< (i+1) << ". "
                  << std::setw(18) << std::left << options[i]
                  << BOLD << MAGENTA  << RESET << "\n";
    }
    std::cout << "\n";
    std::cout << BOLD << MAGENTA << "╚══════════════════════════╝" << RESET << "\n";
}

std::string Utils::trim(const std::string &str) {
    auto first = str.find_first_not_of(' ');
    if (std::string::npos == first) return str;
    auto last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> Utils::split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

std::string Utils::join(const std::vector<std::string> &vec, const std::string &delimiter) {
    std::ostringstream oss;
    for (auto i = 0; i < vec.size(); ++i) {
        if (i != 0) oss << delimiter;
        oss << vec[i];
    }
    return oss.str();
}

std::string Utils::getStatusColor(double score, double required) {
    if (score >= required * 1.2) return GREEN;
    if (score >= required) return YELLOW;
    return RED;
}

int Utils::getIntInput(const std::string &prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << BOLD << prompt << RESET;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError("Invalid input. Please enter a number.");
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (value < min || value > max) {
            printError("Please enter a number between " + std::to_string(min) +
                      " and " + std::to_string(max));
            continue;
        }
        return value;
    }
}

std::string Utils::getStringInput(const std::string &prompt) {
    std::string input;
    std::cout << BOLD << prompt << RESET;
    std::getline(std::cin, input);
    return trim(input);
}

double Utils::getDoubleInput(const std::string &prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << BOLD << prompt << RESET;
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError("Invalid input. Please enter a number.");
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (value < min || value > max) {
            printError("Please enter a number between " + std::to_string(min) +
                      " and " + std::to_string(max));
            continue;
        }
        return value;
    }
}

void Utils::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Utils::pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
