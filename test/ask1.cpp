#include <stdio.h>

#include <pcrdef.h>
#include <pcomm.h>

#include <string.h>

void main ()
{
	PComm *elmo = new PComm ("/dev/ttyS0");
	char message[255];
	/*elmo->PCTell(PCRINITM);
	fprintf(stderr, "tell: %s\n", PCRINITM);
	elmo->PCClose();
	elmo->PCOpen("/dev/ttyS0");

	char frequency[256];
	strcpy(frequency, PCRFRQ);
	strcat(frequency, "0162475000050200");
	elmo->PCTell(frequency);
	fprintf(stderr, "tell: %s\n", frequency);
	elmo->PCHear(message);
	fprintf(stderr, "hear: %s\n", message);*/

	elmo->PCTell(PCRVOLON);
	fprintf(stderr, "tell: %s\n", PCRVOLON);
	elmo->PCHear(message);
	fprintf(stderr, "hear: %s\n", message);

	elmo->PCAsk(PCRQRST);
	fprintf(stderr, "tell: %s\n", PCRQRST);
	elmo->PCHear(message);
	fprintf(stderr, "hear: %s\n", message);
	
}
