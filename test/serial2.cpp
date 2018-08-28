#include <stdio.h>

#include <pcrdef.h>
#include <pcomm.h>

void main ()
{
	PComm *elmo = new PComm ("/dev/ttyS0");
	char message[255];
	elmo->PCTell(PCRSQLO);
	fprintf(stderr, "sizeof(message): %i\n", sizeof(message));
	elmo->PCHear(message);
	fprintf(stderr, "I heard this \"%s\" in main()\n", message);
	sleep(5);
	elmo->PCTell(PCRSQLC);
	elmo->PCHear(message);
	fprintf(stderr, "I heard this \"%s\" in main()2\n", message);
}
