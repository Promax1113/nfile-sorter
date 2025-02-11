#include "pageSystem/pageSystem.h"
#include <algorithm>
#include <cstdio>
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

    for (const std::string item : rightItems) {
      mvwprintw(win, yOffset, last_x - item.length() - 2, " %s ", item.c_str());
      last_x -= (item.length() + xOffset);
    }
    last_x = borderPadding;

    for (const std::string item : leftItems) {
      mvwprintw(win, yOffset, last_x, " %s ", item.c_str());
      last_x += item.length() + xOffset;
    }
  }
};

PageSystem::directoryFiles_t getDirectoryEntriesSorted() {
  std::string filepath = std::filesystem::current_path().string();
  PageSystem::directoryFiles_t dirFiles;
  for (const auto &entry : std::filesystem::directory_iterator(filepath)) {
    if (std::filesystem::is_directory(entry) ||
        entry.path().filename().string() == "..") {
      wprintw(stdscr, "%s", entry.path().filename().c_str());
      dirFiles.push_back(
          std::make_pair(entry.path().filename().string(), entry));
    }
  }
  sort(dirFiles.begin(), dirFiles.end());
  return dirFiles;
}

void displayDirectory(const std::string filepath) {
  int selectedOption = 0;
  int yMax, xMax;

  std::filesystem::current_path(filepath);
  std::string curr = std::filesystem::current_path().string();
  std::vector<std::string> leftItems = {
      "Directory", std::filesystem::current_path().string()};
  std::vector<std::string> rightItems = {"v0.0.1", "nfile"};

  PageSystem::directoryFiles_t files = getDirectoryEntriesSorted();

  initscr();
  use_env(TRUE);
  refresh();
  getmaxyx(stdscr, yMax, xMax);

  resizeterm(0, 0);
  /* vars for PageInfo
  int itemsPerPage;
  int currentPage;
  */
  PageSystem::PageInfo Pages(10, 0);
  Toolbar stdscr_toolbar(stdscr, 2, 3, 0, leftItems, rightItems);
  PageSystem::directoryFiles_t currPageFiles = Pages.getPageItems(files);
  int currentItem = 0;
  int count = 0;
  int offset = 2;

  for (const auto &ent : currPageFiles) {
    mvwprintw(stdscr, offset, 4, "%i. %s/", count, ent.first.c_str());
    offset += 1;
    count += 1;
  }

  std::vector<std::string> dirFiles;

  curs_set(0);

  keypad(stdscr, TRUE);
  noecho();

  box(stdscr, 0, 0);
  refresh();
  stdscr_toolbar.printToolbar(yMax, xMax);

  int ch;

  while ((ch = wgetch(stdscr)) != 'q' && ch != 'Q') {

    switch (ch) {
    case KEY_UP:
      if (currentItem + 1 == currPageFiles.size())
        currentItem -= 1;
    case KEY_DOWN:
      currentItem += 1;
    case KEY_ENTER:
      if (currPageFiles.at(currentItem).first == "..") {
        displayDirectory(filepath + "/..");
      }
    default:
      continue;
    }
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
