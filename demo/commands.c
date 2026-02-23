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
#include "../tge.h"
#include <string.h>

#define NWORDS 16

static void malloc_special_words() {
  special_words = malloc(sizeof(tge_command_special_words));
  special_words->ind_objs = malloc(sizeof(char *) * NWORDS);
  special_words->nouns = malloc(sizeof(char *) * NWORDS);
  special_words->preps = malloc(sizeof(char *) * NWORDS);
  special_words->verbs = malloc(sizeof(char *) * NWORDS);
  for (int i = 0; i < NWORDS; i++) {
    special_words->ind_objs[i] = malloc(sizeof(char) * MAX_NAME_LEN);
    special_words->verbs[i] = malloc(sizeof(char) * MAX_NAME_LEN);
    special_words->preps[i] = malloc(sizeof(char) * MAX_NAME_LEN);
    special_words->nouns[i] = malloc(sizeof(char) * MAX_NAME_LEN);
  }
}

void free_special_words() {
  for (int i = 0; i < NWORDS; i++) {
    free(special_words->ind_objs[i]);
    free(special_words->nouns[i]);
    free(special_words->preps[i]);
    free(special_words->verbs[i]);
  }
  free(special_words->ind_objs);
  free(special_words->verbs);
  free(special_words->nouns);
  free(special_words->preps);
  free(special_words);
}

void setup_verbs() {
  strcpy(special_words->verbs[0], "describe");
  strcpy(special_words->verbs[1], "look");
  strcpy(special_words->verbs[2], "go");
  strcpy(special_words->verbs[3], "walk");
  strcpy(special_words->verbs[4], "run");
  strcpy(special_words->verbs[5], "lit");
  strcpy(special_words->verbs[6], "take");
  strcpy(special_words->verbs[7], "put");
  strcpy(special_words->verbs[8], "pick");
  strcpy(special_words->verbs[9], "grab");
  strcpy(special_words->verbs[10], "hold");
  strcpy(special_words->verbs[11], "use");
  strcpy(special_words->verbs[12], "get");
  strcpy(special_words->verbs[13], "unlock");
  strcpy(special_words->verbs[14], "open");
  strcpy(special_words->verbs[15], "light");
  special_words->verbs_size = 16;
}

void setup_nouns() {
  strcpy(special_words->nouns[0], "around");
  strcpy(special_words->nouns[1], "room");
  strcpy(special_words->nouns[2], "north");
  strcpy(special_words->nouns[3], "west");
  strcpy(special_words->nouns[4], "south");
  strcpy(special_words->nouns[5], "east");
  strcpy(special_words->nouns[6], "torch");
  strcpy(special_words->nouns[7], "key");
  strcpy(special_words->nouns[8], "dagger");
  special_words->nouns_size = 9; 
}

void setup_preps() {
  special_words->preps_size = 0;
}

void setup_ind() {
  strcpy(special_words->ind_objs[0], "brazier");
  strcpy(special_words->ind_objs[1], "door");
  special_words->ind_objs_size = 2;
}

void setup_commands() {
  malloc_special_words();  
  setup_verbs();
  setup_nouns();
  setup_preps();
  setup_ind();
}