#ifndef LOGIN_H
#define LOGIN_H

#include <stdbool.h>
#include "tools.h"

int login_player(Player *player);
bool player_exists(char *user_name);
bool register_player(Player *player);
bool update_player(Player *player);

#endif // LOGIN_H
