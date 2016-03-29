#include <stdlib.h>
#include <pthread.h>
#include "SendRecv.h"
#include "PulseAudioMicInput.h"
//#include "VoIPSendRecv.h"

/* TODO: Modify the compilation script to manage the new file(s) */

int main(int argc, char**argv) {

        /* Do not call this if you want to resume a call, This deletes the
         * participants file */
    
        IP_File();
    
        if(argc >= 2) {
                /* TODO: Check the error code returned from this function */
		request_connection(argv[1]);
	}
        
        /* Write the counter-part to the wait thread, a client to connect to
         * another's wait thread with */
        
	pthread_t wait_thread;

	pthread_create(&wait_thread, NULL, (void *)&wait_for_connection, NULL);

	pthread_t inet_recvfrom_thread;

	pthread_create(&inet_recvfrom_thread, NULL, (void *)&inet_recvfrom, NULL);        
        
	get_mic_input();        
        
	return 0;
}