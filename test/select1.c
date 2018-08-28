#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h> /* for the timeval struct */
#include <unistd.h>   /* for the select() stuff */
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

/* baudrate settings are defined in <asm/termbits.h>, which is
included by <termios.h> */
#define BAUDRATE B9600
/* change this definition for the correct port */
#define PCRDEV "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define FALSE 0
#define TRUE 1

volatile int STOP=FALSE; 

main(int argc, char *argv[])
{
        int fd,a,b,c, res;
        struct termios oldtio,newtio;
	char buf[256];
	char buf2[256];
	char buf3[256];

	/* select stuff for a timeout */
	int ok; /* to see if it's ok to read/write */
	fd_set *mainSet;
	fd_set *readSet;
	fd_set *writeSet;
	struct timeval *mainTimeOut;

	mainSet = malloc(sizeof(fd_set));
	readSet = malloc(sizeof(fd_set));
	writeSet = malloc(sizeof(fd_set));
	bzero(mainSet, sizeof(fd_set));
	bzero(readSet, sizeof(fd_set));
	bzero(writeSet, sizeof(fd_set));

	mainTimeOut = malloc(sizeof(struct timeval));
	bzero(mainTimeOut, sizeof(struct timeval));
	/* end select stuff for a timeout */

        /* Open modem device for reading and writing and not as controlling tty
        because we don't want to get killed if linenoise sends CTRL-C.  */
        fd = open(PCRDEV, O_RDWR | O_NOCTTY ); 
        if (fd <0) {perror(PCRDEV); exit(-1); }

        tcgetattr(fd,&oldtio); /* save current serial port settings */
	bzero(&newtio, sizeof(newtio)); 

        /* 
         BAUDRATE: Set bps rate. use cfsetispeed and cfsetospeed.
         CS8     : 8n1 (8bit,no parity,1 stopbit)
         CLOCAL  : local connection, no modem contol
         CREAD   : enable receiving characters
        */
	cfsetispeed( &newtio, BAUDRATE );
	cfsetospeed( &newtio, BAUDRATE );
        newtio.c_cflag |=  (CLOCAL | CREAD);
	newtio.c_cflag &= ~HUPCL ;
	newtio.c_cflag &= ~PARENB ;
	newtio.c_cflag &= ~CSTOPB ;
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;
       
        /* ICANON  : enable canonical input disable all echo functionality, 
		  and don't send signals to calling program */
        newtio.c_lflag |= ICANON;
	newtio.c_lflag &= ~(ECHO | ECHOCTL);
 
        /* Raw output.  */
        newtio.c_oflag &= ~OPOST;
       
        /*
         IGNPAR  : ignore bytes with parity errors
         ICRNL   : map CR to NL (otherwise a CR input on the other computer
                   will not terminate input)
          otherwise make device raw (no other input processing)
 	newtio.c_iflag = IGNPAR ;
        */
       
        /* initialize control characters */
        /* newtio.c_cc[VTIME]    = 5; *//* inter-character timer unused */
        /* newtio.c_cc[VMIN]     = 0; *//* blocking read until 1 character arrives */

        /* now clean the modem line and activate the settings for the port */
        /* tcflush(fd, TCIFLUSH); */
        tcsetattr(fd,TCSANOW,&newtio);

	/* here goes the select stuff */
	FD_ZERO(mainSet); 
	FD_ZERO(readSet);
	FD_ZERO(writeSet);
	FD_SET(fd, mainSet);
	FD_SET(fd, readSet);
	FD_SET(fd, writeSet);
	mainTimeOut->tv_sec=5;
	mainTimeOut->tv_usec=0;

	
	
	if (argc > 1) {
		a = 0;
		b = 0;
		strcpy(buf, "G300\x0d\x0a");
		if (select(fd+1, NULL, writeSet, NULL, mainTimeOut)) {
			write( fd, buf, strlen(buf)); 
			fprintf(stderr, "\tA\tWriting ok to %d\n", fd);
    		if (select(fd+1, readSet, NULL, NULL, mainTimeOut)) {
	    		a = read(fd, buf, 0xff);
		    	fprintf(stderr, "\tA\tReading ok to %d\n", fd);
		    	/* second loop for data */
        		strcpy(buf, "G4\?\x0d\x0a");
		        fprintf(stderr, "REQ: %s\n", buf);
        		if (select(fd+1, NULL, writeSet, NULL, mainTimeOut)) {
		        	write( fd, buf, strlen(buf));
        			fprintf(stderr, "\tB\tWriting ok to %d\n", fd);
            		if (select(fd+1, readSet, NULL, NULL, mainTimeOut)) {
                		b = read(fd, buf2, 0xff);
		    	        fprintf(stderr, "\tB\tReading ok to %d\n", fd);
		    	    } else {
		    	        fprintf(stderr, "\tB\tReading timeout %d\n", fd);
		    	    }
		        } else {
        			fprintf(stderr, "\tB\tWriting timeout %d\n", fd);		    	
        		}
    		} else {
	    		fprintf(stderr, "\tA\tReading timeout %d\n", fd);
		    }
		} else {
			fprintf(stderr, "\tA\tWriting timeout %d\n", fd);
		}
		buf[a]=0;buf2[b]=0;
		fprintf(stderr, "\t \tRED: %i,%s %i,%s\n", a,buf,b,buf2);
	}

   /* restore the old port settings */
   tcsetattr(fd,TCSANOW,&oldtio);
}
