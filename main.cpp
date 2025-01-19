#include <iostream>
#include <ncurses.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>
#include <algorithm>
#include <vector>
#include "pageSystem/pageSystem.h"

std::vector<std::string> getDirectoryFilesSorted(std::string filepath){
    using namespace std;

    vector<string> dirFiles;
    for (const auto& entry : filesystem::directory_iterator(filepath)){
        dirFiles.push_back(entry.path().filename().string());
    }
    sort(dirFiles.begin(), dirFiles.end());
    return dirFiles;
}

void displayDirectory(const std::string filepath){
    int selectedOption = 0;
    int yMax, xMax;

    initscr();
    getmaxyx(stdscr, yMax, xMax);
}

int main(int argc, char *argv[]) {

    int selected = 0;
    int yMax, xMax;


    initscr();
    getmaxyx(stdscr, yMax, xMax);
    std::vector<std::string> dirFiles;
    pageSystem::PageInfo main;

    curs_set(0);

    keypad(stdscr, TRUE);
    noecho();

    box(stdscr, 0, 0);

    mvwprintw(stdscr, 0, 2, "Directory");


    int ch;
    while ((ch = wgetch(stdscr)) != 'q' && ch != 'Q') {
        continue;
    }
    endwin();
}
