#include <pcp.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>

int main (int argc, char **argv)
{
	char *mhz, *khz;
	char upperword[4];
	int vol = 60;
	int sql = 40;
	pcrfreq_t freq = 100300000;
	pcrcmd_t filter[] = "230";
	pcrcmd_t mod[] = "WFM";
	char foo[256];

	PCP *rig;
	rig = new PCP (PCRDEV,B9600);
	printf("Executing PCPInit()\n");  	rig->PCPInit();
	printf("Executing PCPPowerUp()\n"); 	rig->PCPPowerUp();
	printf("Executing PCPSetSpeed()\n");	rig->PCPSetSpeed(B38400);
	delete rig;
	rig = new PCP (PCRDEV,B38400);
	sleep(1);
	printf("Executing PCPSetVolume()\n"); 	rig->PCPSetVolume(vol);
	printf("Executing PCPSetSquelch()\n"); 	rig->PCPSetSquelch(sql);
	printf("Executing PCPSetFreq()\n"); 	rig->PCPSetFreq(freq);
	printf("Executing PCPSetMode()\n"); 	
	 if (!rig->PCPSetMode(mod))
		fprintf(stderr, "***********ERROR************");
	printf("Executing PCPSetFilter()\n"); 	rig->PCPSetFilter(filter);
	rig->PCPSetSpeed(B9600);
	return(0);
}
