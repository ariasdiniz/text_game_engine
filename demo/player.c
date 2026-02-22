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
    strncpy(player->pronouns[0], "he", 5);
    strncpy(player->pronouns[1], "him", 5);
  } else if (tge_word_compare(buffer, "2") == 0) {
    strncpy(player->pronouns[0], "she", 5);
    strncpy(player->pronouns[1], "her", 5);
  } else {
    strncpy(player->pronouns[0], "they", 5);
    strncpy(player->pronouns[1], "them", 5);
  }
  printf("\n\n");
}

void initialize() {
  tge_malloc();
  initialize_player();
}

void finish() {
  tge_free();
}
