#include <stdio.h>
#include <time.h>
#include <ctype.h>
#define NOMAXMIN
#include <windows.h>
#include "game_function.h"
#include "tools.h"

extern const pair moves[] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};

void generate_room(char (*dungeon)[100]) {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (i == 0 || j == 0 || i == 99 || j == 99) dungeon[i][j] = '#'; //boundary wall
            else if (i % 20 == 0 || j % 20 == 0) {
                if (i % 10 == 0 && i % 20 != 0) { // y - axis door
                    dungeon[i - 2][j] = '.';
                    dungeon[i - 1][j] = '.';
                    dungeon[i][j] = '.';
                }
                else if (j % 10 == 0 && j % 20 != 0) { // x - axis door
                    dungeon[i][j - 2] = '.';
                    dungeon[i][j - 1] = '.';
                    dungeon[i][j] = '.';
                }
                else dungeon[i][j] = '#';
            }
            else dungeon[i][j] = '.';
        }
    }
}

void show_grid(char (*dungeon)[100], pair position) {
    int top_i = position.first - WIDTH / 2; //user view shifting x axis
    if (top_i < 0) top_i = 0;
    int bottom_i = top_i + WIDTH;
    if (bottom_i > 100) {
        int temp = bottom_i - 100;
        top_i -= temp;
        bottom_i -= temp;
    }

    int left_j = position.second - HEIGHT / 2; //user view shifting y axis
    if (left_j < 0) left_j = 0;
    int right_j = left_j + HEIGHT;
    if (right_j > 100) {
        int temp = right_j - 100;
        left_j -= temp;
        right_j -= temp;
    }

    for (int i = top_i; i < bottom_i; i++) {
        for (int j = left_j; j < right_j; j++) {
            printf("%c", dungeon[i][j]);
        }
        printf("\n");
    }
}

bool move(char (*dungeon)[100], pair *position, int *move_count, bool *game_running) {
    bool move_done = false;
    int x = position->first, y = position->second, dx = 0, dy = 0;

    printf(GREEN "Enter your move (W/A/S/D): \n" RESET);

    switch (_getch()) {
        case 'w':
            dx = -1;
            break;
        case 'a':
            dy = -1;
            break;
        case 's':
            dx = 1;
            break;
        case 'd':
            dy = 1;
            break;
        case 27:
            clear_screen();
            printf(CYAN "Are you sure you want to quit? \nYou haven't completed the level yet. Progress won't be saved. (y/n)\n" RESET);
            while (true) {
                char confirmation = tolower(getch());
                if (confirmation == 'y') {
                    *game_running = false;
                    return false;
                }
                else if (confirmation == 'n') return false;
                else {
                    printf(RED "Invalid input\n" RESET);
                    Sleep(SLEEP_TIMER);
                }
            }
            break;
        default:
            printf(RED "Invalid move\n" RESET);
            Sleep(SLEEP_TIMER);
            return false;
    }

    // Invalid move logic
    int new_x = x + dx, new_y = y + dy;
    if (new_x >= 0 && new_x < 100 && new_y >= 0 && new_y < 100) {
        if (dungeon[new_x][new_y] == '.') {
            char temp = dungeon[x][y];
            dungeon[x][y] = dungeon[new_x][new_y];
            dungeon[new_x][new_y] = temp;

            position -> first = new_x;
            position -> second = new_y;

            (*move_count)++;
            move_done = true;
        }
        else {
            printf(RED "Invalid move\n" RESET);
            Sleep(SLEEP_TIMER);
        }
    }
    else {
        printf(RED "Invalid move\n" RESET);
        Sleep(SLEEP_TIMER);
    }
    return move_done;
}

// breadth first search
// used to generate parent array, level array according to current player position
void bfs(pair pos, char (*dungeon)[100], pair (*parent)[100], int (*level)[100]) {
    bool vis[100][100];
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            level[i][j] = -1;
            parent[i][j] = (pair){-1, -1};

            if (dungeon[i][j] == '#') vis[i][j] = true;
            else vis[i][j] = false;
        }
    }

    pair queue[10000];
    queue[0] = pos;
    int lo_q = 0, hi_q = 1;
    parent[pos.first][pos.second] = (pair){-1, -1};
    level[pos.first][pos.second] = 0;
    vis[pos.first][pos.second] = true;
    while (hi_q - lo_q) {
        pair source = queue[lo_q];
        lo_q++;
        for (int i = 0; i < 4; i++) {
            int dx = source.first + moves[i].first;
            int dy = source.second + moves[i].second;
            if (dx >= 0 && dx < 100 && dy >= 0 && dy < 100) {
                if (!vis[dx][dy]) {
                    level[dx][dy] = level[source.first][source.second] + 1;
                    parent[dx][dy] = source;
                    queue[hi_q] = (pair){dx, dy};
                    hi_q++;
                    vis[dx][dy] = true;
                }
            }
        }
    }
}


void duel(char (*dungeon)[100], Player *player, Final_boss *fb, Enemy *enemies, int *lo_e, int *hi_e, int *kill_count) {
    Enemy enemies2[100];
    bool dueled = false;
    int ind = 0;

    if (fb->pos.first != -1 && fb->pos.second != -1) {
        printf("Final boss - ");
        if (abs(fb->pos.first - player->pos.first) + abs(fb->pos.second - player->pos.second) <= ATTACK_DIST) {
            int temp = max(0, player -> attack - fb -> defense);
            if (temp == 0) temp = (player -> attack) * random(0, 20) / 100;
            else temp = temp * random(80, 120) / 100;
            fb -> health -= temp;

            temp = max(0, fb -> attack - player -> defense);
            if (temp == 0) temp = (fb -> attack) * random(0, 20) / 100;
            else temp = temp * random(80, 120) / 100;
            player -> health -= temp;
            dueled = true;
        }

        if (fb->health > 0) printf(RED "Retained!!!\n" RESET);
        else {
            printf(GREEN "Killed!!!\n" RESET);
            dungeon[fb->pos.first][fb->pos.second] = '.';
            fb->pos.first = -1;
            fb->pos.second = -1;
        }
        if (player -> health <= 0) return;
    }

    for (int i = *lo_e; i < *hi_e; i++) {
        printf("Enemy no.%d - ", i + 1);
        if (abs(enemies[i].pos.first - player -> pos.first) + abs(enemies[i].pos.second - player -> pos.second) <= ATTACK_DIST) {
            int temp = max(0, player -> attack - enemies[i].defense);
            if (temp == 0) temp = (player -> attack) * random(0, 20) / 100;
            else temp = temp * random(80, 120) / 100;
            enemies[i].health -= temp;

            temp = max(0, enemies[i].attack - player -> defense);
            if (temp == 0) temp = (enemies[i].attack) * random(0, 20) / 100;
            else temp = temp * random(80, 120) / 100;
            player -> health -= temp;
            dueled = true;
        }

        if (enemies[i].health > 0) {
            printf(RED "Retained!!!\n" RESET);
            enemies2[ind++] = enemies[i];
        }
        else {
            printf(GREEN "Killed!!!\n20 XP bonus received\n" RESET);
            dungeon[enemies[i].pos.first][enemies[i].pos.second] = '.';
            (*kill_count)++;
            player->health += 20;

            if (random(0, 10) == 6) {
                int temp = random(0, 2);
                switch (temp % 2) {
                case 0 :
                    player -> attack += player -> attack * random(110, 120) / 100;
                    printf("Enemy weapon dropped, your weapon has been upgraded!!!\n");
                    break;
                case 1 :
                    player -> defense += player -> defense * random(110, 120) / 100;
                    printf("Enemy armor dropped, your armor has been upgraded!!!\n");
                    break;
                }
            }
        }
        if (player -> health <= 0) return;
    }

    if (dueled) Sleep(SLEEP_TIMER);
    for (int i = 0; i < ind; i++) enemies[i] = enemies2[i];
    *hi_e = ind;
}

void move_enemy(char (*dungeon)[100], pair position, int lo_e, int hi_e, Enemy *enemies, pair (*parent)[100]) {
    for (int i = lo_e; i < hi_e; i++) {
        pair curr = enemies[i].pos;
        pair target = parent[curr.first][curr.second];
        if (dungeon[target.first][target.second] == '.' && target.first != -1 && target.second != -1) {
            dungeon[target.first][target.second] = enemies[i].token;
            dungeon[curr.first][curr.second] = '.';
            enemies[i].pos = target;
        }
    }
}

void move_final_boss(char (*dungeon)[100], char player_token, Final_boss *fb, pair (*parent)[100]) {
    pair target = parent[fb -> pos.first][fb -> pos.second];
    if (dungeon[target.first][target.second] == '.' && fb -> pos.first != -1 && fb -> pos.second != -1) {
        dungeon[target.first][target.second] = fb -> token;
        dungeon[fb->pos.first][fb->pos.second] = '.';
        fb->pos = target;
    }
    else {
        if (dungeon[target.first][target.second] != player_token) {
            char temp = dungeon[fb -> pos.first][fb -> pos.second];
            dungeon[fb -> pos.first][fb -> pos.second] = dungeon[target.first][target.second];
            dungeon[target.first][target.second] = temp;
        }
    }
}


void spawn_final_boss(char (*dungeon)[100], pair position, int lvl, int (*level)[100], Final_boss *fb) {
    pair possib[1000];
    int lo_p = 0, hi_p = 0;

    for (int i = max(position.first - ATTACK_DIST, 0); i < min(position.first + ATTACK_DIST, 100); i++) {
        for (int j = max(position.second - ATTACK_DIST, 0); j < min(position.second + ATTACK_DIST, 100); j++) {
            bool possible = true;
            for (int k = 0; k < 4; k++) {
                int dx = i + moves[k].first, dy = j + moves[k].second;
                if (dx >= 0 && dx < 100 && dy >= 0 && dy < 100) {
                    if (dungeon[dx][dy] != '.') {
                        possible = false;
                        break;
                    }
                }
            }
            if (level[i][j] == SPAWN_DIST && possible && dungeon[i][j] == '.') {
                possib[hi_p] = (pair){i, j};
                hi_p++;
            }
        }
    }

    if (hi_p - lo_p > 0) {
        pair in_fb = possib[random(lo_p, hi_p - 1)];
        fb->pos = in_fb;
        dungeon[fb->pos.first][fb->pos.second] = fb->token;
    }
}


void spawn_enemy(char (*dungeon)[100], int (*level)[100], Enemy *enemies, pair position, int lvl, int move_threshold, int *hi_e, int *move_count) {
    pair possib[1000];
    int lo_p = 0, hi_p = 0;

    for (int i = max(position.first - 5, 0); i < min(position.first + 5, 100); i++) {
        for (int j = max(position.second - 5, 0); j < min(position.second + 5, 100); j++) {
            bool possible = true;
            for (int k = 0; k < 4; k++) {
                int dx = i + moves[k].first, dy = j + moves[k].second;
                if (dx >= 0 && dx < 100 && dy >= 0 && dy < 100) {
                    if (dungeon[dx][dy] != '.') {
                        possible = false;
                        break;
                    }
                }
            }
            if (level[i][j] == 5 && possible && dungeon[i][j] == '.') {
                possib[hi_p] = (pair){i, j};
                hi_p++;
            }
        }
    }
    if (hi_p - lo_p >= 0) {
        pair in_enemy = possib[random(lo_p, hi_p - 1)];
        Enemy temp = enemy_at_level(lvl);
        temp.pos = in_enemy;
        enemies[*hi_e] = temp;
        dungeon[in_enemy.first][in_enemy.second] = enemies[*hi_e].token;
        (*hi_e)++;
    }
    else (*move_count) = move_threshold;
}
