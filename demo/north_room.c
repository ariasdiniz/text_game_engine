#include "demo.h"
#include "../tge.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define DESCRIPTION_0_0_CONDITIONAL "This place is pitch black. Without a light source, %s can't see anything here.\n\n"

#define DESCRIPTION_0_1_CONDITIONAL "With %s lit torch, %s can now see what lies in this place. " \
                                    "It's a very tight place, almost a corridor. Two adults would have difficulty in " \
                                    "walking side by side. At the end of this narrow, short corridor, there is a stone door. " \
                                    "From the wind coming from the door gaps and the faint sound of leaves, %s assumes that " \
                                    "this door is the cave's entrance, and possibly %s exit from here. Since %s can't spot " \
                                    "light coming from the gaps, it is probably dark outside.\n\n" \
                                    "Looking closer at the door, it appears to be locked. There is a visible keyhole " \
                                    "in the center of it.\n\n" \
                                    "In the middle of the corridor, there is another body wearing a black robe. " \
                                    "This one is sitting on the ground with their back against the wall. Differently " \
                                    "from the last one, this person appears to have all their bones, but it is also " \
                                    "faceless. Looks like they've taken their own life by cutting their neck.\n\n"

#define DESCRIPTION_1_0_CONDITIONAL "The dagger they used are still in their hands.\n\n"

#define DESCRIPTION_0 "To the " ANSI_COLOR_GREEN "south" ANSI_COLOR_RESET " there is the part of the cave where " \
                      "%s came from.\n\nNothing else piques %s interest here.\n\n"

#define ENDING_0 "As %s step out of the cave, %s stops. A cold drop of sweat slowly drips from %s forehead to %s " \
                 "nose while slightly moisting %s left eye, but this doesn't seem to bother %s while %s observes the " \
                 "cataclysm before %s eyes. As %s slowly illuminates the surroundings with the torch, %s hands shake " \
                 "more and more violently. The trees direct in front of the cave entrance are completely dry, and the " \
                 "small animals that lived there are twisted and pale like the bodies inside the cave. There is a path " \
                 "of death and decay carved in almost a straight line through the forest, going directly to the city. " \
                 "Whatever otherworldly creature those people were trying to summon, they surely got its attention.\n\n"

#define ENDING_1 "%s slowly looks to the sky. Above the trees, %s can see a bright red light coming from the city, " \
                 "and after concentrating, %s can also hear the distant sound of people screming. "

#define ENDING_0_CONDITIONAL "%s looks down to the dagger in %s hands.\n"

#define ENDING_1_CONDITIONAL "There is no hope. %s is alone.\n"

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

  if (have_dagger == 0 && have_lit_torch) printf(DESCRIPTION_1_0_CONDITIONAL);

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
    tge_word_compare(cmd->verb, "use") == 0 ||
    tge_word_compare(cmd->verb, "open") == 0
  ) {
    if (tge_word_compare(cmd->noun, "key") == 0) {
      if (tge_word_compare(cmd->ind_obj, "door") == 0) {
        if (tge_find_item("KEY", player->items) != -1) {
          tge_destroy_item("KEY", player->items);
          printf("%s unlocks the door using the metal key and opens it.\n\n", player->name);
          finish_game = 0;
          printf(
            ENDING_0,
            player->name,
            player->pronouns[0],
            player->pronouns[1],
            player->pronouns[1],
            player->pronouns[1],
            player->pronouns[1],
            player->pronouns[0],
            player->pronouns[1],
            player->pronouns[0],
            player->pronouns[1]
          );
          printf(
            ENDING_1,
            player->name,
            player->pronouns[0],
            player->pronouns[0]
          );
          tge_capitalize(player->pronouns[0]);
          if (have_dagger) {
            printf(
              ENDING_0_CONDITIONAL,
              player->pronouns[0],
              player->pronouns[1]
            );
          } else {
            printf(ENDING_1_CONDITIONAL, player->pronouns[0]);
          }
          return;
        } else {
          printf("%s does not have a key that unlocks this door.\n", player->name);
          return;
        }
      }
    }
    if (tge_word_compare(cmd->ind_obj, "door") == 0) {
      printf("How does %s open the door?\n", player->name);
    }
  }

  printf("%s can't do that.\n\n", player->name);
}

void setup_north_room() {
  if (is_room_instantiated == 0) {
    north_room.description = describe;
    north_room.func = north_room_action;
    strncpy(west_room.items[0], "DAGGER", 7);
    is_room_instantiated++;
  }
}
