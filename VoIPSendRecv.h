#ifndef VOIPSENDRECV_H
#define VOIPSENDRECV_H
#include <netinet/in.h>

struct getmic_inet1 {
int sockfd1; 
struct sockaddr_in dest_addr1;
};

void inet_fatal(char *error, int exit_code);

void inet_sendto(int sockfd, void *mic_input, size_t mic_input_len, struct sockaddr_in dest_addr);

void inet_recvfrom(int sockfd, struct sockaddr_in dest_addr);

void create_sendrecv_socket(char *ipaddress);

void wait_for_connection(void);

void request_connection(char *ipaddress);
#endif /* VOIPSENDRECV_H */