#include "pageSystem/pageSystem.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
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
  dirFiles.push_back(std::make_pair("..", std::filesystem::directory_entry(std::filesystem::path(".."))));
  for (const auto &entry : std::filesystem::directory_iterator(filepath)) {
    if (std::filesystem::is_directory(entry)){
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
  clear();
  std::filesystem::current_path(filepath);
  std::string curr = std::filesystem::current_path().string();
  int sz = curr.size();
  if (curr.size() >= 60) {
    curr = "..." + curr.substr(curr.size() - ((curr.size() - 3) / 2) - 10);
  }
  std::vector<std::string> leftItems = {"Directory", curr};
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
  PageSystem::PageInfo Pages(20, 0);
  Toolbar stdscr_toolbar(stdscr, 2, 3, 0, leftItems, rightItems);
  PageSystem::directoryFiles_t currPageFiles = Pages.getPageItems(files);
  int currentItem = 0;
  int count = 0;
  int offset = 2;

  // for (const auto &ent : currPageFiles) {
  //   mvwprintw(stdscr, offset, 4, "%i. %s/", count, ent.first.c_str());
  //   offset += 1;
  //   count += 1;
  // }

  std::vector<std::string> dirFiles;

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);  // Highlighted text (black on white)
  init_pair(2, COLOR_WHITE, COLOR_BLACK);

  curs_set(0);
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  noecho();

  box(stdscr, 0, 0);
  refresh();
  stdscr_toolbar.printToolbar(yMax, xMax);



  int ch;

  while ((ch = wgetch(stdscr)) != 'q' && ch != 'Q') {
      // ------
      for (int _i = 0; _i < currPageFiles.size(); ++_i) {
                  if (_i == currentItem) {
                      // Apply the highlighted color pair
                      attron(COLOR_PAIR(1));
                  } else {
                      // Apply the normal color pair
                      attron(COLOR_PAIR(2));
                  }

                  mvwprintw(stdscr, offset + _i, 4, "%s/", currPageFiles.at(_i).first.c_str());

                  // Turn off the attributes after drawing the line
                  attroff(COLOR_PAIR(1));
                  attroff(COLOR_PAIR(2));
              }

              // Refresh the screen to show the updated drawing
              refresh();
        // -----
    switch (ch) {
    case KEY_UP:
      if (currentItem - 1 >= 0){
          currentItem -= 1;
      } else {
          if (Pages.currentPage >= 1){
            Pages.currentPage -= 1;
            currPageFiles = Pages.getPageItems(files);
            currentItem = currPageFiles.size() - 1;
          }
      }
      break;
    case KEY_DOWN:
        if (currentItem + 1 != currPageFiles.size()){
            currentItem += 1;
        } else {
            if (Pages.currentPage + 1 <= Pages.totalPages){
                Pages.currentPage -= 1;
                currPageFiles = Pages.getPageItems(files);
                currentItem = 0;
            }
        }
        break;
    case 10:
        displayDirectory("./" + currPageFiles.at(currentItem).first);
      break;
    default:
        refresh();
      break;

    }

  }
  endwin();
  exit(0);
}

int main(int argc, char *argv[]) {
  std::string filepath = ".";

  if (argc > 1) {
    filepath = argv[1];
  }
  // } else {
  //   std::cout << "No arg. defaulting to '.'\n";
  // }
  displayDirectory(filepath);
}
