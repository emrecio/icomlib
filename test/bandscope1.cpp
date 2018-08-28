#include <stdio.h>

#include <pcrdef.h>
#include <pcomm.h>

#include <string.h>

void main ()
{
	PComm *elmo = new PComm ("/dev/ttyS0");
	char message[255];

	elmo->PCRawWrite("H101\x0d\x0a");
	elmo->PCRawWrite("G103\x0d\x0a");
	/* delete elmo;
	elmo = new PComm("/dev/ttyS0", B38400);*/
	elmo->PCClose();
	elmo->PCOpen("/dev/ttyS0");
	elmo->PCRawWrite("G300\x0d\x0a"); printf("sent autoupdate off\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("H1\?\x0d\x0a"); printf("sent are you on?\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("H101\x0d\x0a"); printf("sent radio on\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("G4\?\x0d\x0a"); printf("sent version?\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("GE\?\x0d\x0a"); printf("sent euro/am?\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("GD\?\x0d\x0a"); printf("sent dsp?\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	// elmo->PCRawWrite("G301\x0d\x0a"); printf("sent autoupdate on\n");

	strcpy(message, PCRFRQ);
	strcat(message, "0449775000050200\x0d\x0a");
	elmo->PCRawWrite(message); printf("sent 449775f\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);

	elmo->PCRawWrite("J4100\x0d\x0a"); printf("sent sql = 0\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J5100\x0d\x0a"); printf("sent tsql = 0\n");	
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J5000\x0d\x0a"); printf("sent VSC off\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J4380\x0d\x0a"); printf("sent IF Shift = midrange\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J4500\x0d\x0a"); printf("sent AGC off\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J4600\x0d\x0a"); printf("sent NB off\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J4700\x0d\x0a"); printf("sent RFA off\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J4A80\x0d\x0a"); printf("sent J4A80\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("LD820\x0d\x0a0"); printf("sent Tracking filter auto\n");
//	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J8001J8101J8200J8301\x0d\x0a"); printf("sent DSP Packet");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
	elmo->PCRawWrite("J4060\x0d\x0a"); printf("sent volume on\n");
	elmo->PCRawRead(message); printf("got data: %s\n", message);
//	elmo->PCRawWrite("G301\x0d\x0a"); printf("sent autoupdate on\n");
	elmo->PCRawWrite("ME0000120050100012500\x0d\x0a"); printf("sent bandscope on\n");

	elmo->PCRawRead(message);
	fprintf(stderr, "hear: %s\n", message);
	elmo->PCRawRead(message);
	fprintf(stderr, "hear: %s\n", message);
	elmo->PCRawRead(message);
	fprintf(stderr, "hear: %s\n", message);
	elmo->PCRawRead(message);
	fprintf(stderr, "hear: %s\n", message);
	elmo->PCRawRead(message);
	fprintf(stderr, "hear: %s\n", message);
}
