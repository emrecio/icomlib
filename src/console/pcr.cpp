/*
   Copyright (C) 1999, 2000 PolyWog and Javaman for Ghetto.Org
   This file is the command line component of the PCR1000 Control Suite.

   The PCR-1000 Control Suite is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The PCR-1000 Control Suite is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the PCR-1000 API Library; see the file LICENSE.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
 */

/* 
 * This file declars the PCR-1000 command line interface for the
 * Ghetto.org radio toolkit.  It implements as much of the functionality
 * as possible as defined in the API
 *
 */

#include <pcp.h>
#include <plog.h>
#include <ctype.h>

#if defined (SunOS) || defined (Irix)
#include <strings.h>
#else 
#include <string.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

pcrfreq_t freqconv(char *);  
	// Go from string with decimal to unsigned long int
pcrfreq_t stepconv(char *);
	// Same, but express it in khz rather than mhz.
void bargraph(char *, pcrfreq_t, pcrfreq_t, pcrfreq_t);
	// Generate the bargraph string seen in the scanner mode, copied to
	// the first argument.
void formatfreq(char *, pcrfreq_t);
	// Reverse of freqconv(char *): convert unsigned long int to a
	// character string, and copy it to the first argument.
void cmdhelp(void);

int main (int argc, char **argv)
{
	bool scanmode = false;	// Turn on scanner mode if set.
	bool unatten = false; // Turn on logging.
	bool replay = false;// Load from file and rerun.
	bool agc = false;   // Store setting of AGC after parsing and before transmission.
	bool nb = false;  	// Same as above, but for noise blanker.
	bool rf = false;   	// Same as above, but for RF attenuation.
	char *hz, *frac_hz;	// Pointers used in the parsing of the frequency.
	char upperword[4]; 	// Variable for converting lowercase to uppercase.
	char c[128];        // Temp variable for a read.
	char temp[80];		// Temp variable for logging and printing to the screen.
	int vol = 70;      	// Same as for AGC, but for volume.
	int sql = 40;      	// Same as for AGC, but for squelch.
	int filnum = 15;   	// Same as for AGC, but for the filter IF statements.
	int i, j;          	// Loop counter, used multiple times.
	int wait = 5;		// How long to wait in the single frequency mode.
	int hpoint = 80;	// Hold point for the scanner.
	float tonesq = 0;  	// Same as for AGC, but for the tone squelch.
	pcrfreq_t lwrfreq;  // unsigned long int lower frequency.
	pcrfreq_t uprfreq;  // unsigned long int upper frequency.
	pcrfreq_t curfreq;  // unsigned long int current frequency.
	pcrfreq_t step;     // unsigned long int step size.
	pcrcmd_t filter[11] = PCRFLTR15; // Same as for AGC, but for IF filter.
	pcrcmd_t mod[10];                // Same as for AGC, but for mod format.
	pcrcmd_t port[128] = PCRDEV;	 // Port setting for the pcr1000.

	PLog *unatten_p;				 // Pointer to the PLog unattended log file.
	PLog *replay_p;					 // Pointer to the PLog replay log file.

	memset(mod, '\0', sizeof(mod));
	
	/*
	 * If the command is issued with no arguments, issue instructions.
	 * Otherwise, instantiate a new class, parse data, and issue commands.
	 */

	if (argc > 1) {

		/*
		 * Thanks to someone in #cdc who wrote an icmp flooder,
		 * I am now able to use getopt.
		 */

		while ((i = getopt(argc, argv, "sanrv:h:w:t:m:q:f:p:u:l:")) != EOF) {
			switch (i) {
				// Case statement for each parameter.

				case 'v':
					vol = atoi(optarg);
					// Check for valid volume range.  If not, set to 70.
					if (vol > 99 || vol < 0) {
						vol = 70;
						fprintf(stderr, "Invalid volume setting.  Volume set to %i.\n", vol);
					}
					break;

				case 's':
					scanmode = true;  // Turn on scan mode if -s flag is set.
					break;

				case 'a':
					agc = true;  // Turn on AGC if -a flag is set.
					break;
				
				case 'n':
					nb = true;  // Turn on noise blanker if -n flag is set.
					break;

				case 'r':
					rf = true;  // Turn on RF Attenuator if -r flag is set.
					break;

				case 'h':
					hpoint = atoi(optarg);
					// Check for valid hold point range.  If not, set to 80.
					if (hpoint > 255 || hpoint < 0) {
						hpoint = 80;
						fprintf(stderr, "Invalid halt point setting.  Halt point set to %i.\n", hpoint);
					}
					break;

				case 'w':
					wait = atoi(optarg);
					// Check for pause time validity
					if (wait < 1) {
						wait = 5;
						fprintf(stderr, "Invalid wait time.  Wait time set to %i.\n", wait);
					}
					break;

				case 't':
					// Convert a string value with a decimal to float.
					hz = optarg;
					// Get pointer to decimal. 
					frac_hz = strchr(optarg, '.');
					// Cast float of the integer part of the tone squelch.		
					tonesq = (float) atoi(hz);
	
					/*
					 * If no decimal part is present, break.
					 * Otherwise, convert string that comprises
					 * the fraction into a float and adds it to the
					 * previous title.
					 */
					if (frac_hz == NULL) {
						break;
					} else {
						*frac_hz = '\0';
						frac_hz++;
						tonesq += ((float) atoi(frac_hz))/10;
						break;
					}

				case 'q':
					// Check for valid squelch range.  If not, set to 70.
					sql = atoi(optarg);
					if (sql > 99 || vol < 0) {
						sql = 40;
						fprintf(stderr, "Invalid squelch setting.  Squelch set to %i.\n", sql);
					}	
					break;

				case 'f':
				/*
			 	 * The only possible filter options are 3, 6, 15, 50, and 230
				 * kHz.  The user enters these values in the command line.
				 * Otherwise, the code defaults to 15 kHz.
				 */
					filnum = atoi(optarg);

					if ((filnum == 3) || (filnum == 6) || (filnum == 15) || (filnum == 50) || (filnum == 230)) {
						strcpy(filter, optarg);
					} else {
						strcpy(filter, "15");
						fprintf(stderr, "Invalid filter setting.  IF Filter set to 15 kHz.\n");
					}
					break;
				
				case 'p':
					// Grab the port that the radio is attached to.
					strcpy(port, optarg);
					break;

				case 'u':
					// Switch on unattended mode.
					unatten = true;
					if (optarg == NULL) {
						fprintf(stderr, "Please specify a file name, exiting...\n");
						exit(-1);
					}
					//strcpy(logfile, optarg);
					unatten_p = new PLog(optarg, "w");
					break;
					
				case 'l':
					// Switch on replay mode.
					replay = true;
					if (optarg == NULL) {
						fprintf(stderr, "Please specify a file name, exiting...\n");
						exit(-1);
					}
					//strcpy(replayfile, optarg);
					replay_p = new PLog(optarg, "r");
					break;

				case 'm':
					/*
					 * The first part of the following code coverts
					 * the command lime parameter to an uppercase statement.
					 * The rest determines what the actual mode passed was.
					 */
					for (j = 0; optarg[j] != '\0'; j++) {
						upperword[j] = (char) toupper(optarg[j]);
					}

					upperword[j] = '\0';
					
					if (strncmp(upperword, "LSB", 3) == 0) {
						strcpy(mod, "LSB");
					} else if (strncmp(upperword, "USB", 3) == 0) {
						strcpy(mod, "USB");
					} else if (strncmp(upperword, "AM", 2) == 0) {
						strcpy(mod, "AM");
					} else if (strncmp(upperword, "CW", 2) == 0) {
						strcpy(mod, "CW");
					} else if (strncmp(upperword, "UNK", 3) == 0) {
						strcpy(mod, "UNK");
					} else if (strncmp(upperword, "NFM", 3) == 0) {
						strcpy(mod, "NFM");
					} else if (strncmp(upperword, "WFM", 3) == 0) {
						strcpy(mod, "WFM");
					} else {
						fprintf(stderr, "Invalid mode found.  Using NFM by default.\n");
						strcpy(mod, "NFM");
					}
					break;
			}
		}

		// If there is only one argument and radio is not in 
		// scanmode and not in replay mode, make sure it is a valid frequency.
		
		if (!scanmode && !replay) {
			curfreq = freqconv(argv[optind]);
			if (curfreq < 50000 || curfreq > 1300000000) {
				fprintf(stderr, "Error: Cannot process frequency.\n\n");
				cmdhelp();
				exit(-1);
			}
		}

		// If it is in scanmode, and there are three arguments, then check
		// for validity and let the program continue.

		if (scanmode) {
			lwrfreq = freqconv(argv[optind]);
			uprfreq = freqconv(argv[optind+1]);
			step = stepconv(argv[optind+2]);

			if (lwrfreq < 50000 || lwrfreq > 1300000000 || uprfreq < 50000 || uprfreq > 1300000000 || step <= 0) {
				fprintf(stderr, "Error: Cannot process frequencies.\n\n");
				cmdhelp();
				exit(-1);
			}
		}
		
		// Pass all the commands to the radio.
		// Instantiate the class.
		PCP *rig = new PCP (port, BAUDRATE, "RIG"); 

		if(!rig->PCPQueryOn()) {
			if(rig->PCPInit()) {
				fprintf(stderr, "Error: Cannot initialize radio.  Check port.\n\n");
				cmdhelp();
				exit(-1);
			}
			//Start up the radio.
			rig->PCPPowerUp();
		}

		// Send non-mode (scanner/single frequency) specific commands to
		// the radio.
		if(!rig->PCPSetVolume(vol)) {
			fprintf(stderr, "Error: Cannot set volume, exiting.\n");
			cmdhelp();
			exit(-1);
		}
		if(!rig->PCPSetSquelch(sql)) {
			fprintf(stderr, "Error: Cannot set squelch, exiting.\n");
			cmdhelp();
			exit(-1);
		}
		if(!rig->PCPSetToneSq(tonesq)) {
			fprintf(stderr, "Error: Cannot set tone squelch, exiting.\n");
			cmdhelp();
			exit(-1);
		}
		if(!rig->PCPSetAutoGain(agc)) {
			fprintf(stderr, "Error: Cannot set AGC, exiting.\n");
			cmdhelp();
			exit(-1);
		}
		if(!rig->PCPSetNB(nb)) {
			fprintf(stderr, "Error: Cannot set NB, exiting.\n");
			cmdhelp();
			exit(-1);
		}
		if(!rig->PCPSetRFAttenuator(rf)) {
			fprintf(stderr, "Error: Cannot set RF Attenuator, exiting.\n");
			cmdhelp();
			exit(-1);
		}

		// If not in scanner mode and not in replay mode, set the
		// frequency and modulation method.
		if(!scanmode && !replay) {
			if(!rig->PCPSetFreq(curfreq)) {
				fprintf(stderr, "Error: Cannot set frequency, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			if(!rig->PCPSetMode(mod)) {
				fprintf(stderr, "Error: Cannot set mode, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			if(!rig->PCPSetFilter(filter)) {
				fprintf(stderr, "Error: Cannot set filter, exiting.\n");
				cmdhelp();
				exit(-1);
			}

			// If in unattended mode, log the time and the power level to
			// a file continuously.
			if (unatten) {
				while (1) {
					unatten_p->LogFreq(curfreq, rig->PCPSigStrength());
					sleep(wait);
				}
			} else {
				// Otherwise, print out the sig strength and quit.
				formatfreq(temp, curfreq);
				printf("Command successful.\nFrequency: %s", temp);
				printf("; Signal Strength: %i.\n", rig->PCPSigStrength());
				return(0);
			}
		}
	
		// Enter replay mode loop
		if(replay) {
			
			// Try to set the frequency, modulation, and filter information.
			if(!rig->PCPSetFreq(replay_p->ReturnChan(0))) {
				fprintf(stderr, "Error: Cannot set initial frequency, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			if(!rig->PCPSetMode(mod)) {
				fprintf(stderr, "Error: Cannot set mode, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			if(!rig->PCPSetFilter(filter)) {
				fprintf(stderr, "Error: Cannot set filter, exiting.\n");
				cmdhelp();
				exit(-1);
			}

			// If not in unattended mode, set stdin to nonblocking so
			// that the program can listen for a carriage return while
			// grabbing the signal strength.  This is so a user
			// can skip over a frequency that is always busy.
			if (!unatten) {
			    if(fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) < 0){
					fprintf(stderr, "Unable to set stdin to nonblocking\n");
					exit(1);
				}
			}
			while (1) {
				// Continuously loop the memory channels, so to speak.
				for (i = 0; i < replay_p->ChanCount(); i++) {
					// Go through the previously malloc'ed array. 
					if(!rig->PCPSetFreq(replay_p->ReturnChan(i))) {
						fprintf(stderr, "Error: Cannot set current frequency %i hz, exiting.\n", replay_p->ReturnChan(i));
						exit(-1);
					}
					// Generate a bargraph and the current frequency.
					bargraph(temp, 0, replay_p->ChanCount(), i);
					printf("Scanning... %s ", temp);
					formatfreq(temp, replay_p->ReturnChan(i));
					printf("%s\r", temp);
					// Before looping in hold mode, grab the signal
					// strength.
					j = rig->PCPSigStrength();
					fflush(stdout);
					if(j >= hpoint) {
						// Grab the signal strength again, just in case
						// the signal drops carrier.
						j = rig->PCPSigStrength();
						formatfreq(temp, replay_p->ReturnChan(i));
						printf("Hit......   Press ENTER to contine.         %s\r", temp);
						fflush(stdout);
						// If we are in unattended mode, log it.
						if (unatten) {
							unatten_p->LogFreq(replay_p->ReturnChan(i), j);
						} else {
							while (j >= hpoint) {
								// Exit on carriage return.
								if(read(STDIN_FILENO, c, 128) < 0) {
									if(errno == EAGAIN) {
										sleep(1);
										j = rig->PCPSigStrength();
										continue;
									} else {
										fprintf(stderr, "Error in read(), exiting...\n\n");
										exit(1);
									}
								} else {
									break;
								}
							}
						}
					}
				}
			}
		}
	
		// If we are in scanner mode...
		if(scanmode) {
			// Try to set the frequency, modulation, and filter information.
			if(!rig->PCPSetFreq(lwrfreq)) {
				fprintf(stderr, "Error: Cannot set initial frequency, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			if(!rig->PCPSetMode(mod)) {
				fprintf(stderr, "Error: Cannot set mode, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			if(!rig->PCPSetFilter(filter)) {
				fprintf(stderr, "Error: Cannot set filter, exiting.\n");
				cmdhelp();
				exit(-1);
			}
			
			printf("Beginning scan...\n");

			// Do that handy dandy unblocking thing again to stdin for
			// scanner mode now.  This was previously commented.
			if (!unatten) {
			    if(fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) < 0){
					fprintf(stderr, "Unable to set stdin to nonblocking\n");
					exit(1);
				}
			}

			while (1) {
				// Loop through the frequency set by the set list.
				for (curfreq = lwrfreq; curfreq <= uprfreq; curfreq += step) {
					if(!rig->PCPSetFreq(curfreq)) {
						fprintf(stderr, "Error: Cannot set current frequency %i hz, exiting.\n", curfreq);
						exit(-1);
					}
					bargraph(temp, lwrfreq, uprfreq, curfreq);
					printf("Scanning... %s ", temp);
					formatfreq(temp, curfreq);
					printf("%s\r", temp);
					fflush(stdout);
					i = rig->PCPSigStrength();

					if(i >= hpoint) {
						i = rig->PCPSigStrength();
						formatfreq(temp, curfreq);
						printf("Hit......   Press ENTER to contine.         %s\r", temp);
						fflush(stdout);

						if (unatten) {
							unatten_p->LogFreq(curfreq, rig->PCPSigStrength());
						} else {
							while (i >= hpoint) {
								if(read(STDIN_FILENO, c, 128) < 0) {
									if(errno == EAGAIN) {
										sleep(1);
										i = rig->PCPSigStrength();
										continue;
									} else {
										fprintf(stderr, "Error in read(), exiting...\n\n");
										exit(1);
									}
								} else {
									break;
								}
							}
						}
					}
				}
				if (unatten) {
					break;
					printf("\n");
				}
			}
		}
	} else {
		cmdhelp();
	}
	return 0;
}

void cmdhelp(void)
{
	printf("\npcr version 0.9.2 written by Polywog and Javaman.\n");
	printf("Usage is:\n");
	printf("\tSingle shot:\n");
	printf("\tpcr [-p<port>] [-u<logfile>] [-w<secs>] [-v<volume>] [-q<squelch>] [-f<filter>] [-m<mode>] [-t<freq>] [-a] [-n] [-r] freq (in MHz)\n");
	printf("\tScan mode:\n");
	printf("\tpcr -s [-u<logfile>] [-p<port>] [-v<volume>] [-q<squelch>] [-f<filter>] [-m<mode>] [-t<freq>] [-a] [-n] [-r] -h<hold> lower_freq upper_freq (in MHz) step_freq (in kHz)\n");
	printf("\tReplay mode:\n");
	printf("\tpcr -l [-p<port>] [-v<volume>] [-q<squelch>] [-f<filter>] [-m<mode>] [-t<freq>] [-a] [-n] [-r] -h<hold> <logfile>\n");
	printf("Options:\n");
	printf("   -a\t= Turn AGC on\n");
	printf("   -f\t= Set filter (3, 6, 15, 50, 230)\n");
	printf("   -h\t= Hold point for scanning/replay mode (0..255)\n");
	printf("   -l\t= Replay mode\n");
	printf("   -m\t= Set mode (usb, lsb, am, cw, nfm, wfm)\n");
	printf("   -n\t= Turn the noise blanker on\n");
	printf("   -p\t= Set port (/dev/pcr1000 by default)\n");
	printf("   -q\t= Set squelch (00 through 99)\n");
	printf("   -r\t= Turn the RF attenuator on\n");
	printf("   -s\t= Turn on scanner mode\n");
	printf("   -t\t= Set tone squelch (103.5, 141.3)\n");
	printf("   -u\t= Switch into unattended mode (filename)\n");
	printf("   -v\t= Set volume (00 through 99)\n");
	printf("   -w\t= Wait time between strength check in single freq mode (seconds).\n");
	printf("Standard mode: \n");
	printf("   freq\t= Frequency in megahertz\n");
	printf("Example:\n\tpcr -p/dev/ttyS0 -v70 -q40 -f15 -mnfm -w10 146.52\n");
	printf("Scanner mode: \n");
	printf("   lower_freq\t= Lower frequency in megahertz\n");
	printf("   upper_freq\t= Upper frequency in megahertz\n");
	printf("   step_freq\t= Step frequency in kilohertz\n");
	printf("Example:\n\tpcr -p/dev/ttyS0 -v70 -q40 -f15 -mnfm -h70 -u2m.log -s 144 148 5\n"); 
	printf("Replay mode: \n");
	printf("   <logfile>\t= File name of old log to replay\n");
	printf("Example:\n\tpcr -p/dev/ttyS0 -v70 -q40 -f15 -mnfm -h70 -l 2m.log\n"); 
}

/*
 * Convert a frequency string to a long int.
 *
 * The following is to parse the frequency out into MHz and kHz
 * components.  A pointer is placed where the decimal is
 * and all data from that point forward is considered to be
 * the kHz component.
 */
pcrfreq_t freqconv(char *strfreq)
{
	pcrfreq_t tempfreq;
	char *mhz, *khz;
	int i;

	mhz = strfreq;
	khz = strchr(strfreq, '.');

	if (khz == NULL) {
		if (strlen(mhz) > 4) {
			fprintf(stderr, "Malformed frequency: %s\n", strfreq);
			return 0;
		}
		tempfreq = atoi(mhz) * 1000000;
		return tempfreq;
   	}
   	
	*khz = '\0';
   	khz++;
	tempfreq = atoi(khz);

	for (i = 5; i >= strlen(khz) ; i--) {
		tempfreq *= 10;
	}

	tempfreq += atoi(mhz) * 1000000;

	return tempfreq;
}

// Same as above, but for kHz.
pcrfreq_t stepconv(char *strstep)
{
	int step;
	char *khz, *hz;
	int i;

	khz = strstep;
	hz = strchr(strstep, '.');

	if (hz == NULL) {
		return atoi(khz) * 1000;
	}

	*hz = '\0';
	hz++;

	step = atoi(hz);

	for (i = 2; i >= strlen(hz); i--) {
		step *= 10;
	}

	step += atoi(khz) * 1000;

	return step;
}

// Produce the bargraph used in the scanner.
void bargraph(char *temp, pcrfreq_t lwrfreq, pcrfreq_t uprfreq, pcrfreq_t curfreq)
{
	int length;
	int i;
	int step;

	memset(temp, '\0', sizeof(temp));	

	length = (int)((float)(curfreq - lwrfreq) / (float)(uprfreq - lwrfreq) * 30);
	for (i = 0; i < length; i++) {
		strcat(temp, "=");
	}
	strcat(temp, ">");

	for (i++; i <= 30; i++) {
		strcat(temp, ".");
	}
}

void formatfreq(char *temp, pcrfreq_t curfreq)
{
	memset(temp, '\0', sizeof(temp));	
	sprintf(temp, "%i.%06i", curfreq/1000000, curfreq - ((int)(curfreq/1000000))*1000000);
}
