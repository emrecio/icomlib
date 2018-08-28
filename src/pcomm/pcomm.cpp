/*
   Copyright (C) 1999, 2000 PolyWog and Javaman for Ghetto.Org
   This file is part of the PCR-1000 API Library.

   The PCR-1000 API Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The PCR-1000 API Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the PCR-1000 API Library; see the file LICENSE.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
 */

#if defined (SunOS) || defined (Irix)
#include <strings.h>
#else if defined (BSD) || defined (Linux)
#include <string.h>	/* bzero(), strcpy(), strcpy() etc... */
#endif
#include <assert.h> /* assert() */
#include "pcomm.h"

PComm :: PComm (char *pcrDev, tcflag_t baudRate, const char *name)
{
    if (name==NULL) {
        strcpy(int_name, "PComm_Obj");
    } else {
        strcpy(int_name, name);
    }

    callCount=0;
    bzero(pcrDevice, sizeof(pcrDevice));
    strncpy(pcrDevice, pcrDev, sizeof(pcrDevice)-1);
    pcrSpeed = baudRate;

    if (!PCOpen(pcrDev, baudRate))
        { exit(-1); }

}
/*! \fn PComm::PComm(char *pcrDev, tcflag_t baudRate=B9600, const char *name=0)
	\brief The PComm Constructor
	\param pcrDev           name of device to be opened (char string)
	\param baudRate         speed of device to be opened
	\param name             internal object name (for your use)
 
	This is the easiest way to open the serial device for
	communications with the radio. If the serial device cannot
	be opened for processing the object will abort with a -1 status
	code to the operating system.
	
	You must send it at least the device name which is to be
	opened as the first argument. Optionally, you can send it
	an initial \a baudrate and an internal object \a name.
*/

PComm :: ~PComm ()
{
	PCClose();

#ifdef DEBUG_VER_
    fprintf(stderr, "%s: Destroyed\n", int_name);
#endif // DEBUG_VER_ //
}
/*! \fn PComm::~PComm()
	\brief The PComm Destructor

	Destroys the object, and closes any open file descriptors
	to the serial device. It also restores any old (initial)
	settings to the serial device.
*/


bool PComm :: PCOpen(const char *pcrDev, tcflag_t baudRate)
{

    /* Open modem device for reading and writing and not as controlling tty
       because we don't want to get killed if linenoise sends CTRL-C.  */
    fd = open(pcrDev, O_RDWR | O_NOCTTY );
    if (fd <0) {
		perror(pcrDev); 
		return false; 
#ifdef DEBUG_VER_
	} else {
		fprintf(stderr, "PCOMM: PCOpen(%s, %ld): Success at %d\n", 
			pcrDev, baudRate, fd);
#endif // DEBUG_VER_
	}	
	

    oldtio 	= new termios;
    newtio 	= new termios;


	// functions necessary to initialise socket sets for
	// use with the select() function(s).
	timeOut = new timeval;
	FDSet	= new fd_set;
	// clear out that memory.
	bzero(timeOut, sizeof(struct timeval));
	bzero(FDSet, sizeof(fd_set));
	// remove any socket descriptors if necessary
	// and add socket 'fd' to the FDSet
	FD_ZERO(FDSet);
	FD_SET(fd, FDSet);
	// set default timeout values to five seconds
	timeOut->tv_sec  = 5;
	timeOut->tv_usec = 0;
	// now we should be ready to use the select() statement.
	// just make sure that if the socket `fd' is closed
	// to repopulate the fd_set and to call select with
	// fd+1 as the first arg

    tcgetattr(fd,oldtio); /* save current serial port settings */
    bzero(newtio, sizeof(struct termios));

    /*
       BAUDRATE: Set bps rate. use cfsetispeed and cfsetospeed.
       CLOCAL  : local connection, no modem contol
       CREAD   : enable receiving characters
       ~HUPCL  : dont reset DTR to low when program finishes
       ~PARENB : dont enable parity bits
       ~CSTOPB : no stop bits
       ~CSIZE  :
       CS8     : 8n1 (8bit,no parity,1 stopbit)
     */
    cfsetispeed( newtio, baudRate );
    cfsetospeed( newtio, baudRate );
    newtio->c_cflag |=  (CLOCAL | CREAD);
    newtio->c_cflag &= ~HUPCL ;
    newtio->c_cflag &= ~PARENB ;
    newtio->c_cflag &= ~CSTOPB ;
    newtio->c_cflag &= ~CSIZE;
    newtio->c_cflag |= CS8;

    /* ICANON  : enable canonical input disable all echo functionality,
                 and don't send signals to calling program */
    newtio->c_lflag |= ICANON;
    newtio->c_lflag &= ~(ECHO | ECHOCTL);

    /* Raw output.  */
    newtio->c_oflag &= ~OPOST;

    /* now clean the modem line and activate the settings for the port */
    /* tcflush(fd, TCIFLUSH); */
    tcsetattr(fd,TCSANOW,newtio);

    return true;
}
/*! \fn PComm :: PCOpen(const char *pcrDev, tcflag_t baudRate)
	\brief Opens another file descriptor to the serial device.
	\param pcrDev 	Serial port device to be opened.
	\param baudRate	Initial serial speed.

	Opens a file descriptor to the device \a pcrDev (which is by
	default your system's PCRDEV define) at the speed \a baudRate
	(which is by default as delinated in your termios.h set to
	9600 baud).

	\b Warning: You can only call this as long as the object is
	alive. If you do destry the object remember to call this
	function only after setting up the port configuration
	correctly.

	\return true or false based on success value.
*/

void PComm :: PCClose()
{
    close(fd);

    /* restore the old port settings */
    tcsetattr(fd,TCSANOW,oldtio);
    delete oldtio;
    delete newtio;

    FD_ZERO(FDSet);
    delete FDSet;
    delete timeOut;   

}
/*! \fn PComm::PCClose() 
	\brief Closes the currently active file descriptor.

	Used in conjunction with PCOpen(...) so that you can release and
	regain the socket after an initialization procedure.
*/
	
size_pc PComm :: PCTell(char *mesg)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "Tell: %s\n", mesg);
    #endif /* DEBUG_VER_ */

    return (Write(mesg));
}
/*! \fn PComm::PCTell(char *mesg)
	\brief Issue a command string to the radio.
	\param mesg command string from pcrdef.h.

	Sends a command to the radio via the 
	custom made wrapper function Write() . This
	doesnt really alter the message at all
	for reading and writing to the radio.
	
	\return number of bytes actually written gets
	returned, or (-1) on error.

	\sa pcrdef.h 
 */

size_pc PComm :: PCAsk(char *mesg)
{
    #ifdef DEBUG_VER_2
    fprintf(stderr, "Ask: %s\n", mesg);
    assert(mesg!=0);
    assert(strlen(mesg)==2);
    #endif /* DEBUG_VER_ */

    bzero(askBuf, sizeof(askBuf));
    strcpy(askBuf, mesg);
    strcat(askBuf, PCRQST);

    return (Write(askBuf)-1);
}
/*! \fn PComm::PCAsk(char *mesg)
	\brief Issue a status request from the radio
	\param mesg a query command string

	This function makes sure that mesg isnt empty, 
	as well as making sure that mesg is two bytes
	long. Any queries sent to the radio should be
	the header of the information requested to be
	returned.

	It then zero's out the ask buffer, and copies
	the message into the ask buffer. Then it concatenates 
	the PCRQST command terminator, and
	calls Write() sending it the ask buffer.
	
	\return the number of bytes asked minus the PCRQST command terminator.

	\sa PCRQST pcrdef.h
 */

size_pc PComm :: PCHear(char *mesg)
{
    char *cut;

    #ifdef DEBUG_VER_2
    assert(mesg != 0);
    fprintf(stderr, "PComm: PCHEAR \n");
    #endif /* DEBUG_VER_ */

    hearCtr=0;
    bzero(hearBuf, sizeof(hearBuf));
    bzero(mesg, sizeof(mesg));

    hearCtr = Read(hearBuf);
    cut=strpbrk(hearBuf, PCRECMD);
    if (cut!=NULL) { 
        *cut=0; 
        hearCtr -= 2;
    }
	
    strcpy(mesg, hearBuf); 
    
    #ifdef DEBUG_VER_2
    fprintf(stderr, "Hear: %s\n", mesg);
    #endif /* DEBUG_VER_ */

    	if (errRead) {
		return (-1);
	} else { 
		return (hearCtr);
	}

}
/*! \fn PComm::PCHear(char *mesg)
	\brief Receive a repsonse from the radio
	\param mesg a character string long enough to hold a reply
	
	This function makes sure that mesg is pointing
	to a valid address (ie: is malloc'd) then it
	zero's out the message string and hearBuffer. It
	then calls Read()... remembering the number of
	bytes read, it cuts out the end-of-command marker
	read in (CR-LF) and copies what it heard into
	the message string. 

	If the radio kept spitting out LF chars, it checks 
	for the err bool.
	
	\return the number of bytes read, or -1 on err.

	\sa pcrdef.h
 */

size_pc PComm :: PCRawWrite(char *mesg)
{

    bzero(writeBuf, sizeof(writeBuf));
    strcpy(writeBuf, mesg);

    if (select(fd+1, NULL, FDSet, NULL, timeOut)) {
    	writeCtr = write(fd, writeBuf, strlen(writeBuf));
    } else {
#ifdef DEBUG_VER_
	    fprintf(stderr, "PCRawWrite Time Out on socket %d\n", fd);
#endif // DEBUG_VER_ 
	writeCtr = 0;
    }

    if ((unsigned)writeCtr!=strlen(writeBuf)) {
        return (-1);
    }

    return writeCtr;
   
}
/*! \fn PComm::PCRawWrite(char *mesg)
	\brief writes directly to the current socket.
	\param mesg the character string which it will write
	
	Sends a command directly to the socket without any preprocessing
	
	\return number of bytes actually written

	\sa pcrdef.h PCRawRead(char *mesg)
 */

size_pc PComm :: PCRawRead(char *mesg)
{
    readCtr = 1;

    bzero(readBuf, sizeof(readBuf));
    bzero(readBuf, sizeof(mesg));

    while(readCtr==1) {
	if (select(fd+1, FDSet, NULL, NULL, timeOut)) {
        	readCtr= read(fd, readBuf, sizeof(readBuf));
    		strcpy(mesg, readBuf);
	} else {
		readCtr=(-1);
#ifdef DEBUG_VER_
	    fprintf(stderr, "PCRawRead Time Out on socket %d\n", fd);
#endif // DEBUG_VER_ 

	}
    }

    return readCtr;
}
/*! \fn PComm::PCRawRead(char *mesg)
	\brief reads directly from the current socket.
	\param mesg malloc'd char string big enough to hold a reply

	Reads directly from the socket without any preprocessing
	
	\return number of bytes read.
	\sa pcrdef.h PCRawWrite(char *mesg)
 */


size_pc PComm :: Write(char *submesg)
{
    int __writeCtr;

    bzero(writeBuf, sizeof(writeBuf));
    strcpy(writeBuf, submesg);
    strcat(writeBuf, PCRECMD);

    #ifdef DEBUG_VER_2
    fprintf(stderr, "Write Attempt: %s\n", writeBuf);
    #endif /* DEBUG_VER_ */

    if (select(fd+1, NULL, FDSet, NULL, timeOut)) {
		__writeCtr=write(fd, writeBuf, strlen(writeBuf));
    } else {
#ifdef DEBUG_VER_
	    fprintf(stderr, "Write() Time Out on socket %d\n", fd);
#endif // DEBUG_VER_ 
	    __writeCtr=0;
    }

    return (__writeCtr-2);
}
/*! \fn PComm::Write(char *submesg)
	\brief Internal function to write to the socket
	\param submesg character string to write out to

	sends a command to the radio/socket. 

	It zero's out the internal class's write
	buffer, and copies the message passed in
	called submesg into the write buffer.
	It appends the special end-of-command marker 
	to the write buffer, and exec's write().
	Select() is used to see if we are ready to write
	to the socket in the FDSet. 

	\return what was actually written minus two. 
	Since the minus two is to account for the  
	end-of-command marker. On error we 
	return -2

	\sa Read()

 */

size_pc PComm :: Read(char *submesg)
{
    readCtr=1;
    int loopBreaker=0;

#ifdef DEBUG_VER_2
    assert(submesg!=0);
    fprintf(stderr, "PComm: READ");
#endif /* DEBUG_VER_ */

    bzero(readBuf, sizeof(readBuf));
    bzero(submesg, sizeof(submesg));

    while((readCtr==1) && (loopBreaker<10)) {
    	if (select(fd+1, FDSet, NULL, NULL, timeOut)) {
        	readCtr=read(fd, readBuf, sizeof(readBuf));
    		// call Count keeps track of the number of
	    	// read()/writes() to the serial object
		    countCall();
        	loopBreaker++;
    	} else {
#ifdef DEBUG_VER_
	    	fprintf(stderr, "Read() Time Out on socket %d\n", fd);
#endif // DEBUG_VER_ //
		    readCtr=1;
    		errRead=true;
	    	loopBreaker++;
    	}
    }

    /* 
       Was there an error reading? ie: after five
       loops, did we still read in new lines (in other
       words, did the radio have nothing to tell us?)
       Then we should have never been called, and set
       errRead appropriately.
     */
    (readCtr==1)?(errRead=true):(errRead=false);

    strcpy(submesg, readBuf);

    #ifdef DEBUG_VER_2
    fprintf(stderr, "\nRead: %i -  %s\n", readCtr, submesg);
    #endif /* DEBUG_VER_ */

    return readCtr;

}
/*! \fn PComm::Read(char *submesg)
	\brief Internal function to read from the socket
	\param submesg a character string long enough to hold data

	It reset's the read counter, and loopbreaker.
	The read counter keeps track of the number
	of bytes read in. The loop breaker makes sure
	that read doesnt get stuck reading New Lines
	from the radio for ever and ever. Since
	we are ignoring newlines when we read. The radio
	has a tendency of sending newlines even when
	it has nothing to say.

	it executes read() into the internal variable
	read buffer, and increments the loop breaker. If
	the number of bytes read is greater than 1, then
	some useful data was read in... no need to re-loop.
	When good data is read, it is copied into the submesg
	string from the read buffer.
	Select() will determine if the socket in FDSet is
	ready to read data.
	
	\return If after five times, the read() read in anything less 
	than 1, then break, and set errRead to true, while
	returning the number of bytes read.
	If after five seconds fd says that
	it's still not ready, then it will return, setting
	the read counter to a negative value.
	
	\sa Write()
 */

void PComm :: resetCall()
{
#ifdef DEBUG_VER_2
	fprintf(stderr, "PCOMM: resetCall() @ %d \n", callCount);
#endif DEBUG_VER_2
	callCount=0;
	int __readCounter=0;
	int __loopbreaker=0;
	
	PCClose();
	PCOpen(pcrDevice, pcrSpeed);
	bzero(callBuf, sizeof(callBuf));
	strncpy(callBuf, PCRPWRON, sizeof(callBuf)-1);
	strncat(callBuf, PCRECMD,  ((sizeof(callBuf))-(strlen(callBuf))-1));
   	if (select(fd+1, NULL, FDSet, NULL, timeOut)) {
        write(fd, callBuf, strlen(callBuf));
            if (select(fd+1, NULL, FDSet, NULL, timeOut)) {
                bzero(callBuf, sizeof(callBuf));
                while ((__readCounter==1) && (__loopbreaker<10)) {
                    __readCounter = read(fd, callBuf, sizeof(callBuf));
                    __loopbreaker++;
                }
#ifdef DEBUG_VER_
	    } else {
	       // we should never have gotten here, if so, we are fubar'd
	       // and should abort the program
	   	fprintf(stderr, "Read failed in PCOMM: resetCall()");
	   	fprintf(stderr, "***UNRECOVERABLE ERROR***");
	    	abort();
#endif // DEBUG_VER_
	    }
#ifdef DEBUG_VER_
	} else {
   	    // we should never have gotten here, if so, we are fubar'd
   	    // and should abort the program
		fprintf(stderr, "Write failed in PCOMM: resetCall()");
    	fprintf(stderr, "***UNRECOVERABLE ERROR***");
    	abort();
#endif // DEBUG_VER_
	}
}
/*! \fn PComm::resetCall()
	\brief resets the radio and port

	This function reset's the radio and the port when called.
	It is necessary to reset the radio after a certain number
	of read() and write()'s. In this case we have made it
	after 200 write() calls.

	It closes the port, and reopens the port. Zero's out the
	call buffer, and copy's the poweron/ecmd command strings
	into the call buffer. It then select()'s to see if we are
	ready to write to the buffer. After which it writes to the
	radio to tell it, that we are back online and want it to
	come up. Then it bzero's the call buffer and reads what
	the radio has to say. It loops in that mode if it received
	only one character of data (the radio likes to send nothing
	but newlines sometimes) AND the number of loops is less than
	10. We dont want to get stuck there.

	\b Warning: this function is necessary for the radio to 
	operate properly under the manual update mode. If you override
	this function make sure to run the radio in auto-update mode.

	\sa countCall()

 */

void PComm :: countCall()
{
	if (callCount<200) {
		callCount++;
	} else {
		resetCall();
	}

#ifdef DEBUG_VER_2
	fprintf(stderr, "PCOMM: countCall() %d\n", callCount);
#endif // DEBUG_VER_
}	
/*! \fn PComm::countCall()
	\brief function to keep track on the number of calls

	this function keeps track of the number of calls
	that were sent to the radio before the last reset.
	it calls resetCall() every 200 calls, otherwise
	it increments the call count.

	\sa resetCall
*/
