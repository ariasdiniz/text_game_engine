#ifndef __LIB_TGE_HEADER_
#define __LIB_TGE_HEADER_

#include <stdlib.h>

#define MAX_DESC_LEN 2048
#define MAX_SMAL_DESC_LEN 64
#define MAX_INVENTORY_SIZE 32
#define MAX_NAME_LEN 16

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define TGE_ENOISPACE 300
#define TGE_ENOITEM 301

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
typedef void tge_describe_func();

typedef struct tge_room {
  char items[MAX_INVENTORY_SIZE][MAX_NAME_LEN];
  struct tge_room *adjacent_rooms[4];
  tge_describe_func *description;
  tge_action_func *func;
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
  tge_command_special_words *restrict ctx,
  tge_action_func action
);

void tge_trim(char *restrict str);
void tge_malloc();
void tge_free();

char *tge_capitalize(char *str);

unsigned int tge_word_compare(char *restrict fist, char *restrict second);
int tge_find_item(char *restrict item, char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN]);
void tge_create_item(char *restrict item, char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN]);
void tge_destroy_item(char *restrict item, char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN]);
void tge_item_transfer(char *restrict item, char to[MAX_INVENTORY_SIZE][MAX_NAME_LEN], char from[MAX_INVENTORY_SIZE][MAX_NAME_LEN]);
unsigned int tge_parse_command_array(char *restrict command, char **restrict command_array);

extern tge_player *player;
extern char **tge_parsed_commands;
extern tge_command *tge_structured_commands;

#endif