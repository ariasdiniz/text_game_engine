#include "../tge.h"
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

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

char **tge_allocate_command_array() {
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

void tge_free_command_array(char **restrict command_array) {
  for (unsigned int i = 0; i < TGE_MAXWORDS; i++) {
    free(command_array[i]);
  }
  free(command_array);
}

unsigned int tge_word_compare(char *restrict first, char *restrict second) {
  for (int i = 0; i < TGE_MAXLEN; i++) {
    if (tolower(first[i]) != tolower(second[i])) return 1;
    if (first[i] == '\0' || second[i] == '\0') break;
  }
  return 0;
}

void run_action(char *restrict command, tge_action_func action) {
  
}