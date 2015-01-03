#include <stdlib.h>
#include <pthread.h>
#include "VoIPSendRecv.h"

int main(int argc, char**argv) {

	pthread_t wait_thread;

	pthread_create(&wait_thread, NULL, (void *)&wait_for_connection, NULL);

	if(argc >= 2) {
		request_connection(argv[1]);
	}
	
	pthread_join(wait_thread, NULL);
	return 0;
}