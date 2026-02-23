#include "demo.h"
#include "../tge.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define DESCRIPTION_0 "As %s reaches this part of the cave, a chill runs down %s spine as a faint breeze " \
                      "comes from behind %s. While the rest of the cave looks natural, this part clearly " \
                      "has been worked and used by humans. In front of one of the walls there is a large "\
                      "stone altar. The top of the altar is sullied with bloodstains, and the flame flickering" \
                      "from the small brazier suspended above the altar casts an eerie shadow on it. " \
                      "Murals with symbols in bas-relief were carefully carved in the stone walls.\n\n"

#define DESCRIPTION_0_0_CONDITIONAL "On top of the altar is an unlit " ANSI_COLOR_RED "torch" ANSI_COLOR_RESET ".\n"

#define DESCRIPTION_0_1_CONDITIONAL "With the lit torch burning bright, %s can now make sense of the carvings. " \
                                    "The carvings describe robed figures kidnapping victims, bringing them unconscious to " \
                                    "an altar and sacrificing them. As some of the carvings suggests, these sacrifices are " \
                                    "made to empower an entity %s has never seen before, a gigantic being with human torso, "\
                                    "squid head and several tentacles coming out from its mouth. It's clear that %s "\
                                    "would be the next victim, but %s have no idea what happened here that prevented it.\n\n"

#define DESCRIPTION_0_2_CONDITIONAL "Despite the light from the brazier, %s can't make much sense of the carvings.\n"

#define DESCRIPTION_1 "To the " ANSI_COLOR_GREEN "east" ANSI_COLOR_RESET " there is the part of the cave where " \
                      "%s came from.\n\nNothing else piques %s interest here.\n\n"

static int is_room_instantiated = 0;
static int have_lit_torch = 0;
static int have_torch = 0;

static void describe() {
  if (have_lit_torch == 0 && tge_find_item("L_TORCH", player->items) != -1) {
    have_lit_torch = 1;
  }

  printf(DESCRIPTION_0, player->name, player->pronouns[1], player->pronouns[1]);

  if (have_torch == 0) printf(DESCRIPTION_0_0_CONDITIONAL);

  if (have_lit_torch) {
    printf(DESCRIPTION_0_1_CONDITIONAL, player->name, player->pronouns[0], player->pronouns[0], player->pronouns[0]);
  } else {
    printf(DESCRIPTION_0_2_CONDITIONAL, player->name);
  }

  printf(DESCRIPTION_1, player->name, player->pronouns[1]);
}

static void west_room_action(tge_command *cmd) {
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
    if (tge_word_compare(cmd->noun, "east") == 0) {
      player->current_room = player->current_room->adjacent_rooms[1];
      return;
    }
  }

  if (
    tge_word_compare(cmd->verb, "take") == 0 ||
    tge_word_compare(cmd->verb, "grab") == 0 ||
    tge_word_compare(cmd->verb, "pick") == 0 ||
    tge_word_compare(cmd->verb, "get") == 0
  ) {
    if (tge_word_compare(cmd->noun, "torch") == 0 && have_torch == 0) {
      tge_item_transfer("TORCH", player->items, starting_room.items);
      have_torch++;
      printf("%s takes the torch. A shame that an unlit torch is of little use.\n\n", player->name);
      return;
    }
  }

  if (
    tge_word_compare(cmd->verb, "lit") == 0 ||
    tge_word_compare(cmd->verb, "put") == 0 ||
    tge_word_compare(cmd->verb, "hold") == 0 ||
    tge_word_compare(cmd->verb, "use") == 0 ||
    tge_word_compare(cmd->verb, "light") == 0
  ) {
    if (tge_word_compare(cmd->noun, "torch") == 0) {
      if (tge_word_compare(cmd->ind_obj, "brazier") == 0 && have_torch && have_lit_torch == 0) {
        have_lit_torch++;
        tge_destroy_item("TORCH", player->items);
        tge_create_item("L_TORCH", player->items);
        printf("%s lits the torch using the brazier.\n\n", player->name);
        printf(DESCRIPTION_0_1_CONDITIONAL, player->name, player->pronouns[0], player->pronouns[0], player->pronouns[0]);
        return;
      } else {
        printf("How does %s light the torch?\n\n", player->name);
        return;
      }
    }
  }

  printf("%s can't do that.\n\n", player->name);
}

void setup_west_room() {
  if (is_room_instantiated == 0) {
    west_room.description = describe;
    west_room.func = west_room_action;
    strncpy(west_room.items[0], "TORCH", 6);
    is_room_instantiated++;
  }
}
