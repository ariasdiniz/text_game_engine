#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../tge.h"
#include "demo.h"

tge_room starting_room, north_room, west_room;
tge_command_special_words *special_words;

int main() {
  int endgame = 1;
  char buffer[TGE_MAXLEN];
  initialize();
  setup_commands();
  if (errno != 0) {
    perror("Error allocating memory.");
    return EXIT_FAILURE;
  }
  setup_starting_room();
  tge_room *room = NULL;
  while (endgame) {
    if (tge_current_room != room) {
      room = tge_current_room;
      room->description();
    }
    printf("What does %s do?\n", player->name);
    fgets(buffer, TGE_MAXLEN, stdin);
    run_action(buffer, special_words, room->func);
  }
  free_special_words();
  finish();
  return EXIT_SUCCESS;
}