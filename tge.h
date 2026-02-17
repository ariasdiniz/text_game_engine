#ifndef __LIB_TGE_HEADER_
#define __LIB_TGE_HEADER_

#include <stdlib.h>

#define MAX_DESC_LEN 2048
#define MAX_SMAL_DESC_LEN 64
#define MAX_INVENTORY_SIZE 32
#define MAX_NAME_LEN 16

typedef struct tge_command {
  char *verb;
  char *noun;
  char *prep;
  char *ind_obj;
} tge_command;

typedef struct tge_command_special_words {
  char **verbs;
  unsigned int verbs_size;
  char **nouns;
  unsigned int nouns_size;
  char **preps;
  unsigned int preps_size;
  char **ind_objs;
  unsigned int ind_objs_size;
} tge_command_special_words;

typedef void tge_action_func(tge_command *cmd);

typedef struct tge_room {
  char description[MAX_DESC_LEN];
  char small_description[MAX_SMAL_DESC_LEN];
  char items[MAX_INVENTORY_SIZE][MAX_NAME_LEN];
  struct tge_room **adjacent_rooms;
} tge_room;

typedef struct tge_player {
  char name[MAX_NAME_LEN];
  char pronouns[2][MAX_NAME_LEN];
  char items[MAX_INVENTORY_SIZE][MAX_NAME_LEN];
  char status[MAX_INVENTORY_SIZE][MAX_NAME_LEN];
  int max_health;
  int current_health;
  struct tge_room *current_room;
} tge_player;

#define TGE_MAXLEN 128
#define TGE_MAXWORDS 32

void run_action(
  char *restrict unparsed_command,
  tge_command_special_words *ctx,
  tge_action_func action
);

void tge_trim(char *restrict str);
void tge_malloc();
void tge_free();

unsigned int tge_word_compare(char *restrict fist, char *restrict second);
unsigned int tge_parse_command_array(char *restrict command, char **restrict command_array);

extern tge_player *player;
extern tge_room *tge_current_room;
extern char **tge_parsed_commands;
extern tge_command *tge_structured_commands;

#endif