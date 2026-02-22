#include "demo.h"
#include "../tge.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define START_GAME_0 "%s wakes up on the cold stone floor. %s eyes try to adjust to the dim light " \
                     "as %s try to understand where %s is. "

#define START_GAME_1 "%s head aches and %s feels nauseous, but these feelings are suddenly suppressed " \
                     "by the cold realization that %s is alone.\n\n"

#define DESCRIPTION_0 "%s is in the center of a roundish stone cave. A dim light illuminates the place, but " \
                      "%s isn't sure where it comes from. The air is damp and still, with a faint odor of " \
                      "mold.\n\n"

#define DESCRIPTION_0_1_CONDITIONAL "Now that %s carries a lit torch, %s can spot a small shining object in " \
                                    "one of the corners. As %s approaches the object for closer examination, " \
                                    "it is a " ANSI_COLOR_RED "metal key " ANSI_COLOR_RESET "."

#define DESCRIPTION_1 "To the " ANSI_COLOR_GREEN "north" ANSI_COLOR_RESET ", where %s can barely see, camouflaged " \
                      "by the shadows is an opening between two rocks, where %s can pass. There is no light " \
                      "there, so it's hard to say where it leads. Maybe %s should find a light source before " \
                      "proceeding.\n"

#define DESCRIPTION_2 "To the " ANSI_COLOR_GREEN "west" ANSI_COLOR_RESET " there is something that, from here," \
                      "looks like a stone altar.\n\nNothing else picks %s interest here.\n\n"

static int is_room_instantiated = 0;
static int have_lit_torch = 0;
static int key_taken = 0;

static void describe() {
  if (have_lit_torch == 0 && tge_find_item("L_TORCH", player->items) != -1) {
    have_lit_torch = 1;
  }
  printf(DESCRIPTION_0, player->name, player->pronouns[0]);
  player->pronouns[1][0] = tolower(player->pronouns[1][0]);
  player->pronouns[1][1] = tolower(player->pronouns[1][1]);
  if (have_lit_torch && key_taken == 0) {
    printf(DESCRIPTION_0_1_CONDITIONAL, player->name, player->pronouns[0], player->pronouns[0]);
  }
  printf(DESCRIPTION_1, player->name, player->pronouns[0], player->pronouns[0]);
  printf(DESCRIPTION_2, player->pronouns[1]);
}

static void starting_room_action(tge_command *cmd) {
  if (tge_find_item("L_TORCH", player->items) != -1) {
    have_lit_torch = 1;
  }

  if (
      tge_word_compare(cmd->verb, "describe") == 0 ||
      (
        tge_word_compare(cmd->verb, "look") == 0 && (
          tge_word_compare(cmd->noun, "around") == 0 ||
          tge_word_compare(cmd->noun, "room") == 0
        )
      )
    ) {
      describe();
      return;
    }

  if (
    tge_word_compare(cmd->verb, "go") == 0 ||
    tge_word_compare(cmd->verb, "walk") == 0 ||
    tge_word_compare(cmd->verb, "run") == 0
  ) {
    if (tge_word_compare(cmd->noun, "north") == 0) {
      player->current_room = player->current_room->adjacent_rooms[0];
      return;
    } else if (tge_word_compare(cmd->noun, "west") == 0) {
      player->current_room = player->current_room->adjacent_rooms[3];
      return;
    }
  }

  if (
    tge_word_compare(cmd->verb, "take") == 0 ||
    tge_word_compare(cmd->verb, "grab") == 0 ||
    tge_word_compare(cmd->verb, "pick") == 0
  ) {
    if (tge_word_compare(cmd->noun, "key") == 0 && key_taken == 0 && have_lit_torch) {
      tge_item_transfer("KEY", player->items, starting_room.items);
      key_taken++;
      printf("%s take the key.\n\n", player->name);
      return;
    }
  }

  printf("%s can't do that.\n\n", player->name);
}

void setup_starting_room() {
  if (is_room_instantiated == 0) {
    printf(START_GAME_0, player->name, tge_capitalize(player->pronouns[1]), player->pronouns[0], player->pronouns[0]);
    printf(START_GAME_1, tge_capitalize(player->pronouns[1]), player->pronouns[0], player->pronouns[0]);
    player->pronouns[1][0] = tolower(player->pronouns[1][0]);
    starting_room.description = describe;
    starting_room.func = starting_room_action;
    starting_room.adjacent_rooms[0] = &north_room;
    starting_room.adjacent_rooms[3] = &west_room;
    north_room.adjacent_rooms[2] = &starting_room;
    west_room.adjacent_rooms[1] = &starting_room;
    player->current_room = &starting_room;
    is_room_instantiated++;
  }
}
