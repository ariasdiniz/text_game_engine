#include "../tge.h"
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

tge_player *player = NULL;
tge_command *tge_structured_commands = NULL;
char **tge_parsed_commands = NULL;

static void parse_command(
  char *restrict word,
  char **possible_cmds,
  unsigned int cmds_size,
  char **command
) {
  for (unsigned int i = 0; i < cmds_size; i++) {
    if (tge_word_compare(word, possible_cmds[i]) == 0) {
      *command = possible_cmds[i];
      break;
    }
  }
}

static char **tge_allocate_command_array() {
  char **commands = malloc(sizeof(char *) * TGE_MAXWORDS);
  if (commands == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  for (unsigned int i = 0; i < TGE_MAXWORDS; i++) {
    commands[i] = malloc(sizeof(char) * TGE_MAXLEN);
    if (commands[i] == NULL) {
      for (unsigned int j = 0; j < i; j++) {
        free(commands[j]);
      }
      free(commands);
      errno = ENOMEM;
      return NULL;
    }
  }
  return commands;
}

static void tge_free_command_array(char **restrict command_array) {
  for (unsigned int i = 0; i < TGE_MAXWORDS; i++) {
    free(command_array[i]);
  }
  free(command_array);
}

static void clean_structured_commands() {
  tge_structured_commands->ind_obj = NULL;
  tge_structured_commands->prep = NULL;
  tge_structured_commands->noun = NULL;
  tge_structured_commands->verb = NULL;
}

static int find_empty_idx(char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN], int size) {
  for (int i = 0; i < size; i++) {
    if (list[i][0] == '\0') return i;
  }
  return -1;
}

int tge_find_item(char *restrict item, char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN]) {
  for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
    if (tge_word_compare(list[i], item) == 0) return i;
  }
  return -1;
}

void tge_create_item(char *restrict item, char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN]) {
  int idx = find_empty_idx(list, MAX_INVENTORY_SIZE);
  if (idx == -1) {
    errno = TGE_ENOISPACE;
    return;
  }
  strncpy(list[idx], item, MAX_NAME_LEN);
}

void tge_destroy_item(char *restrict item, char list[MAX_INVENTORY_SIZE][MAX_NAME_LEN]) {
  int idx = tge_find_item(item, list);
  if (idx == -1) {
    errno = TGE_ENOITEM;
    return;
  }
  list[idx][0] = '\0';
}

void tge_malloc() {
  tge_structured_commands = malloc(sizeof(tge_command));
  if (tge_structured_commands == NULL) {
    errno = ENOMEM;
    return;
  }

  player = malloc(sizeof(tge_player));
  if (tge_structured_commands == NULL) {
    errno = ENOMEM;
    free(tge_structured_commands);
    return;
  }

  tge_parsed_commands = tge_allocate_command_array();
  if (tge_parsed_commands == NULL) {
    errno = ENOMEM;
    free(tge_structured_commands);
    free(player);
    return;
  }
}

void tge_free() {
  free(tge_structured_commands);
  free(player);
  tge_free_command_array(tge_parsed_commands);
}

void tge_trim(char *restrict str) {
  unsigned int begin_flag = 0;
  unsigned int begin_idx = 0;
  unsigned int end_idx = 0;

  for (unsigned int i = 0; i < TGE_MAXLEN && str[i] != '\0'; i++) {
    if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') {
      if (begin_flag == 0) {
        begin_idx++;
        end_idx++;
      }
      continue;
    }
    begin_flag |= 1;
    end_idx = i;
  }

  for (unsigned int i = begin_idx; i <= end_idx; i++) {
    str[i - begin_idx] = str[i];
  }

  str[end_idx - begin_idx + 1] = '\0';
}

char *tge_capitalize(char *str) {
  if (str == NULL) return str;
  if (str[0] == '\0') return str;

  str[0] = toupper(str[0]);
  return str;
}

unsigned int tge_parse_command_array(char *restrict command, char **restrict command_array) {

  unsigned int wordcount = 0;
  unsigned int new_word_flag = 0;
  unsigned int word_idx = 0;

  tge_trim(command);

  for (unsigned int i = 0; i < TGE_MAXLEN && command[i] != '\0'; i++) {
    if (command[i] == ' ' && new_word_flag == 1) {
      if (word_idx < TGE_MAXLEN - 1) {
        command_array[wordcount][word_idx + 1] = '\0';
      } else {
        command_array[wordcount][TGE_MAXLEN - 1] = '\0';
      }
      new_word_flag &= 0;
      wordcount++;
      word_idx = 0;
      continue;
    } else if (command[i] == ' ') {
      continue;
    }

    new_word_flag |= 1;
    if (wordcount == TGE_MAXWORDS) return TGE_MAXWORDS;
    command_array[wordcount][word_idx] = command[i];
    word_idx++;
  }

  if (word_idx < TGE_MAXLEN - 1) {
    command_array[wordcount][word_idx + 1] = '\0';
  } else {
    command_array[wordcount][TGE_MAXLEN - 1] = '\0';
  }
  return wordcount + 1;
}

unsigned int tge_word_compare(char *restrict first, char *restrict second) {
  if (first == NULL || second == NULL) return 1;
  for (int i = 0; i < TGE_MAXLEN; i++) {
    if (tolower(first[i]) != tolower(second[i])) return 1;
    if (first[i] == '\0' || second[i] == '\0') break;
  }
  return 0;
}

void tge_item_transfer(char *restrict item, char to[MAX_INVENTORY_SIZE][MAX_NAME_LEN], char from[MAX_INVENTORY_SIZE][MAX_NAME_LEN]) {
  int idx = tge_find_item(item, from);
  if (idx == -1) {
    errno = TGE_ENOITEM;
    return;
  }
  int inv_idx = find_empty_idx(to, MAX_INVENTORY_SIZE);
  if (inv_idx == -1) {
    errno = TGE_ENOISPACE;
    return;
  }
  strncpy(to[inv_idx], from[idx], MAX_NAME_LEN);
  from[idx][0] = '\0';
}

void run_action(
  char *restrict unparsed_command,
  tge_command_special_words *restrict ctx,
  tge_action_func action
) {
  clean_structured_commands();
  unsigned int commands_size = tge_parse_command_array(unparsed_command, tge_parsed_commands);
  for (unsigned int i = 0; i < commands_size; i++) {
    parse_command(tge_parsed_commands[i], ctx->verbs, ctx->verbs_size, &tge_structured_commands->verb);
    parse_command(tge_parsed_commands[i], ctx->nouns, ctx->nouns_size, &tge_structured_commands->noun);
    parse_command(tge_parsed_commands[i], ctx->preps, ctx->preps_size, &tge_structured_commands->prep);
    parse_command(tge_parsed_commands[i], ctx->ind_objs, ctx->ind_objs_size, &tge_structured_commands->ind_obj);
  }
  if (tge_structured_commands->verb == NULL) {
    printf("%s can't do that.\n\n", player->name);
  } else {
    action(tge_structured_commands);
  }
  for (unsigned int i = 0; i < commands_size; i++) {
    for (unsigned int j = 0; j < TGE_MAXLEN; j++) {
      tge_parsed_commands[i][j] = '\0';
    }
  }
}