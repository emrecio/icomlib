#ifndef PFILEIO_H_
#define PFILEIO_H_
/*
 *   QPcr1k 
 *   Copyright (C) 1999, 2000 by PolyWog and Javaman for Ghetto.Org
 *    <polywog@ghetto.org>, <javaman@ghetto.org>
 *
 *   A GUI front end using the QT Toolkit and PCP/PCOMM library
 *   for the Icom Pcr-1000 radio.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <termios.h>

#define STDRCFILE ".pcprc"

struct qpcrCh {
    char ChFreq[32];
    char ChMode[8];
    char ChFilter[8];
    char ChToneSq[8];
    char ChDesc[64];
};

struct qpcrSc {
    char ScLow[16];
    char ScHi[16];
    char ScStepV[8];
    char ScStepR[8];
    int  ScHold;
};

struct qpcrRc {
    char RcMagic[16];       // magic number for rcfile
    char RcSql[16];         // save sess sql
    char RcVol[8];          // save sess vol
    char RcAgc[4];          // save sess auto gain
    char RcNbl[4];          // save sess noise blanker
    char RcRfa[4];          // save sess rf attenuator
    char RcDevice[64];      // save sess /dev
    tcflag_t RcSpeed;       // save sess speed
    int  RcLastCh;          // save last channel
    int  RcCurrCh;	    // current channel
    int  RcMaxCh;	 // max channels saved
    qpcrCh RcMem[40];    // saved memory channels
    qpcrSc ScMem[3];     // scanning bounds
};


class PFileIO
{
    public:
        PFileIO(const char *name=0);
        ~PFileIO();

	bool	PFOpen(const char*, const char*, bool force=false);
        bool    PFClose();
        bool    PFRead(char *);
        bool    PFWrite(const char *);
        bool    PFWrite(void *, size_t);
        bool    PFRead(void *, size_t);

    private:
        FILE    *rc_file;
        char    filename[128];
        FILE    *tmp_file;
        char    tmp_filename[128];
        char    home_dir[128];
        char    curr_dir[128];
        char    rc_filepath[256];


};

#endif /* PFILEIO_H_ */
