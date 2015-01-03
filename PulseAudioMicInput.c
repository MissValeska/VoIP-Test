#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <pulse/simple.h>
#include <pulse/error.h>
#include <pulse/gccmacro.h>
#include <netinet/in.h>
#include "VoIPSendRecv.h"

#define BUFSIZE 1024

void fatal(pa_simple *s, int exit_code) {

    if (s)
        pa_simple_free(s);

        exit(exit_code);

}

/* If any of the three if statements are true, fatal() will be called
* And the program will exit without closing any of it's sockets.
* Thus creating a bind error on the next run of this program,
* And leaving a socket open on the system, This needs to be fixed.  */

void playback_connected_input(void *data, size_t bytes) {

    pa_sample_spec playback_ss;

   playback_ss.format = PA_SAMPLE_S16LE;
   playback_ss.rate = 44100;
   playback_ss.channels = 2;

    pa_simple *playback_s;
    int ret;
    int playback_error;

    /* Create a new playback stream */
    if (!(playback_s = pa_simple_new(NULL, "VoIP-PlayBack", PA_STREAM_PLAYBACK, NULL, "playback", &playback_ss, NULL, NULL, &playback_error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n", pa_strerror(playback_error));
        fatal(playback_s, 3);
    }

        pa_usec_t latency;
        if ((latency = pa_simple_get_latency(playback_s, &playback_error)) == (pa_usec_t) -1) {
            fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(playback_error));
            fatal(playback_s, 4);
        }
        fprintf(stderr, "%0.0f usec    \r", (float)latency);

        /* ... and play it */
        if (pa_simple_write(playback_s, data, bytes, &playback_error) < 0) {
            fprintf(stderr, __FILE__": pa_simple_write() failed: %s\n", pa_strerror(playback_error));
            fatal(playback_s, 5);
        }
        /* Removing it mitigated massive latency and distoration */
    /* Make sure that every single sample was played */
    /*if (pa_simple_drain(playback_s, &playback_error) < 0) {
        fprintf(stderr, __FILE__": pa_simple_drain() failed: %s\n", pa_strerror(playback_error));
        fatal(playback_s, 5);
    }*/

}

int get_mic_input(struct getmic_inet1 getmic_inet) {

int sockfd; 
struct sockaddr_in dest_addr;

sockfd = getmic_inet.sockfd1;
dest_addr = getmic_inet.dest_addr1;

   pa_sample_spec ss;

   ss.format = PA_SAMPLE_S16LE;
   ss.rate = 44100;
   ss.channels = 2;

    pa_simple *s;
    int ret;
    int error;

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, "VoIPMicInput", PA_STREAM_RECORD, NULL,
"record", &ss, NULL, NULL, &error))) {
        fprintf(stderr, __FILE__": pa_simple_new() failed: %s\n",
pa_strerror(error));
	close(sockfd);
        fatal(s, 1);
    }

    while(1) {

    uint8_t buf[BUFSIZE];

        /* Record some data ... */
    if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
    	fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n",
pa_strerror(error));
	close(sockfd);
        fatal(s, 2);
        }
        inet_sendto(sockfd, buf, sizeof(buf), dest_addr);
    }
    return 0;
}
