#ifndef GAME_FUNCTION_H
#define GAME_FUNCTION_H

#include <stdbool.h>
#include "tools.h"

void generate_room(char (*dungeon)[]);
void show_grid(char (*dungeon)[], pair position);
bool move(char (*dungeon)[], pair *position, int *move_count, bool *game_running);
void bfs(pair pos, char (*dungeon)[], pair (*parent)[], int (*level)[]);
void duel(char (*dungeon)[], Player *player, Final_boss *fb, Enemy *enemies, int *lo_e, int *hi_e, int *kill_count);
void move_enemy(char (*dungeon)[100], pair position, int lo_e, int hi_e, Enemy *enemies, pair (*parent)[100]);
void move_final_boss(char (*dungeon)[], char player_token, Final_boss *fb, pair (*parent)[]);
void spawn_final_boss(char (*dungeon)[], pair position, int lvl, int (*level)[], Final_boss *fb);
void spawn_enemy(char (*dungeon)[100], int (*level)[100], Enemy *enemies, pair position, int lvl, int move_threshold, int *hi_e, int *move_count);


#endif // GAME_FUNCTION_H
