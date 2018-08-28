#ifndef PCOMM_H_
#define PCOMM_H_
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#include "pcrdef.h"

#define BAUDRATE B9600
#define PCRDEV "/dev/pcr1000"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

typedef int size_pc;    /* i/o count confirmation */

//! The Primitive Communications Object
/*!
        PComm object is the serial i/o object. It facilitates the
        low level grunt work for reading and writing to the serial
        device(s). It handles the string manipulation necessary for
        communication with the radio.
 */
class PComm {
    public:
	PComm(char *pcrDev, tcflag_t baudRate=B9600, const char *name=0);
	~PComm();

	bool PCOpen(const char *pcrDev=PCRDEV, tcflag_t baudRate=B9600);
	void PCClose();

	size_pc PCTell(char *);
	size_pc PCAsk(char *);
	size_pc PCHear(char *);
	size_pc PCRawWrite(char *);
	size_pc PCRawRead(char *);

    private:
	size_pc Write(char *);
	size_pc Read(char *);
	
	void 	resetCall();
	void	countCall();
	//! Counter for number of times the radio has been written to
	int 	callCount;
	//! Internal buffer for radio reset
	char   callBuf[256];

	//! Used in the select for timeout(s)
	struct  timeval *timeOut;  	
	//! Socket set used in the select
	fd_set  *FDSet;	        	
	//! Select()'s return value
	int     retVal;           
	//! Stores the port's original settings
	struct  termios *oldtio;   
	//! Port settings to be applied for current session
	struct  termios *newtio;   
	//! Socket descriptor used for current session
	int     fd;                
	//! Current serial/comm device
	char    pcrDevice[256];	
	//! Current baud rate
	tcflag_t pcrSpeed;	     

	//! Was there an error reading?
	 bool errRead;       
	//! Was there an error writing?
	 bool errWrite;          
	//! Buffer for sending data in PCAsk 
	 char askBuf[256];      
	//! Buffer for receiving data in PCHear
	 char hearBuf[256];      
	//! Buffer for writing data used in wrapper Write()
	 char writeBuf[256];     
	//! Buffer for reading data used in wrapper Read()
	 char readBuf[256];      
	//! Internal object name. \b Warning: not necessarily populated.
	 char int_name[256];     
	//! Send bytecount used in wrapper function Write()
	 size_pc writeCtr;       
	//! Receive bytecount used in wrapper function Read()
	 size_pc readCtr;        
	//! Receive bytecount used in PCAsk
	 size_pc askCtr;         
	//! Send bytecount used in PCTell
	 size_pc tellCtr;        
	//! Receive bytecount used in PCHear
	 size_pc hearCtr;        
};

#endif /* PCOMM_H_ */
