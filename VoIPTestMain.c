#include <stdlib.h>
#include "VoIPSendRecv.h"

int main(int argc, char**argv) {
	/* Make both of us threads */
	if(argc >= 2) {
		request_connection(argv[1]);
	}
	wait_for_connection();
}