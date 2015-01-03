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
#include "PulseAudioMicInput.h"

void inet_fatal(char *error, int exit_code) {
	perror(error);
	exit(exit_code);
}

void inet_sendto(int sockfd, void *mic_input, size_t mic_input_len, struct sockaddr_in dest_addr) {
int bytes_sent;
if((bytes_sent = sendto(sockfd, mic_input, mic_input_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr))) < 0) {
	inet_fatal("Error in sendto()", 5);
}
if(bytes_sent < mic_input_len) {
mic_input_len = mic_input_len - bytes_sent;
inet_sendto(sockfd, mic_input, mic_input_len, dest_addr);
}
}

void inet_recvfrom(int sockfd, struct sockaddr_in dest_addr) {
while(1) {
int bytes_recvd, call_sound_len;
socklen_t dest_addr_len;
void *call_sound;
call_sound_len = sizeof(call_sound);
dest_addr_len = sizeof(dest_addr);
if((bytes_recvd = recvfrom(sockfd, call_sound, call_sound_len, 0, (struct sockaddr*)&dest_addr, &dest_addr_len)) < 0) {
	inet_fatal("Error in recvfrom()", 7);
}

playback_connected_input(call_sound, call_sound_len);

/*if(bytes_recvd < call_sound_len) {
call_sound_len = call_sound_len - bytes_recvd;
inet_recvfrom(sockfd, call_sound, call_sound_len, dest_addr);
}*/
}
}

void create_sendrecv_socket(char *ipaddress) {

int sockfd;

struct sockaddr_in dest_addr;

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(2000);
inet_pton(AF_INET, ipaddress, &dest_addr.sin_addr);

if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	inet_fatal("Error in socket() ", 6);
}
/* Make me a thread */
inet_recvfrom(sockfd, dest_addr);

get_mic_input(sockfd, dest_addr);
}

void wait_for_connection(void) {

int listenfd, connfd;

struct sockaddr_storage buf;
char ipstr[INET6_ADDRSTRLEN];
socklen_t buf_len = sizeof(buf);
int port = 2001;

struct sockaddr_in dest_addr;

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(port);
dest_addr.sin_addr.s_addr = INADDR_ANY;

if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	inet_fatal("Error in server socket() ", 8);
}

if(bind(listenfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
	close(listenfd);
	inet_fatal("Error in server bind() ", 9);
}

if(listen(listenfd, 100) < 0) {
	inet_fatal("Error in server listen() ", 10);
}
printf("OFNHVE\n");
while(1) {
if((connfd = accept(listenfd, (struct sockaddr*)&buf, &buf_len)) < 0) {
	inet_fatal("Error in server accept() ", 11);
}
printf("Wtf is this\n");
if(getpeername(connfd, (struct sockaddr*)&buf, &buf_len) < 0) {
	inet_fatal("Error in server getpeername() ", 12);
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
/* Make me a thread */
printf("Peer IP address: %s\n", ipstr);
create_sendrecv_socket(ipstr);
}
}

void request_connection(char *ipaddress) {

printf("Peer IP address: %s\n", ipaddress);

int sockfd;

char *buf;
socklen_t buf_len;
buf_len = sizeof(buf);
int port = 2001;

struct sockaddr_in dest_addr;

dest_addr.sin_family = AF_INET;
dest_addr.sin_port = htons(2001);
inet_pton(AF_INET, ipaddress, &dest_addr.sin_addr.s_addr);

if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	inet_fatal("Error in client socket() ", 13);
}

if(connect(sockfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
	inet_fatal("Error in client connect() ", 14);
}
close(sockfd);

create_sendrecv_socket(ipaddress);
}