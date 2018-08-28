#ifndef PCP_H_
#define PCP_H_
/* 
   Copyright (C) 1999,2000 PolyWog and Javaman for Ghetto.Org
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


#include <pcrdef.h>
#include <pcomm.h>
#include <sys/termios.h> /* for tcflag_t */

//! Default radio port for this object
#define PCPDefaultPort "/dev/pcr1000"
//! Default baud rate for this object
#define PCPDefaultSpeed B9600

//! Holds radio information
/*!
	Stores the important radio information for the current
	state of the radio.
*/
struct PRadInf {
	//! Currently active port/device
        char        PCPPort[64];
	//! Currently set speed (tcflag_t var)
        tcflag_t    PCPSpeed;
	//! Currently set speed (char * version, unstable)
        char        PCPInitSpeed[8];
	//! Currently set volume
        int         PCPVolume;
	//! Currently set squlech
        int         PCPSquelch;
	//! Currently set frequency 
	/*! \sa pcrdef.h pcrfreq_t */
        pcrfreq_t   PCPFreq;
	//! Currently set radio Mode
	/*! \sa pcrdef.h pcrcmd_t */
        pcrcmd_t    PCPMode[128];
	//! Currently set radio Filter
	/*! \sa pcrdef.h pcrcmd_t */
        pcrcmd_t    PCPFilter[128];
	//! Currently set update mode?
        bool		PCPAutoUpdate;

	//! Currently set CTCSS (const char*, unstable)
    	const char	*PCPToneSq;
	//! Currently set CTCSS (float)
    	float       PCPToneSqFloat;
	//! Currenly set autogain
	bool		PCPAutoGain;
	//! Currently set noiseblanking
    	bool		PCPNoiseBlank;
	//! Currently set RF Attenuation
	bool		PCPRFAttenuator;
};

//! PCP is the Main API for documentation of the class
/*! 
	PCP Is the actual object that interfaces with the GUI
	This API deals with the error handling and the calls
	that must be made to and from the radio, via the PComm
	serial i/o object.
	
	\sa PComm pcrdef.h
 */
class PCP { 
    public:
        PCP(char *port=PCPDefaultPort, tcflag_t speed=PCPDefaultSpeed,
            const char *name=0);
        ~PCP();

        bool PCPInit(bool autoUpdate=false);
        bool PCPPowerUp();
        bool PCPPowerDown();

        // basic radio methods
        bool PCPSetSpeed(tcflag_t);
        bool PCPSetPort(const char *);
        bool PCPSetVolume(int);
        bool PCPSetSquelch(int);
        bool PCPSetFreq(pcrfreq_t);
        bool PCPSetMode(const char *);
        bool PCPSetFilter(const char *);
        int  PCPSigStrength();
        const char *PCPSigStrengthStr();

        // advanced radio methods
        bool PCPSetToneSq(const char*);
        bool PCPSetToneSq(float);
        bool PCPSetAutoGain(bool);
        bool PCPSetNB(bool);
        bool PCPSetRFAttenuator(bool);

        bool 		 PCPIsOn();
        bool 		 PCPQueryOn();
	bool		 PCPQuerySquelch();
        const char 	*PCPGetPort();
	tcflag_t	 PCPGetSpeed_t();
        const char 	*PCPGetSpeed();
        int 		 PCPGetVolume();
    	const char	*PCPGetVolumeStr();
        int 		 PCPGetSquelch();
    	const char	*PCPGetSquelchStr();
        pcrfreq_t 	 PCPGetFreq();
    	const char      *PCPGetFreqStr();
        const pcrcmd_t 	*PCPGetMode();
    	const char	*PCPGetModeStr();
        const pcrcmd_t 	*PCPGetFilter();
        const char 	*PCPGetFilterStr();

    	const char  *PCPGetToneSq();
    	const char  *PCPGetToneSqStr();
        bool		 PCPGetAutoGain();
        const char  *PCPGetAutoGainStr();
    	bool		 PCPGetNB();
    	const char  *PCPGetNBStr();
        bool		 PCPGetRFAttenuator();
        const char  *PCPGetRFAttenuatorStr();

	//! Retrieves the current radio struct	
        /*! 
		this gets the current radio information struct
		in case the user wants to save the state of the
		radio.
		\return const PRadInf
		\sa PRadInf 
	*/
        const PRadInf PCPGetRadioInfo();

    private:
        bool 		PCPCheckResponse();

	//! The currently active Primitive Communication Object ( PComm )
        PComm   	*PCPComm;
	//! The state of the PComm object (on or off)
    	bool	    PCPStatus;
	//! Was there an error reading from the PComm object?
        bool		PCPErrRead;
	//! Temporary buffer to hold PCP string data
        char		PCPTemp[256];

	//! Currently active radio data
        struct PRadInf *PCPRadio;
};


#endif /* PCP_H_ */
