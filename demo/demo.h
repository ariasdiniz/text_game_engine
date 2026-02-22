#ifndef __TGE_DEMO_GAME
#define __TGE_DEMO_GAME

#include "../tge.h"

void initialize();
void finish();
void setup_commands();

void setup_starting_room();
void setup_west_room();
void setup_north_room();
void free_special_words();

extern tge_room starting_room;
extern tge_room north_room;
extern tge_room west_room;
extern tge_command_special_words *special_words;
extern int finish_game;

#endif