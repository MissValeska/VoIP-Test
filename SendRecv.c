/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SendRecv.c
 * Author: Valeska
 *
 * Created on January 24, 2016, 1:21 AM
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <netdb.h>
#include <pthread.h>
#include <stdarg.h>
#include "PulseAudioMicInput.h"
#include "SendRecv.h"

void inet_recvfrom(void) {

int sockfd;

pthread_t playback_thread;

struct sockaddr_in dest_addr;
struct playback_sound playback;

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(2000);
dest_addr.sin_addr.s_addr = INADDR_ANY;

if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("Error in socket() 6 %i\n", sockfd);
}

printf("create_sendrecv_socket sockfd %d\n", sockfd);

while(1) {
int bytes_recvd, call_sound_len;
socklen_t dest_addr_len;
void *call_sound;
call_sound_len = sizeof(call_sound);
dest_addr_len = 1024;
if((bytes_recvd = recvfrom(sockfd, call_sound, call_sound_len, 0, (struct sockaddr*)&dest_addr, &dest_addr_len)) != 1024) {
	printf("Error in recvfrom() 7 %i\n", sockfd);
}
/* TODO: Make this a thread to enable simultaneous reception of data */

playback.call_sound = call_sound;
playback.call_sound_len = call_sound_len;

pthread_create(&playback_thread, NULL, (void *)&playback_connected_input, (void*)&playback);

//playback_connected_input(playback);
}
}

void IP_File(void) {
    FILE *fp;
    FILE *fp1;

   fp = fopen("participants.txt", "w");
   fp1 = fopen("participants-num.txt", "w");
   fprintf(fp1, "%s\n", "0");
   fclose(fp);
   fclose(fp1);
   return;
}

void wait_for_connection(void) {

int listenfd, connfd;

struct sockaddr_storage buf;
char ipstr[INET6_ADDRSTRLEN];
socklen_t buf_len = sizeof(buf);
int port = 2000;

struct sockaddr_in dest_addr;

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(port);
dest_addr.sin_addr.s_addr = INADDR_ANY;

if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("Error in server socket() 8 %i\n", listenfd);
}

printf("wait_for_connection listenfd %d\n", listenfd);

if(bind(listenfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
	printf("Error in server bind() 9 %i\n", listenfd);
}

while(1) {
if(listen(listenfd, 100) < 0) {
	printf("Error in server listen() 10 %i\n", listenfd);
}

if((connfd = accept(listenfd, (struct sockaddr*)&buf, &buf_len)) < 0) {
	printf("Error in server accept() 11 %i %i\n", connfd, listenfd);
}

if(buf.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in *)&buf;
    inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
}
if(buf.ss_family == AF_INET6) {
    struct sockaddr_in6 *s = (struct sockaddr_in6 *)&buf;
    inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
}

close(connfd);

printf("Peer IP address Server: %s\n", ipstr);

/* TODO: More work may need to be done here to ensure it never writes a NULL
 * or repetitive value */

/* Optional, Unnecessary: (Would prevent any loss of incoming sound for new 
 * participant upon joining. I.E Not losing a word or so upon connecting.)
 * Indicate to the sending function when a new participant is added */

if(ipstr != NULL) {

    FILE *fp;
    FILE *fp1;
    char *strnum;
    char *c;
    int z;
    
    fp = fopen("participants.txt", "a");
    fprintf(fp, "%s\n", ipstr);
    
    fp1 = fopen("participants-num.txt", "r+");
    *c = getc(fp1);
    z = atoi(c);
    z++;
    sprintf(strnum, "%d", z);
    fprintf(fp1, "%s\n", strnum);
    
    printf("%s\n %s\n %d\n", strnum, c, z);
    
    fclose(fp);
    fclose(fp1);
}
}
}

int request_connection(char *ipaddress) {
    int result;
    
    printf("Peer IP address Client: %s\n", ipaddress);

int sockfd;

int port = 2000;

struct sockaddr_in dest_addr;

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(port);
inet_pton(AF_INET, ipaddress, &dest_addr.sin_addr.s_addr);

if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	printf("Error in client socket() 12 %d\n", sockfd);
}

int iSetOption = 1;

setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
        sizeof(iSetOption));
        
result = connect(sockfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

if(result < 0) {
	printf("Error in client connect() 13 %d\n", sockfd);
}

printf("request_connection sockfd %d\n", sockfd);

close(sockfd);
    
    FILE *fp;
    FILE *fp1;
    char *strnum;
    char *c;
    int z;
    
    fp = fopen("participants.txt", "a");
    fprintf(fp, "%s\n", ipaddress);
    
    fp1 = fopen("participants-num.txt", "r+");
    *c = getc(fp1);
    z = atoi(c);
    z++;
    sprintf(strnum, "%d", z);
    fprintf(fp1, "%s\n", strnum);
    
    printf("%s\n %s\n %d\n", strnum, c, z);
    
    fclose(fp);
    fclose(fp1);
    
    return result;
}

int real_inet_sendto(struct line_num_and_sound line_sound1) {
    
    struct line_num_and_sound strct;
    strct = line_sound1;
    
    int sockfd;
    struct sockaddr_in dest_addr;

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(2000);
    
    inet_pton(AF_INET, strct.ipaddress, &dest_addr.sin_addr);
    
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("Error in socket() 6 %i\n", sockfd);
    }

    printf("create_sendrecv_socket sockfd %d\n", sockfd);
    
    while(1) {
        int bytes_recvd;
        size_t mic_input_len = sizeof(strct.buf);
        if((bytes_recvd = sendto(sockfd, strct.buf, mic_input_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr))) != 1024) {
            printf("Error in sendto() 7 %i\n", sockfd);
        }
        printf("Bytes Received: %i\n", bytes_recvd);
        /* TODO: Make always equal 1024
         * P.S: This could never happen due to get_mic_input's BUFSIZE */
        if(bytes_recvd < mic_input_len) {
        mic_input_len = mic_input_len - bytes_recvd;
        strct.buf_len = mic_input_len;
        real_inet_sendto(strct);
        }
    break;
    }
    close(sockfd);
    
    /* Add error codes */
    
    return 0;
}

/* Checks if an IP address is valid, Returning result == 0 or -1,
 * 0 being an invalid IP address, -1 being an invalid IP address family
 * I.E trying IPv6 on a system where that is not supported */
int isValidIpAddress(char *ipAddress, int family, int timeout, int timeoutc)
{
    struct sockaddr_in6 sa;
    int result = inet_pton(family, ipAddress, &(sa.sin6_addr));
    if(timeout <= timeoutc) {
    if(result == 0 || result == -1) {
        if(family != AF_INET6) {
            family = AF_INET6;
        }
        if(family != AF_INET) {
            family = AF_INET;
        }
        timeoutc++;
        result = isValidIpAddress(ipAddress, family, timeout, timeoutc);
    }
    }
    return result;
}

/* TODO: This may not be finished! */
void inet_sendto(struct getmic_inet1 strct) {

char *ipaddress = (char *)malloc(100);

FILE *fp;
FILE *fp1;

char *c;
int z;

int ch=0;
int lines=0;

fp = fopen("participants.txt", "r");
fp1 = fopen("participants-num.txt", "r");

*c = getc(fp1);
z = atoi(c);
    
printf("%s\n %d\n", c, z);

lines = z;

/*
while(!feof(fp))
{
  ch = fgetc(fp);
  if(ch == '\n')
  {
    lines++;
  }
}*/

pthread_t send_thread;

/* Temporarily maybe invalidated I think, Revisit: CRITICAL TODO: Make a 
 * separate base copy of strct so get_mic_input can call 
 * an infinite amount of times without issues. Remove latency requirement. */

struct line_num_and_sound line_sound;

int x;
int lines2;
int y;

lines2 = lines;
/*
if(lines2 >= 32) {
    lines2 = 32;
    y = lines % 32;
}*/

line_sound.lines = lines;
memcpy(line_sound.buf, strct.buf, sizeof(strct.buf));
line_sound.buf_len = strct.buf_len;

for(x = 0; x < lines2; x++) {
 
    fgets(ipaddress, 255, (FILE*)fp);

    *strrchr(ipaddress, '\n') = '\0';
    
    fseek(fp, 1, SEEK_CUR);
    
    /*int ret = isValidIpAddress(ipaddress, AF_INET, 4, 0);
    
    if(ret == 0 || ret == -1) {
        usleep(5000);
    }*/
    
    line_sound.ipaddress = ipaddress;
    
    /* Add reception of error codes, When added to real_inet_sendto() */
    
    pthread_create(&send_thread, NULL, (void *)&real_inet_sendto, (void*)&line_sound);
}

fclose(fp);
free(ipaddress);
return;
}