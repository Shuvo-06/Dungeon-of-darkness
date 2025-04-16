#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <conio.h>

#include "tools.h"
#include "login.h"
#include "game_function.h"

extern const pair moves[];
bool level_up = false;

Player main_game(Player player) {
    char dungeon[100][100];
    int level[100][100];
    pair parent[100][100];
    Enemy enemies[100];
    Final_boss final_boss = final_boss_at_level(player.lvl);
    int move_count = 0, kill_count = 0, lo_e = 0, hi_e = 0, move_threshold = 5 + 2 * (10 - player.lvl);
    bool game_running = true, final_boss_spawned = false, final_boss_killed = false;

    generate_room(dungeon);
    dungeon[player.pos.first][player.pos.second] = player.token;

    while (game_running && !final_boss_killed) {
        clear_screen();
        printf("Player health - %d\n", player.health);
        printf("Player attack - %d\n", player.attack);
        printf("Player defense - %d\n", player.defense);
        show_grid(dungeon, player.pos);
        bool bfs_done = false;

        if (move(dungeon, &player.pos, &move_count, &game_running)) {
            bfs(player.pos, dungeon, parent, level);
            bfs_done = true;
            if (final_boss.pos.first != -1 && final_boss.pos.second != -1) {
                move_final_boss(dungeon, player.token, &final_boss, parent);
            }
            if (hi_e - lo_e > 0){
                move_enemy(dungeon, player.pos, lo_e, hi_e, enemies, parent);
                duel(dungeon, &player, &final_boss, enemies, &lo_e, &hi_e, &kill_count);
                if (player.health <= 0) {
                    printf("You have lost this level as you have run out of health. Better luck next time.\n");
                    game_running = false;
                }
                if (final_boss.health <= 0) {
                    final_boss_killed = true;
                    final_boss.pos.first = -1;
                    final_boss.pos.second = -1;
                }
            }
        }

        if (move_count >= move_threshold && !final_boss_killed) {
            if (!bfs_done) bfs(player.pos, dungeon, parent, level);
            if (kill_count >= 10 + player.lvl && !final_boss_spawned) {
                spawn_final_boss(dungeon, player.pos, player.lvl, level, &final_boss);
                if (final_boss.pos.first != -1 && final_boss.pos.second != -1) final_boss_spawned = true;
            }
            move_count = 0;
            spawn_enemy(dungeon, level, enemies, player.pos, player.lvl, move_threshold, &hi_e, &move_count);
        }

        if (final_boss_killed) {
            printf("You have successfully completed the level!!!\n");
            player.lvl++;
            player.health += 10;
            player.attack += 2;
            player.defense += 1;
            level_up = true;
        }
        bool no_move_possible = true;
        for (int i = 0; i < 4; i++) {
            int dx = player.pos.first + moves[i].first, dy = player.pos.second + moves[i].second;
            if (dx >= 0 && dx < 100 && dy >= 0 && dy < 100) {
                if (dungeon[dx][dy] == '.') no_move_possible = false;
            }
        }
        if (no_move_possible) game_running = false;
    }
    player.pos.first = 5;
    player.pos.second = 10;
    return player;
}


void homepage(Player *player, bool *game_running) {
    clear_screen();
    read_entire_file("dungeon_render1.txt");
    int option = menu(7, "Start Game", "How to Play", "Store", "Player Stats", "Leaderboard", "Credits", "Exit");

    clear_screen();
    read_entire_file("dungeon_render1.txt");
    printf("\n\n\n");
    switch (option) {
        case 0 :
            Player temp = *player;
            *player = main_game(*player);
            if (level_up) update_player(player);
            else *player = temp;
            printf("\n\nPress any key to exit.");
            getch();
            break;
        case 1 :
            read_entire_file("how_to_play.txt");
            printf("\n\nPress any key to exit.");
            getch();
            break;
        case 2 :
            store(player);
            break;
        case 3 :
            player_stats(player);
            printf("\n\nPress any key to exit.");
            getch();
            break;
        case 4 :
            leaderboard();
            printf("\n\nPress any key to exit.");
            getch();
            break;
        case 5 :
            read_entire_file("credits.txt");
            printf("\n\nPress any key to exit.");
            getch();
            break;
        case 6 :
            exit(0);
    }
}

Player login() {
    Player player;
    while (1) {
        clear_screen();
        read_entire_file("Dungeon_render1.txt");
        int selected_op = menu(3, "Login", "Register", "Exit");
        if (selected_op == 2) exit(0);

        printf("Enter username : ");
        scanf("%s", player.name);
        printf("Enter password : ");
        read_password(player.password);

        if (selected_op == 0) {
            if (login_player(&player)) {
                printf("\n\nLogin successful! Welcome, %s!\n", player.name);
                return player;
            }
            else printf("\n\nInvalid username or password.\n");
            Sleep(SLEEP_TIMER);
        }
        else {
            if (register_player(&player)) {
                printf("\n\nRegistration successful!\n");
                return player;
            }
        }
    }
    printf("Welcome to the arena. \n");
    Sleep(SLEEP_TIMER);
    return player;
}

int main() {
    srand(time(NULL));
    Player player = login();
    bool game_running = true;
    while (game_running) homepage(&player, &game_running);
    read_entire_file("users.data");
    return 0;
}
