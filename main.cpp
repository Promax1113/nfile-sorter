#include "pageSystem/pageSystem.h"
#include <algorithm>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

class Toolbar {
  using strvec = std::vector<std::string>;
  strvec leftItems;
  strvec rightItems;

public:
  Toolbar(strvec _leftItems, strvec _rightItems) {
    this->leftItems = _leftItems;
    this->rightItems = _rightItems;
  }
  void printToolbar(WINDOW *win, int yMax, int xMax) {
    int last_x = 0;
    for (const std::string item : leftItems) {
      mvwprintw(win, yMax, last_x + item.length(), "%s", item.c_str());
      last_x += item.length();
    }
  }
};

std::vector<std::string> getDirectoryFilesSorted(std::string filepath) {
  using namespace std;
  vector<string> dirFiles;
  for (const auto &entry : filesystem::directory_iterator(filepath)) {
    dirFiles.push_back(entry.path().filename().string());
  }
  sort(dirFiles.begin(), dirFiles.end());
  return dirFiles;
}

void displayDirectory(const std::string filepath) {
  int selectedOption = 0;
  int yMax, xMax;

  initscr();
  getmaxyx(stdscr, yMax, xMax);
  std::vector<std::string> dirFiles;

  curs_set(0);

  keypad(stdscr, TRUE);
  noecho();

  box(stdscr, 0, 0);
  mvwprintw(stdscr, 0, 2, "Directory");
  mvwprintw(stdscr, 0, xMax - 14, "nfile-sorter");

  int ch;
  while ((ch = wgetch(stdscr)) != 'q' && ch != 'Q') {
    continue;
  }
  endwin();
}

int main(int argc, char *argv[]) {
  std::string filepath;

  if (argc <= 1) {
    std::cout << "No arg. defaulting to '.'";
  }

  displayDirectory(argv[1]);
}
