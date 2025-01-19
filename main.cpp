#include <iostream>
#include <ncurses.h>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *ent;
    int count = 0;
    int yMax, xMax;


    initscr();
    getmaxyx(stdscr, yMax, xMax);

    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    WINDOW *win = newwin(yMax/2, xMax/2, yMax/4, xMax/4);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, "Directory\n");
    if ((d = opendir(".")) == nullptr){
        std::cerr << "Could not open directory!";
        return 1;
    }
    while ((ent = readdir(d)) != nullptr){
        count += 1;
        wprintw(win, "%i. %s\n", count, ent->d_name);
    }




    int ch;
    while ((ch = wgetch(win)) != 'q' && ch != 'Q') {
        continue;
    }
    endwin();
}
