#include "demo.h"
#include "../tge.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define DESCRIPTION_0_0_CONDITIONAL "This place is pitch black. Without a light source, %s can't see anything here.\n\n"

#define DESCRIPTION_0_1_CONDITIONAL "With %s lit torch, %s can now see what lies in this place. " \
                                    "It's a very tight place, almost a corridor. Two adults would have difficulty to " \
                                    "walk side by side. At the end of this narrow, short corridor, there is a stone door. " \
                                    "From the wind coming from the door gaps and the faint sound of leaves, %s assumes that " \
                                    "this door is the cave's entrance, and possibly %s exit from here. Since %s can't spot " \
                                    "light coming from the gaps, it is probably dark outside.\n\n"

#define DESCRIPTION_0_2_CONDITIONAL "Despite the light from the brazier, %s can't make much sense of the carvings.\n"

#define DESCRIPTION_0 "To the " ANSI_COLOR_GREEN "south" ANSI_COLOR_RESET " there is the part of the cave where " \
                      "%s came from.\n\nNothing else piques %s interest here.\n\n"

static int is_room_instantiated = 0;
static int have_dagger = 0;
static int have_lit_torch = 0;

static void describe() {
  if (have_lit_torch == 0 && tge_find_item("L_TORCH", player->items) != -1) {
    have_lit_torch = 1;
  }

  if (have_lit_torch == 0) {
    printf(DESCRIPTION_0_0_CONDITIONAL, player->name);
  } else {
    printf(DESCRIPTION_0_1_CONDITIONAL, player->pronouns[1], player->name, player->pronouns[0], player->pronouns[1], player->pronouns[0]);
  }

  if (have_dagger == 0 && have_lit_torch) printf(DESCRIPTION_0_0_CONDITIONAL);

  if (have_dagger && have_lit_torch) {
    printf(DESCRIPTION_0_1_CONDITIONAL, player->name, player->pronouns[0], player->pronouns[0], player->pronouns[0]);
  } else {
    printf(DESCRIPTION_0_2_CONDITIONAL, player->name);
  }

  printf(DESCRIPTION_0, player->name, player->pronouns[1]);
}

static void north_room_action(tge_command *cmd) {
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
    if (tge_word_compare(cmd->noun, "south") == 0) {
      player->current_room = player->current_room->adjacent_rooms[2];
      return;
    }
  }

  if (
    tge_word_compare(cmd->verb, "take") == 0 ||
    tge_word_compare(cmd->verb, "grab") == 0 ||
    tge_word_compare(cmd->verb, "pick") == 0 ||
    tge_word_compare(cmd->verb, "get") == 0
  ) {
    if (tge_word_compare(cmd->noun, "dagger") == 0 && have_dagger == 0) {
      tge_item_transfer("DAGGER", player->items, starting_room.items);
      have_dagger++;
      printf("%s takes the dagger. Maybe %s won't need it, but better safe than sorry.\n\n", player->name, player->pronouns[0]);
      return;
    }
  }

  if (
    tge_word_compare(cmd->verb, "put") == 0 ||
    tge_word_compare(cmd->verb, "unlock") == 0 ||
    tge_word_compare(cmd->verb, "use") == 0
  ) {
    if (tge_word_compare(cmd->noun, "key") == 0) {
      if (tge_word_compare(cmd->ind_obj, "door") == 0 && tge_find_item("KEY", player->items) != -1) {
        tge_destroy_item("KEY", player->items);
        printf("%s unlocks the door using the metal key and opens it.\n\n", player->name);
        printf(DESCRIPTION_0_1_CONDITIONAL, player->name, player->pronouns[0], player->pronouns[0], player->pronouns[0]);
        finish_game = 0;
        return;
      }
    }
  }

  printf("%s can't do that.\n\n", player->name);
}

void setup_north_room() {
  if (is_room_instantiated == 0) {
    north_room.description = describe;
    north_room.func = north_room_action;
    strcpy(west_room.items[0], "DAGGER");
    is_room_instantiated++;
  }
}
