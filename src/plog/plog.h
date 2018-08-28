#ifndef PLOG_H_
#define PLOG_H_
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

/*
 *  This file declares PLog.
 *  PLog is the logging object.  It facilitates the reading and writing
 *  to log files.
 */

#if defined (SunOS) || defined (Irix)
#include <strings.h>
#else
#include <string.h>
#endif

#include <pcrdef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

class PLog {
	public:
		PLog(char *, char *);
		~PLog();

		void LogFreq(pcrfreq_t);
		void LogFreq(pcrfreq_t, int);
		pcrfreq_t ReturnChan(int);
		int ChanCount();

	private:
		void CountChannels();
		void ReadChannels();
		void CurrentTime(char *);
		void Intro();
		void Freq2Str(char *, pcrfreq_t);
		bool readfile;
		bool writefile;
		int ChanNum;
		char temp[80];
		FILE *logfile_p;
		pcrfreq_t *chanfreqs;
		pcrfreq_t Str2Freq(char *);
};

#endif /* PLOG_H_ */
