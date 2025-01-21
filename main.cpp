#include "pageSystem/pageSystem.h"
#include <algorithm>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>
#include <vector>

class Toolbar {
  using strvec = std::vector<std::string>;
  WINDOW *win;
  strvec leftItems;
  strvec rightItems;
  int xOffset;
  int yOffset;
  int borderPadding;

public:
  Toolbar(WINDOW *_win, int _borderPadding, int _xOffset, int _yOffset,
          strvec _leftItems, strvec _rightItems) {
    this->win = _win;
    this->leftItems = _leftItems;
    this->rightItems = _rightItems;
    this->xOffset = _xOffset;
    this->yOffset = _yOffset;
    this->borderPadding = _borderPadding;
  }
  void printToolbar(int yMax, int xMax) {

    int last_x = xMax - borderPadding;
    if (rightItems.size() == 0) {
      return;
    }
    for (const std::string item : rightItems) {
      mvwprintw(win, yOffset, last_x - item.length() - 2, " %s ", item.c_str());
      last_x -= (item.length() + xOffset);
    }
    last_x = borderPadding;

    wrefresh(win);
    if (leftItems.size() == 0) {
      return;
    }
    for (const std::string item : leftItems) {
      mvwprintw(win, yOffset, last_x, " %s ", item.c_str());
      last_x += item.length() + xOffset;
    }
  }
};

PageSystem::directoryFiles_t getDirectoryEntriesSorted(std::string filepath) {
  PageSystem::directoryFiles_t dirFiles;
  for (const auto &entry : std::filesystem::directory_iterator(filepath)) {
    if (entry.path().filename().string() != "." ||
        entry.path().filename().string() != "..")
      dirFiles.push_back(
          std::make_pair(entry.path().filename().string(), entry));
  }
  sort(dirFiles.begin(), dirFiles.end());
  return dirFiles;
}

void displayDirectory(const std::string filepath) {
  int selectedOption = 0;
  int yMax, xMax;

  std::vector<std::string> leftItems = {"Directory"};
  std::vector<std::string> rightItems = {"v0.0.1",
                                         "ncurses - a file sorter in ncurses"};

  PageSystem::directoryFiles_t files = getDirectoryEntriesSorted(filepath);

  initscr();

  resizeterm(0, 0);
  /* vars for PageInfo
  int itemsPerPage;
  int currentPage;
  */
  PageSystem::PageInfo Pages(10, 0);
  Toolbar tb(stdscr, 2, 3, 0, leftItems, rightItems);
  PageSystem::directoryFiles_t currPageFiles = Pages.getPageItems(files);
  int count = 0;
  int offset = 2;

  for (const auto &ent : currPageFiles) {
    mvwprintw(stdscr, offset, 4, "%i. %s", count, ent.first.c_str());
    offset += 2;
    count += 1;
  }

  getmaxyx(stdscr, yMax, xMax);

  std::vector<std::string> dirFiles;

  curs_set(0);

  keypad(stdscr, TRUE);
  noecho();

  box(stdscr, 0, 0);
  refresh();
  tb.printToolbar(yMax, xMax);

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
