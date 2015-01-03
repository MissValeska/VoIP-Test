#ifndef PULSEAUDIOMICINPUT_H
#define PULSEAUDIOMICINPUT_H
#include <pulse/simple.h>

void fatal(pa_simple *s, int exit_code);

void playback_connected_input(void *data, size_t bytes);

int get_mic_input(int sockfd, struct sockaddr_in dest_addr);
#endif /* PULSEAUDIOMICINPUT_H */