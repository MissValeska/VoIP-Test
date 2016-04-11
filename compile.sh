<<<<<<< HEAD
clang VoIPTestMain.c -o VoIPTestMain -g SendRecv.c PulseAudioMicInput.c -lpulse-simple -lpulse -lpthread
=======
clang VoIPTestMain.c -o VoIPTestMain -g -L. VoIPSendRecv.c PulseAudioMicInput.c -lpulse-simple -lpulse -lpthread
>>>>>>> a4150c9c0e81a09740cd44a5eabad1664830588b
