#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>

#define SLEEP_TIMER 400
#define ATTACK_DIST 3
#define SPAWN_DIST 5
#define HEIGHT 40
#define WIDTH 30
#define MAX_PASSWORD_SIZE 20
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define CYAN  "\x1b[36m"
#define RESET "\x1b[0m"
#define PLAYER_TOKEN 'P'
#define FINAL_BOSS_TOKEN 'E'
#define ENEMY_TOKEN 'T'

typedef struct {
    int first, second;
} pair;

typedef struct {
    char token;
    char name[20], password[20];
    int attack, defense, health, lvl, coin;
    pair pos;
} Player;

typedef struct {
    char token;
    int attack, defense, health;
    pair pos;
} Enemy;

typedef struct {
    char token;
    int attack, defense, health;
    pair pos;
} Final_boss;


void swap(int *a, int *b);
void swapc(char *a, char *b);
int abs(int a);
void clear_screen();
int random(int lo, int hi);
bool read_entire_file(char *s);
void read_password(char *password);

void Menu(int choice, int scWidth, int scHeight, int no_of_options, char *menu[]);
int menu(int no_of_options, ...);

Enemy enemy_at_level(int lvl);
Final_boss final_boss_at_level(int lvl);


#endif // TOOLS_H
