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
//#include "SendRecv.c"
#include "SendRecv.h"
//#include "VoIPSendRecv.h"
//#include "PulseAudioMicInput.h"

void fatal(pa_simple *s, int exit_code) {

    if (s)
        pa_simple_free(s);

        exit(exit_code);

}

/* If any of the three if statements are true, fatal() will be called
* And the program will exit without closing any of it's sockets.
* Thus creating a bind error on the next run of this program,
* And leaving a socket open on the system, This needs to be fixed.  */

void playback_connected_input(struct playback_sound playback) {
    
    void *data;
    size_t bytes;
    
    data = playback.call_sound;
    bytes = playback.call_sound_len;
    
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
        return;
}

int get_mic_input(void) {

    FILE *fp1;
    char *c;
    int z;
    while(1) {
    fp1 = fopen("participants-num.txt", "r");

    *c = getc(fp1);
    z = atoi(c);
    
    printf("%s\n %d\n", c, z);
    
    if(z == 0) {
        usleep(5000);
    }
    if(z > 0) {
        break;
    }
    }
    
   struct getmic_inet1 get_mic;
    
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
        fatal(s, 1);
    }

    while(1) {

    uint8_t buf[BUFSIZE];

        /* Record some data ... */
    if (pa_simple_read(s, buf, BUFSIZE, &error) < 0) {
    	fprintf(stderr, __FILE__": pa_simple_read() failed: %s\n",
pa_strerror(error));
        fatal(s, 2);
        }
        /* All that really matters if the buf variable */
        memcpy(get_mic.buf, buf, sizeof(buf));
        get_mic.buf_len = BUFSIZE;
        inet_sendto(get_mic);
    }
    return 0;
}
