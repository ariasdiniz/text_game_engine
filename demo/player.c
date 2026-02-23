/*
Text Game Engine
Copyright (C) 2026 Aria Diniz

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "demo.h"
#include <stdio.h>
#include <string.h>

static void initialize_player() {
  char buffer[MAX_NAME_LEN];
  tge_initialize_player();
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
