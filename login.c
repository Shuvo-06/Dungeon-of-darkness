#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "tools.h"


int login_player(Player *player) {
    FILE *file = fopen("users.data", "rb");
    if (!file) {
        printf(RED "Something went wrong. Please try again later.\n" RESET);
        return 0;
    }

    Player temp;
    while (fread(&temp, sizeof(Player), 1, file) == 1) {
        if (strcmp(temp.name, player->name) == 0 && strcmp(temp.password, player->password) == 0) {
            *player = temp;
            player->token = PLAYER_TOKEN;
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

bool player_exists(char *user_name) {
    FILE *file = fopen("users.data", "rb");
    if (!file) {
        printf(RED "Something went wrong. Please try again later.\n" RESET);
        return false;
    }

    Player temp;
    while (fread(&temp, sizeof(Player), 1, file) == 1) {
        if (strcmp(temp.name, user_name) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

bool register_player(Player *player) {
    if (player_exists(player->name)) {
        printf(RED "\nPlayer with this user name already exists.\n" RESET);
        Sleep(SLEEP_TIMER);
        return false;
    }

    player->token = PLAYER_TOKEN;
    player->health = 100;
    player->attack = 10;
    player->defense = 5;
    player->coin = 100;
    player->lvl = 1;
    player->pos.first = 5;
    player->pos.second = 10;

    FILE *file = fopen("users.data", "ab");
    if (!file) {
        printf(RED "Something went wrong. Please try again later.\n" RESET);
        return false;
    }

    fwrite(player, sizeof(Player), 1, file);
    fclose(file);
    return true;
}

bool update_player(Player *player) {
    FILE *file = fopen("users.data", "rb+");
    if (!file) {
        printf(RED "Something went wrong. Please try again later.\n" RESET);
        return false;
    }

    Player temp;
    while (fread(&temp, sizeof(Player), 1, file) == 1) {
        if (strcmp(temp.name, player->name) == 0) {
            fseek(file, -sizeof(Player), SEEK_CUR);
            fwrite(player, sizeof(Player), 1, file);
            fclose(file);
            return true;
        }
    }

    fclose(file);
    printf(RED "Player not found.\n" RESET);
    return false;
}
