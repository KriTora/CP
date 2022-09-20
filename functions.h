#include <Windows.h>
#include <string>
#include <time.h>
#include "parameters.h"
#include "net.h"

void get_playfield_limits(int*, int*);

void screen_clean(char*);

void playfield_clean(char*, int*, int*);

void playfield_draw(char*, int*, int*);

void player_draw(char*, const int*, const int*);

bool process_input(int*, int*, int*, int*, bool*, bool*, Net*);

void quit(bool*, bool = false);

void player_pointer_draw(char*, int*, int*, int*, int*);

void player_position_label_draw(char*, int*, int*, int*, int*);

bool time_check(int*, int*);

void fps_label_draw(char*, int*);