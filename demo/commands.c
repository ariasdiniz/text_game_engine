#include "demo.h"
#include "../tge.h"
#include <string.h>

#define NWORDS 8

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

void setup_commands() {
  malloc_special_words();
  special_words->nouns_size = 0;
  special_words->preps_size = 0;
  special_words->verbs_size = 0;
  special_words->ind_objs_size = 0;
}