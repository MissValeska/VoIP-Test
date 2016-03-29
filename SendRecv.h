/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SendRecv.h
 * Author: valeska
 *
 * Created on March 4, 2016, 7:25 PM
 */

#ifndef SENDRECV_H
#define SENDRECV_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "PulseAudioMicInput.h"
#include <stdio.h>
#include <stdint.h>
  
#define BUFSIZE 1024
    
struct getmic_inet1 {
uint8_t buf[1024];
int buf_len;
FILE *fp;
int rawr;
int EOF1;
char *check;
char *check1;
int exempt;
};

struct line_num_and_sound {
int lines;
uint8_t buf[1024];
int buf_len;
char *ipaddress;
};

struct playback_sound {
void *call_sound;
int call_sound_len;
};

void inet_recvfrom(void);

void IP_File(void);

void wait_for_connection(void);

int real_inet_sendto(struct line_num_and_sound line_sound1);

void inet_sendto(struct getmic_inet1 strct);

int isValidIpAddress(char *ipAddress, int family, int timeout, int timeoutc);

int request_connection(char *ipaddress);

#ifdef __cplusplus
}
#endif

#endif /* SENDRECV_H */

