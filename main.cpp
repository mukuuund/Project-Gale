#include "include/menu.h"
#include <iostream>
#include <cstdlib>   // for system()

#ifdef _WIN32
  #include <windows.h>
#endif

int main() {
    // Ensure the console is in UTF-8 and ANSI-escape mode on Windows
    #ifdef _WIN32
      // 1) Switch the output code page to UTF-8
      SetConsoleOutputCP(CP_UTF8);

      // 2) Enable ANSI escape sequences (for colors, etc.)
      HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
      if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
          SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
      }
    #endif

    // Create data directory if it doesn't exist
    #ifdef _WIN32
      system("mkdir data 2> nul");
    #endif

    try {
        Menu menu;
        menu.run();
    } catch (const std::exception &e) {
        std::cerr << "An error occurred: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
