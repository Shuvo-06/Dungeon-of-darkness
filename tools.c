#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include "tools.h"


// used to swap value of two integer variables using pointer
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// used to swap value of two character variables using pointer
void swapc(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

// return absolute value of a integer
int abs(int a) {
    return (a < 0) ? -a : a;
}

// function to clear console
// system() is a built-in function of windows.h, which is used to invoke an operating system command
void clear_screen() {
    system("cls");
}

// returns a random value in [lo, hi] range
// The rand() function is used to generate pseudo-random numbers
int random(int lo, int hi) {
    if (lo > hi) swap(&lo, &hi);
    long long int k = rand() % (hi - lo + 1);
    return (int)k + lo;
}

// function to read whole file contents
bool read_entire_file(char *s) {
    FILE *file = fopen(s, "r");
    if (file == NULL) {
        printf(RED "Something went wrong\n" RESET);
        return false;
    }
    char ch;
    while ((ch = fgetc(file)) != EOF) putchar(ch);
    fclose(file);
    return true;
}

// used to read password from user input while keeping secrecy
void read_password(char *password) {
    int i = 0;
    while (true) {
        char ch = getch();
        if (ch == '\n' || ch == '\r') {
            password[i] = '\0';
            break;
        }
        if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else if (i < MAX_PASSWORD_SIZE - 1) {
            password[i++] = ch;
            printf("*");
        }
    }
}

void Menu(int choice, int scWidth, int scHeight, int no_of_options, char *menu[]) {
    int menuWidth = 20, menuHeight = no_of_options;
    int startRow = (scHeight / 2) - (menuHeight / 2);
    int startCol = (scWidth / 2) - (menuWidth / 2);

    for (int i = 0; i < startRow; i++) printf("\n");
    for (int i = 0; i < no_of_options; i++) {
        for (int j = 0; j < startCol; j++) printf(" ");
        if (strcmp(menu[i], "Exit") == 0) {
            if (i == choice) printf(">> ");
            else printf("   ");

            printf(RED "exit" RESET);

            if (i == choice) printf(" <<");
            printf("\n");
        }
        else {
            if (i == choice) printf(">> ");
            else printf("   ");

            printf(CYAN "%s" RESET, menu[i]);

            if (i == choice) printf(" <<");
            printf("\n");
        }
    }
}


int menu(int no_of_options, ...) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int scWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1, scHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    int choice = 0, key;

    va_list args;
    va_start(args, no_of_options);
    const char **menu_options = malloc(no_of_options * sizeof(const char*));
    if (menu_options == NULL) {
        printf(RED"Free up more memory space!\n" RESET);
        va_end(args);
        return -1;
    }
    for (int i = 0; i < no_of_options; i++) menu_options[i] = va_arg(args, const char*);
    va_end(args);

    while (1) {
        clear_screen();
        read_entire_file("dungeon_render1.txt");
        Menu(choice, scWidth, scHeight, no_of_options, menu_options);
        key = _getch();
        if (key == 224) {
            switch (_getch()) {
                case 72: // Up arrow
                    choice = (choice - 1 + no_of_options) % no_of_options;
                    break;
                case 80: // Down arrow
                    choice = (choice + 1) % no_of_options;
                    break;
            }
        }
        else if (key == 13) return choice;
    }
}

Enemy enemy_at_level(int level) {
    Enemy e1;
    e1.token = ENEMY_TOKEN;
    e1.health = 30 + 10 * level;
    e1.attack = 5 + 2 * level;
    e1.defense = 2 + level;
    e1.pos = (pair){-1, -1};
    return e1;
}

Final_boss final_boss_at_level(int level) {
    Final_boss fb;
    fb.token = FINAL_BOSS_TOKEN;
    fb.health = 150 + 15 * level;
    fb.attack = 10 + 5 * level;
    fb.defense = 5 + level;
    fb.pos = (pair){-1, -1};
    return fb;
}
