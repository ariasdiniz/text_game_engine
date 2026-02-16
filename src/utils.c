#include "../tge.h"
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#define MAXLEN 128
#define MAXWORDS 32

extern void tge_trim(char *restrict str) {
  char begin_flag = 0;
  char begin_idx = 0;
  char end_idx = 0;

  for (unsigned int i = 0; i < MAXLEN && str[i] != '\0'; i++) {
    if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') {
      if (begin_flag == 0) {
        begin_idx++;
        end_idx++;
      }
      continue;
    }
    begin_flag |= 0b1;
    end_idx = i;
  }

  for (unsigned int i = begin_idx; i <= end_idx; i++) {
    str[i - begin_idx] = str[i];
  }

  str[end_idx - begin_idx + 1] = '\0';
}

extern unsigned short tge_parse_command_array(char *restrict command, char **restrict command_array) {

  unsigned short wordcount = 0;
  unsigned short new_word_flag = 0;
  unsigned short word_idx = 0;

  tge_trim(command);

  for (unsigned short i = 0; i < MAXLEN && command[i] != '\0'; i++) {
    if (command[i] == ' ' && new_word_flag == 1) {
      if (word_idx < MAXLEN - 1) {
        command_array[wordcount][word_idx + 1] = '\0';
      } else {
        command_array[wordcount][MAXLEN - 1] = '\0';
      }
      new_word_flag &= 0b0;
      wordcount++;
      word_idx = 0;
      continue;
    } else if (command[i] == ' ') {
      continue;
    }

    new_word_flag |= 0b1;
    if (wordcount == MAXWORDS) return MAXWORDS;
    command_array[wordcount][word_idx] = command[i];
    word_idx++;
  }

  if (word_idx < MAXLEN - 1) {
    command_array[wordcount][word_idx + 1] = '\0';
  } else {
    command_array[wordcount][MAXLEN - 1] = '\0';
  }
  return wordcount + 1;
}

extern char **tge_allocate_command_array() {
  char **commands = malloc(sizeof(char *) * MAXWORDS);
  if (commands == NULL) {
    errno = ENOMEM;
    perror("Error allocating memory.");
    return NULL;
  }

  for (unsigned short i = 0; i < MAXWORDS; i++) {
    commands[i] = malloc(sizeof(char) * MAXLEN);
    if (commands[i] == NULL) {
      for (unsigned short j = 0; j < i; j++) {
        free(commands[j]);
      }
      free(commands);
      errno = ENOMEM;
      perror("Error allocating memory.");
      return NULL;
    }
  }
  return commands;
}

extern void tge_free_command_array(char **restrict command_array) {
  for (unsigned short i = 0; i < MAXWORDS; i++) {
    free(command_array[i]);
  }
  free(command_array);
}

extern unsigned int tge_word_compare(char *restrict first, char *restrict second) {
  for (int i = 0; i < MAXLEN; i++) {
    if (tolower(first[i]) != tolower(second[i])) return 1;
    if (first[i] == '\0' || second[i] == '\0') break;
  }
  return 0;
}

extern void run_action(char *restrict command, tge_action_func action) {
  
}