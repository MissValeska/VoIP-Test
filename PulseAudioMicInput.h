#ifndef PULSEAUDIOMICINPUT_H
#define PULSEAUDIOMICINPUT_H
#include <pulse/simple.h>

#include "SendRecv.h"

void fatal(pa_simple *s, int exit_code);

void playback_connected_input(struct playback_sound playback);

int get_mic_input(void);
#endif /* PULSEAUDIOMICINPUT_H *//