#include <iostream>
#include <ncurses.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <algorithm>
#include <vector>

int main(int argc, char *argv[]) {

    int count = 0;
    int yMax, xMax;


    initscr();
    getmaxyx(stdscr, yMax, xMax);
    std::vector<std::string> dirFiles;

    curs_set(0);
    WINDOW *win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    keypad(win, TRUE);
    noecho();
    for (const auto& entry : std::filesystem::directory_iterator(".")){
        dirFiles.push_back(entry.path().filename().string());
    }
    std::sort(dirFiles.begin(), dirFiles.end());
    box(win, 0, 0);

    mvwprintw(win, 0, 2, "Directory");


    int ch;
    while ((ch = wgetch(win)) != 'q' && ch != 'Q') {
        continue;
    }
    endwin();
}
