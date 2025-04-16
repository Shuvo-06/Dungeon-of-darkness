#include <stdio.h>
#include "tools.h"

void leaderboard() {
    FILE *file = fopen("users.data", "r");
    if (!file) {
        printf(RED "Something went wrong. Please try again later.\n" RESET);
        return;
    }

    Player players[100];
    int count = 0;

    while (fread(&players[count], sizeof(Player), 1, file)) count++;
    fclose(file);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (players[j].lvl < players[j + 1].lvl ||
               (players[j].lvl == players[j + 1].lvl && players[j].health < players[j + 1].health)) {
                Player temp = players[j];
                players[j] = players[j + 1];
                players[j + 1] = temp;
            }
        }
    }

    printf("\n========================= Leaderboard =========================\n");
    printf("%-20s %-10s %-10s %-10s %-10s\n", "Name", "Level", "Health", "Attack", "Defense");
    for (int i = 0; i < count; i++) printf("%-20s %-10d %-10d %-10d %-10d\n", players[i].name, players[i].lvl, players[i].health, players[i].attack, players[i].defense);
    return;
}

void player_stats(Player *player) {
    printf("Username : %s\n", player -> name);
    printf("Level : %d\n", player -> lvl);
    printf("Health : %d\n", player -> health);
    printf("Coin : %d\n", player -> coin);
    printf("Attack : %d\n", player -> attack);
    printf("Defense : %d\n", player -> defense);
    return;
}

void store(Player *player) {
    int selected_op = 8;
    printf("Coin balance : %d\n", player -> coin);
    while (selected_op - 6) {
        int selected_op = menu (7, "\033[1;36mHealth Potion (small)        :\t Restores 20 HP \tcost : 10 gold\033[0m",
                                   "\033[1;36mHealth Potion (big)          :\t Restores 50 HP \tcost : 25 gold\033[0m",
                                   "\033[1;36mWeapons (Iron Sword)         :\t +10 attack     \tcost : 50 gold\033[0m",
                                   "\033[1;36mWeapons (Mystic Bow)         :\t +15 attack     \tcost : 100 gold\033[0m",
                                   "\033[1;36mArmor (Leather armor)        :\t +5 defense     \tcost : 30 gold\033[0m",
                                   "\033[1;36mArmor (Steel armor)          :\t +10 defense    \tcost : 80 gold\033[0m",
                                   "\033[1;31mexit\033[0m\n");
        switch (selected_op) {
            case 0 :
                if ((player -> coin) >= 10) {
                    player -> health += 20;
                    player -> coin -= 10;
                    printf("Congratulation!!! Purchase complete.\n");
                }
                else printf("Not enough gold.\n");
                break;
            case 1 :
                if ((player -> coin) >= 25) {
                    player -> health += 50;
                    player -> coin -= 25;
                    printf("Congratulation!!! Purchase complete.\n");
                }
                else printf("Not enough gold.\n");
                break;
            case 2 :
                if ((player -> coin) >= 50) {
                    player -> attack += 10;
                    player -> coin -= 50;
                    printf("Congratulation!!! Purchase complete.\n");
                }
                else printf("Not enough gold.\n");
                break;
            case 3 :
                if ((player -> coin) >= 100) {
                    player -> health += 15;
                    player -> coin -= 100;
                    printf("Congratulation!!! Purchase complete.\n");
                }
                else printf("Not enough gold.\n");
                break;
            case 4 :
                if ((player -> coin) >= 30) {
                    player -> health += 5;
                    player -> coin -= 30;
                    printf("Congratulation!!! Purchase complete.\n");
                }
                else printf("Not enough gold.\n");
                break;
            case 5 :
                if ((player -> coin) >= 80) {
                    player -> health += 10;
                    player -> coin -= 80;
                    printf("Congratulation!!! Purchase complete.\n");
                }
                else printf("Not enough gold.\n");
                break;
            default : return;
        }
        Sleep(SLEEP_TIMER);
    }
    return;
}
