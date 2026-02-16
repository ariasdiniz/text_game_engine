#include "../tge.h"
#include <ctype.h>
#include <stdlib.h>

#define MAXLEN 64

static char **command_array(char *restrict command) {

}

extern unsigned int word_compare(char *restrict first, char *restrict second) {
  for (int i = 0; i < MAXLEN; i++) {
    if (tolower(first[i]) != tolower(second[i])) return 1;
    if (first[i] == '\0' || second[i] == '\0') break;
  }
  return 0;
}

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

extern void run_action(char *restrict command, tge_action_func action) {
  
}