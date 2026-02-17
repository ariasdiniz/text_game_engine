#include "demo.h"
#include <stdio.h>
#include <string.h>

static void initialize_player() {
  char buffer[MAX_NAME_LEN];
  printf("What is your character's name?\n");
  fgets(player->name, MAX_NAME_LEN, stdin);
  tge_trim(player->name);
  printf("What are their pronouns?\n1: He/Him   2: She/Her   Other: They/Them\n");
  fgets(buffer, MAX_NAME_LEN, stdin);
  tge_trim(buffer);
  if (tge_word_compare(buffer, "1") == 0) {
    strncpy(player->pronouns[0], "he", 2);
    strncpy(player->pronouns[1], "him", 3);
  } else if (tge_word_compare(buffer, "2") == 0) {
    strncpy(player->pronouns[0], "she", 3);
    strncpy(player->pronouns[1], "her", 3);
  } else {
    strncpy(player->pronouns[0], "they", 4);
    strncpy(player->pronouns[1], "them", 4);
  }
  printf("%s: %s/%s\n", player->name, player->pronouns[0], player->pronouns[1]);
  printf("HP: %d/%d\n", player->current_health, player->max_health);
}

void initialize() {
  tge_malloc();
  initialize_player();
}

void finish() {
  tge_free();
}
