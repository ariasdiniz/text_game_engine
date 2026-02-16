#ifndef __LIB_TGE_HEADER_
#define __LIB_TGE_HEADER_

#include <stdlib.h>

typedef void *tge_action_func(
  char *verb,
  char *noun,
  char *prep,
  char *ind_obj
);

typedef struct tge_command {
  char *verb;
  char *noun;
  char *preposition;
  char *indirect_obj;
  void *action_func;
} tge_command;

typedef struct tge_room {
  char *description;
  char **items;
  struct tge_room **adjacent_rooms;
} tge_room;

typedef struct tge_player {
  char *name;
  char **pronouns;
  char **items;
  char **status;
  int max_health;
  int current_health;
  struct tge_room *current_room;
} tge_player;

void run_action(char *restrict command, tge_action_func action);
void tge_trim(char *restrict str);
unsigned int word_compare(char *restrict fist, char *restrict second);

#endif