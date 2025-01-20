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
  int xOffset;
  int yOffset;
  int borderPadding;

public:
  Toolbar(int _borderPadding, int _xOffset, int _yOffset, strvec _leftItems, strvec _rightItems) {
    this->leftItems = _leftItems;
    this->rightItems = _rightItems;
    this->xOffset = _xOffset;
    this->yOffset = _yOffset;
    this-> borderPadding = _borderPadding;
  }
  void printToolbar(WINDOW *win, int yMax, int xMax) {

    int last_x = borderPadding;
    if (leftItems.size() == 0){
        return;
    }
    for (const std::string item : leftItems) {
      mvwprintw(win, yOffset, last_x, "%s", item.c_str());
      last_x += item.length() + xOffset;
    }
    last_x = xMax - borderPadding;
    if (rightItems.size() == 0){
        return;
    }
    for (const std::string item : rightItems) {
      mvwprintw(win, yOffset, last_x - item.length(), "%s", item.c_str());
      last_x -= (item.length() + xOffset);
    }
    wrefresh(win);
  }
};

std::vector<std::string> getDirectoryEntriesSorted(std::string filepath) {
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

  std::vector<std::string> leftItems = {"Directory"};
  std::vector<std::string> rightItems = {"v0.0.1", "nfile"};

  initscr();

  Toolbar tb(2, 2, 0, leftItems, rightItems);

  getmaxyx(stdscr, yMax, xMax);

  std::vector<std::string> dirFiles;

  curs_set(0);

  keypad(stdscr, TRUE);
  noecho();

  box(stdscr, 0, 0);
  refresh();
  tb.printToolbar(stdscr, yMax, xMax);

  int ch;
  while ((ch = wgetch(stdscr)) != 'q' && ch != 'Q') {
    continue;
  }
  endwin();
}

int main(int argc, char *argv[]) {
  std::string filepath = ".";

  if (argc > 1) {
    filepath = argv[1];
  } else {
    std::cout << "No arg. defaulting to '.'\n";
  }

  displayDirectory(filepath);
}
