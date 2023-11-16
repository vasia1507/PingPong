// Copyright 2022 <Simpsons>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<ncurses.h>
#include<time.h>

void render_frame(int x, int y, int first_racket, int second_racket, int score1, int score2) {
    int i = 0, j = 0, w = 80, h = 25;
    clear();
    printw("----------------------------------------");
    printw("----------------------------------------\n");
    for (i = 0; i < h - 2; i++) {
        printw("|");
        for (j = 0; j < w - 2; j++) {
            if (i == y && j == x) {
                printw("*");
            } else if (j == 40 ||
                (j == 0 && i >= first_racket - 1 && i <= first_racket + 1) ||
                (j == 77 && i >= second_racket - 1 && i <= second_racket + 1)) {
                printw("|");
            } else if (i == 2 && j == 38) {
                if (score1 / 10 == 0) {
                    printw(" %d", score1);
                } else {
                    printw("%d", score1);
                }
                j++;
            } else if (i == 2 && j == 41) {
                if (score2  / 10 == 0) {
                    printw("%d ", score2);
                } else {
                    printw("%d", score2);
                }
                j++;
            } else {
                printw(" ");
            }
        }
        printw("|\n");
    }
    printw("----------------------------------------");
    printw("----------------------------------------\n");
    refresh();
}

int main() {
    unsigned int seed = time(0);
    int score1 = 0, score2 = 0, first_racket = 12, second_racket = 12, speed = 100000;
    double x = 0., y = 0., x0 = 40., y0 = 12.;
    double put = 0., vectorX = 3., vectorY = 1.;
    char key = '0';
    initscr();
    clear();
    cbreak();
    WINDOW *menu_win;
    menu_win = newwin(25, 81, 25, 0);
    nodelay(menu_win, TRUE);
    keypad(menu_win, TRUE);
    refresh();
    //system("stty -icanon");
    while (1) {
        key = wgetch(menu_win);
        if (key == 'z' && first_racket <= 20) first_racket++;
        if (key == 'a' && first_racket >= 2) first_racket--;
        if (key == 'm' && second_racket <= 20) second_racket++;
        if (key == 'k' && second_racket >= 2) second_racket--;
        if (key == 'q') break;
        key = '0';
        put = put + 2;
        if (vectorX > 0) {
            x = x0 + put * cos(atan(fabs(vectorY / vectorX)));
        } else {
            x = x0 - put * cos(atan(fabs(vectorY / vectorX)));
        }
        if (vectorY > 0) {
            y = y0 + put * sin(atan(fabs(vectorY / vectorX)));
        } else {
            y = y0 - put * sin(atan(fabs(vectorY / vectorX)));
        }
        if (x > 76.) {
            if ((int)y >= second_racket - 1 && (int)y <= second_racket + 1) {
                vectorX = -vectorX;
                x = 76.;
                x0 = 76.;
                y0 = y;
                put = 0.;
            } else {
                vectorY = rand_r(&seed) % 5 + 1;
                vectorX = -(vectorY + rand_r(&seed) % 5 + 1);
                x = 40.;
                y = 12.;
                x0 = 40.;
                y0 = 12.;
                score1++;
                put = 0.;
            }
        }
        if (x < 1.) {
            if ((int)y >= first_racket - 1 && (int)y <= first_racket + 1) {
                vectorX = -vectorX;
                x = 1.;
                x0 = 1.;
                y0 = y;
                put = 0.;
            } else {
                vectorY = rand_r(&seed) % 5 + 1;
                vectorX = vectorY + rand_r(&seed) % 5 + 1;
                x = 40.;
                y = 12.;
                x0 = 40.;
                y0 = 12.;
                score2++;
                put = 0.;
            }
        }
        if (y > 22.) {
            vectorY = -vectorY;
            y = 22.;
            x0 = x;
            y0 = 22.;
            put = 0.;
        }
        if (y < 0.) {
            vectorY = -vectorY;
            y = 1.;
            x0 = x;
            y0 = 1.;
            put = 0.;
        }
        render_frame((int)x, (int)y, first_racket, second_racket, score1, score2);
        if (score1 > 20 || score2 > 20) {
            break;
        }
        usleep(speed);
    }
    if (score1 > 20) {
        printw("\nplayer 2 win\n");
    } else {
        printw("\nplayer 1 win\n");
    }
    refresh();
    clrtoeol();
    endwin();
    return 0;
}
