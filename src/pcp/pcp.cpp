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
 *  This file declares PCP.
 *  PCP Is the actual object that interfaces with the GUI
 *  This API deals with the error handling and the calls
 *  that must be made to and from the radio, via the PComm
 *  serial i/o object.
 */

#include "pcp.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#if defined (SunOS) || defined (Irix)
#include <strings.h>
#else // def SunOs/Irix
#include <string.h>
#endif // def SunOs/Irix

PCP :: PCP(char *port, tcflag_t speed, const char *name)
{
    PCPRadio = new PRadInf;
    PCPComm = new PComm(port, speed, name);

    memset(PCPRadio, '\0', sizeof(PRadInf));
    memset(PCPTemp, '\0', sizeof(PCPTemp));

    strncpy(PCPRadio->PCPPort, port, 0xff);
    PCPRadio->PCPSpeed = speed;
    PCPRadio->PCPVolume = 0;
    PCPRadio->PCPSquelch = 0;
    PCPRadio->PCPFreq = 146000000;
    strncpy(PCPRadio->PCPMode, PCRMODNFM, sizeof(PCPRadio->PCPMode));
    strncpy(PCPRadio->PCPFilter, PCRFLTR15, sizeof(PCPRadio->PCPFilter));
    PCPRadio->PCPToneSq=0;
    PCPRadio->PCPToneSqFloat=0.0;
    PCPRadio->PCPAutoGain=false;
    PCPRadio->PCPNoiseBlank=false;
    PCPRadio->PCPRFAttenuator=false;
    PCPRadio->PCPAutoUpdate = false;
    PCPStatus = false;
}
/*! \fn PCP::PCP(char *port, tcflag_t speed, const char *name)
	\brief PCP Object constructor
	\param port the device to open
	\param speed the initial baudrate to open at
	\param name internal object name

	Initialises the radio device and sets the default variables to: <br>
	\arg \c #PCPSpeed to \a speed
	\arg \c #PCPVolume to 0 
	\arg \c #PCPSquelch to 0
	\arg \c #PCPFreq to 146.000 MHz
	\arg \c #PCPMode to #PCRMODNFM
	
*/

PCP :: ~PCP()
{
    delete PCPComm;
}
/*! \fn PCP::~PCP()
	\brief Your friendly destructor
	
	Deletes the Primitive Communication ( PComm ) object
	before exitting.
*/

bool PCP :: PCPInit(bool autoUpdate)
{
    if (autoUpdate==false) {
        PCPComm->PCTell(PCRINITM);
	fprintf(stderr, "Radio is coming up. Please wait...\n");
	sleep(1); 
	if (PCPStatus) 
        if (PCPCheckResponse()) {
            PCPRadio->PCPAutoUpdate=false;
            return true;
        }
    } else {
        PCPComm->PCTell(PCRINITA);
	sleep(1);
	if (PCPStatus)
        if (PCPCheckResponse()) {
            PCPRadio->PCPAutoUpdate=true;
            return true;
        }
    }

    return false;
}
/*! \fn bool PCP::PCPInit(bool autoUpdate)
	\brief Initialise the radio
	\param autoUpdate Initialise the radio in \a autoUpdate mode

	This function sends the initialsation command(s).
	Due to the hardware taking some time to initialize,
	we sleep for one second to let it catch up, then check
	to see if the radio was on. If it *was* on then 
	it checks for a response.

	After PCPCheckResponse tells it that the command
	succeeded, it sets PCPRadio->PCPAutoUpdate mode appropriately.

	\return On success : true otherwise false.

	\sa PCPAutoUpdate PCPCheckResponse PCPStatus
 */

bool PCP :: PCPPowerUp()
{
    PCPComm->PCTell(PCRPWRON);
    if (PCPCheckResponse()) {
//	PCPCheckResponse();
	PCPStatus=true;
	return true;
    } 
    return false;
}
/*! \fn bool PCP::PCPPowerUp()
	\brief Powers the radio on
	
	Turns the radio's receiver on. By sending the command
	code #PCRPWRON
	
	\return true or false ( based on #PCPCheckResponse )

	\sa PCPPowerDown PCPCheckResponse PCPStatus PCRPWRON pcrdef.h
 */

bool PCP :: PCPPowerDown()
{
    PCPComm->PCTell(PCRPWROFF);
    // if (PCPCheckResponse()) {
	PCPCheckResponse();
	PCPStatus=false;
	return true;
    // } 
    // return false;
}
/*! \fn bool PCP::PCPPowerDown()
	\brief Powers the radio down
	
	Turns the radio's receiver off. By sending the command
	code PCRPWROFF
	
	\return true or false ( based on #PCPCheckResponse )

	\sa PCPPowerUp PCPCheckResponse PCPStatus PCRPWROFF pcrdef.h
 */

bool PCP :: PCPSetSpeed(tcflag_t speed)
{
    if ( (B300<=speed)&&(speed<=B38400) ) {
	memset(PCPRadio->PCPInitSpeed, '\0', sizeof(PCPRadio->PCPInitSpeed));
        switch (speed) {
                case B38400:
                        // you probably want to set the speed
                        // to fastest available, so let's put
                        // this here first
                        strncpy(PCPRadio->PCPInitSpeed, PCRBD38400,
                                sizeof(PCPRadio->PCPInitSpeed)-1);
                        break;
                case B19200:
                        strncpy(PCPRadio->PCPInitSpeed, PCRBD19200,
                                sizeof(PCPRadio->PCPInitSpeed)-1);
                        break;
                case B300:
                        strncpy(PCPRadio->PCPInitSpeed, PCRBD300,
                                sizeof(PCPRadio->PCPInitSpeed)-1);
                        break;
                case B1200:
                        strncpy(PCPRadio->PCPInitSpeed, PCRBD1200,
                                sizeof(PCPRadio->PCPInitSpeed)-1);
                        break;
                case B2400:
                        strncpy(PCPRadio->PCPInitSpeed, PCRBD2400,
                                sizeof(PCPRadio->PCPInitSpeed)-1);
                        break;
                default:
                        // if all else fails, we'll always
                        // have paris! ~=^)
                        strncpy(PCPRadio->PCPInitSpeed, PCRBD9600,
                                sizeof(PCPRadio->PCPInitSpeed)-1);
        }
	PCPComm->PCTell(PCPRadio->PCPInitSpeed);
        delete PCPComm;
        PCPComm = new PComm(PCPRadio->PCPPort, speed, "PCPComm_R");
// investigate possible responses, i dont think one is given.
//	PCPCheckResponse();
        	PCPRadio->PCPSpeed = speed;
        	return true;
    }

    return false;
}
/*! \fn bool PCP::PCPSetSpeed (tcflag_t speed)
	\brief Sets the speed for current session

	\param speed baudrate as defined in termios.h
	
	First we check to see if the baudrate passed 	
	in \a speed is right, if not then we just quietly 
	return false. Then we decode \a speed and set 
	#PCPInitSpeed to #pcrcmd_t version.

	Then we tell the radio to switch speeds and
	set baudrate on the port by destroying PComm
	and reinitiating it with the new baud setting

	\b Warning: follow these procedures to use this 
	function. 
	<ul><li>create the object (at last known baudrate).
	<li>call init
	<li>call power up
	<li>call this function
	<li>delete the object
	<li>create the object with the new speed setting
	</ul>

	\return true or false based on success.
	\sa PCPGetSpeed PCPGetSpeed_t pcrcmd_t pcrdef.h PCPComm 
 */

bool PCP :: PCPSetPort(const char *port) 
{
    PCPComm->PCClose();
    return(PCPComm->PCOpen(port));
}
/*! \fn bool PCP::PCPSetPort(const char *port)
	\brief Set the port for the current session

	Sets port by closing the filedes and reopening it
	on the new port.
	
	\return true or false if the serial device can be opened
	on the new port.

	\sa PCPGetPort PComm::PCOpen PComm::PCClose 
 */

bool PCP :: PCPSetVolume(int volume) 
{
    if ((0<=volume)&&(volume<=99)) {
        memset(PCPTemp, '\0', sizeof(PCPTemp));
        sprintf(PCPTemp, "%s%0.2d", PCRVOL, volume);
        PCPComm->PCTell(PCPTemp);
        if (PCPCheckResponse()) {
            PCPRadio->PCPVolume=volume;
            return true;
        }
    }

    return false;
}
/*! \fn bool PCP::PCPSetVolume(int volume)
	\brief Set the current session's volume
	\param volume an integer between 0 and 99

    	sprintf converts (and combines) the cmd #PCRVOL with
    	the argument, such that the argument has a minimum field width
    	of two chars. If the field is < 2 chars (ie: arg=5) then it
    	pads the field with one zero.

	\return true or false based on #PCPCheckResponse to indicate
	success or failure

	\sa PCPVolume pcrcmd_t pcrdef.h
 */

bool PCP :: PCPSetSquelch(int squelch) 
{
    if ((0<=squelch)&&(squelch<=99)) {
        memset(PCPTemp, '\0', sizeof(PCPTemp));
        sprintf(PCPTemp, "%s%0.2d", PCRSQL, squelch);
        PCPComm->PCTell(PCPTemp);
        if (PCPCheckResponse()) {
            PCPRadio->PCPSquelch=squelch;
            return true;
        }
    }

    return false;
}
/*! \fn bool PCP::PCPSetSquelch(int squelch)
	\brief Set the current session's squelch
	\param squelch an integer between 0 and 99

    sprintf converts (and combines) the cmd #PCRSQL with
    the argument \a squelch , such that the argument has a 
    minimum field width of two chars. If the field 
    is < 2 chars (ie: arg=5) then it pads the field 
    with one zero.

	\return true or false based on #PCPCheckResponse to indicate
	success or failure

	\sa PCPSquelch pcrcmd_t pcrdef.h
 */

bool PCP :: PCPSetFreq(pcrfreq_t freq)
{
    char freqConv[32];
    memset(freqConv, '\0', sizeof(freqConv));

    if ((LOWERFRQ<=freq)&&(freq<=UPPERFRQ)) {
        memset(PCPTemp, '\0', sizeof(PCPTemp));
        sprintf(freqConv, "%0.10lu", freq);
        strcpy(PCPTemp, PCRFRQ);
        strcat(PCPTemp, freqConv);
        strcat(PCPTemp, PCPRadio->PCPMode);
        strcat(PCPTemp, PCPRadio->PCPFilter);
        strcat(PCPTemp, "00");
        PCPComm->PCTell(PCPTemp);
        if (PCPCheckResponse()) {
            PCPRadio->PCPFreq=freq;
#ifdef DEBUG_VER_
	    fprintf(stderr, "PCP: PCPSETFREQ - Success\n");
#endif
            return true;
        }
    }
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSETFREQ - Failed\n");
#endif
    return false;
}
/*! \fn bool PCP::PCPSetFreq (pcrfreq_t freq)
	\brief Set the current frequency
	\param freq passed frequency compliant to pcrfreq_t

    check to see if the frequencies are within bounds.
    populate a string, with correctly padded 0's based
    on the frequency fed in.

	<ul> This method, #PCPSetMode and #PCPSetFilter 
	take the following steps 
	<li> copy the header, 
	<li> concat the newly converted/padded frequency, 
	<li> concat mode, 
	<li> concat filter, 
	<li> concat suffix.
	</ul>

    send the command to the radio, if it checks out
    set this as the new frequency 

	\return true or false based on success or failure
	\sa pcrdef.h pcrfreq_t PCRFRQ

 */

bool PCP :: PCPSetMode(const char *mode)
{
    char freqConv[32];
    char temp[8];

    memset(PCPTemp, '\0', sizeof(PCPTemp));
    sprintf(freqConv, "%0.10lu", PCPRadio->PCPFreq);
    strcpy(PCPTemp, PCRFRQ);
    strcat(PCPTemp, freqConv);

    if ( strncmp(mode, "AM", 2)==0 ) {
	strcat(PCPTemp, PCRMODAM);
	strcpy(temp, PCRMODAM);
    } else if ( strncmp(mode, "CW", 2)== 0 ) {
	strcat(PCPTemp, PCRMODCW);
	strcpy(temp, PCRMODCW);
    } else if ( strncmp(mode, "LSB", 3)==0 ) {
	strcat(PCPTemp, PCRMODLSB);
	strcpy(temp, PCRMODLSB);
    } else if ( strncmp(mode, "USB", 3)==0 ) {
	strcat(PCPTemp, PCRMODUSB);
	strcpy(temp, PCRMODUSB);
    } else if ( strncmp(mode, "NFM", 3)==0 ) {
	strcat(PCPTemp, PCRMODNFM);
	strcpy(temp, PCRMODNFM);
    } else if ( strncmp(mode, "WFM", 3)==0 ) {
	strcat(PCPTemp, PCRMODWFM);
	strcpy(temp, PCRMODWFM);
    } else {
	return false;
    }

    strcat(PCPTemp, PCPRadio->PCPFilter);
    strcat(PCPTemp, "00");
    PCPComm->PCTell(PCPTemp);
    if (PCPCheckResponse()) {
#ifdef DEBUG_VER_
	fprintf(stderr, "Storing PCPRadio->PCPMode: %s\n", temp);
#endif // DEBUG_VER_
        strcpy(PCPRadio->PCPMode,temp);
        return true;
    }
    
    return false;

}
/*! \fn bool PCP::PCPSetMode(const char *mode)
	\brief Set the current session's mode
	\param mode plain text string of mode (eg: "USB")

	Valid arguments for \a mode:
	\arg \c USB	upper side band
	\arg \c LSB	lower side band
	\arg \c AM	amplitude modulated
	\arg \c NFM	narrow band FM
	\arg \c WFM	wide band FM
	\arg \c CW	continuous wave

	The concept is the same as above ( #PCPSetFreq ) except it accepts
	standard text for "USB"/"LSB" etc... Use of the pcrdef codes
	are not necessary, they will be decoded based on \a mode.

	\return true or false based on success or failure

	\sa PCPRadio pcrdef.h
 */

bool PCP :: PCPSetFilter(const char *filter)
{
    char freqConv[32];
    char temp[8];

    memset(PCPTemp, '\0', sizeof(PCPTemp));
    sprintf(freqConv, "%0.10lu", PCPRadio->PCPFreq);
    strcpy(PCPTemp, PCRFRQ);
    strcat(PCPTemp, freqConv);
    strcat(PCPTemp, PCPRadio->PCPMode);

    if ( strncmp(filter, "3", 1)==0 ) {
        strcat(PCPTemp, PCRFLTR3);
	strcpy(temp, PCRFLTR3);
    } else if ( strncmp(filter, "6", 1)== 0 ) {
        strcat(PCPTemp, PCRFLTR6);
	strcpy(temp, PCRFLTR6);
    } else if ( strncmp(filter, "15", 2)==0 ) {
        strcat(PCPTemp, PCRFLTR15);
	strcpy(temp, PCRFLTR15);
    } else if ( strncmp(filter, "50", 2)==0 ) {
        strcat(PCPTemp, PCRFLTR50);
	strcpy(temp, PCRFLTR50);
    } else if ( strncmp(filter, "230", 3)==0 ) {
        strcat(PCPTemp, PCRFLTR230);
	strcpy(temp, PCRFLTR230);
    } else {
        return false;
    }

    strcat(PCPTemp, "00");
    PCPComm->PCTell(PCPTemp);
    if (PCPCheckResponse()) {
#ifdef DEBUG_VER_
	fprintf(stderr, "Storing PCPRadio->PCPFilter: %s\n", temp);
#endif DEBUG_VER_
        strcpy(PCPRadio->PCPFilter,temp);
        return true;
    }

    return false;
}
/*! \fn bool PCP::PCPSetFilter(const char *filter)
	\brief Sets current session's filter
	\param filter character string version of the filter

	Valid arguments for \a filter:
	\arg \c 3 3.0 kHz
	\arg \c 6 6.0 kHz
	\arg \c 15 15.0 kHz
	\arg \c 50 50.0 kHz
	\arg \c 230 230.0 kHz

	The concept is the same as above ( #PCPSetMode ) it accepts
	standard text for "3"/"6" etc... Use of the pcrdef codes
	are not necessary, they will be decoded based on \a filter.

	\return true or false based on success or failure 

	\sa PCPRadio pcrdef.h
 */

bool PCP :: PCPSetToneSq(const char *value)
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSETTONESQ (string) - %s\n", value);
#endif // DEBUG_VER_ 

        memset(PCPTemp, '\0', sizeof(PCPTemp));
	strncpy(PCPTemp, PCRTSQL, sizeof(PCPTemp));
	strncat(PCPTemp, value, sizeof(PCPTemp));
	PCPComm->PCTell(PCPTemp);
	if (PCPCheckResponse()) {
		PCPRadio->PCPToneSq=value;
		return true;
	}

	return false;
}
/*! \fn bool PCP::PCPSetToneSq(const char *value)
	\brief Sets current session CTCSS
	\param value character string of 01-35 hex

	set's the tone squelch for the radio. The default is
	value 00 for off. The values are \b NOT the hz, but the
	#pcrdef.h vals, 01=67.0 02=69.3 etc... 

	The valid for \a value are \b hex values from \a 00 for off
	through \a 35 

	\return true or false based on #PCPCheckResponse 
	success or failure.

	\sa pcrdef.h PCRTSQL
*/

bool PCP :: PCPSetToneSq(float passvalue)
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSETTONESQ (float) - %f\n", passvalue);
#endif // DEBUG_VER_ 

	int tone;

	tone = (int)(passvalue * 10.0 + .1);  // Hack to remove truncating errors.
	PCPRadio->PCPToneSqFloat=passvalue;		

	switch (tone)
	{
		case 0: return PCPSetToneSq("00"); break;
		case 670: return PCPSetToneSq("01"); break;
		case 693: return PCPSetToneSq("02"); break;
		case 710: return PCPSetToneSq("03"); break;
		case 719: return PCPSetToneSq("04"); break;
		case 744: return PCPSetToneSq("05"); break;
		case 770: return PCPSetToneSq("06"); break;
		case 797: return PCPSetToneSq("07"); break;
		case 825: return PCPSetToneSq("08"); break;
		case 854: return PCPSetToneSq("09"); break;
		case 885: return PCPSetToneSq("0A"); break;
		case 915: return PCPSetToneSq("0B"); break;
		case 948: return PCPSetToneSq("0C"); break;
		case 974: return PCPSetToneSq("0D"); break;
		case 1000: return PCPSetToneSq("0E"); break;
		case 1035: return PCPSetToneSq("0F"); break;
		case 1072: return PCPSetToneSq("10"); break;
		case 1109: return PCPSetToneSq("11"); break;
		case 1148: return PCPSetToneSq("12"); break;
		case 1188: return PCPSetToneSq("13"); break;
		case 1230: return PCPSetToneSq("14"); break;
		case 1273: return PCPSetToneSq("15"); break;
		case 1318: return PCPSetToneSq("16"); break;
		case 1365: return PCPSetToneSq("17"); break;
		case 1413: return PCPSetToneSq("18"); break;
		case 1462: return PCPSetToneSq("19"); break;
		case 1514: return PCPSetToneSq("1A"); break;
		case 1567: return PCPSetToneSq("1B"); break;
		case 1598: return PCPSetToneSq("1C"); break;
		case 1622: return PCPSetToneSq("1D"); break;
		case 1655: return PCPSetToneSq("1E"); break;
		case 1679: return PCPSetToneSq("1F"); break;
		case 1713: return PCPSetToneSq("20"); break;
		case 1738: return PCPSetToneSq("21"); break;
		case 1773: return PCPSetToneSq("22"); break;
		case 1799: return PCPSetToneSq("23"); break;
		case 1835: return PCPSetToneSq("24"); break;
		case 1862: return PCPSetToneSq("25"); break;
		case 1899: return PCPSetToneSq("26"); break;
		case 1928: return PCPSetToneSq("27"); break;
		case 1966: return PCPSetToneSq("28"); break;
		case 1995: return PCPSetToneSq("29"); break;
		case 2035: return PCPSetToneSq("2A"); break;
		case 2065: return PCPSetToneSq("2B"); break;
		case 2107: return PCPSetToneSq("2C"); break;
		case 2181: return PCPSetToneSq("2D"); break;
		case 2257: return PCPSetToneSq("2E"); break;
		case 2291: return PCPSetToneSq("2F"); break;
		case 2336: return PCPSetToneSq("30"); break;
		case 2418: return PCPSetToneSq("31"); break;
		case 2503: return PCPSetToneSq("32"); break;
		case 2541: return PCPSetToneSq("33"); break;
		default: PCPSetToneSq("00"); break;
	}
	return false;
}
/*! \fn bool PCP::PCPSetToneSq(float passvalue)
	\brief Sets session CTCSS based on a float value
	\param passvalue tone squelch in Hz

	Since the previous method requires the programmer to
	remember the PCR-1000's internal number that corresponds
	to the tone squelch frequency, this overloaded method
	allows the programmer to pass a float, where the float
	is the frequency (Hz) in question.

	\return true or false based on #PCPCheckResponse 
	success or failure. On failure, it turns off CTCSS
	and returns false.

	\sa pcrdef.h PCRTSQL
*/

bool PCP :: PCPSetAutoGain(bool value)
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSETAUTOGAIN - %d\n", value);
#endif // DEBUG_VER_ 

	(value)?(PCPComm->PCTell(PCRAGCON)):(PCPComm->PCTell(PCRAGCOFF));

	if (PCPCheckResponse()) {
		PCPRadio->PCPAutoGain=value;
		return true;
	}

	return false;
}
/*! \fn  bool PCP :: PCPSetAutoGain(bool value)
	\brief Toggle autogain functionality
	\param value true or false for autogain on or off

	Valid values for \a value are:
	\arg \c true to activate autogain
	\arg \c false to deactivate autogain

	Sets the automagic gain control to \a value
	(on/off) true/false... checks the radio response
	if ok, then sets the value 

	\return true, on success otherwise returns false

	\sa PCRAGC PCRAGCON PCRAGCOFF pcrdef.h
*/

bool PCP :: PCPSetNB(bool value)
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSETNB - %d\n", value);
#endif // DEBUG_VER_ 

	(value)?(PCPComm->PCTell(PCRNBON)):(PCPComm->PCTell(PCRNBOFF));
	
	if (PCPCheckResponse()) {
		PCPRadio->PCPNoiseBlank=value;
		return true;
	}

	return false;
}
/*! \fn bool PCP::PCPSetNB(bool value)
	\brief Toggle Noiseblanking functionality
	\param value true or false for noiseblanking on or off

	Valid values for \a value are:
	\arg \c true to activate noiseblanking
	\arg \c false to deactivate noiseblanking

	Sets the noise blanking to \a value
	(on/off) true/false... checks the radio response
	if ok, then sets the value 

	\return true, on success otherwise returns false

	\sa PCPGetNB PCPGetNBStr PCRNB PCRNBON PCRNBOFF pcrdef.h
*/

bool PCP :: PCPSetRFAttenuator ( bool value )
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSETRFATTENUATOR - %d\n", value);
#endif // DEBUG_VER_ 

	(value)?(PCPComm->PCTell(PCRRFAON)):(PCPComm->PCTell(PCRRFAOFF));

	if (PCPCheckResponse()) {
		PCPRadio->PCPRFAttenuator=value;
		return true;
	}

	return false;
}
/*! \fn bool PCP :: PCPSetRFAttenuator ( bool value )
        \brief Toggle RF Attenuation functionality
        \param value true or false for RF Attenuation on or off

        Valid values for \a value are:
        \arg \c true to activate RF Attenuation
        \arg \c false to deactivate RF Attenuation

        Sets the RF Attenuation to \a value
        (on/off) true/false... checks the radio response
        if ok, then sets the value

        \return true, on success otherwise returns false

        \sa PCPGetRFAttenuator PCPGetRFAttenuatorStr PCRRFA PCRRFAON PCRRFAOFF pcrdef.h
*/

  ////////////////////
 // getter methods //
////////////////////

bool PCP :: PCPIsOn()
{
    return PCPStatus;
}
/*! \fn bool PCP :: PCPIsOn()
	\brief Inquire radio status

	Check to see if the radio is on based on the internally
	stored data. This function should only be called after
	the object has been initiated and is ready for use.
	
	\return true or false for radio on or off.

	\sa PCPQueryOn
*/

const char *PCP :: PCPSigStrengthStr()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSIGSTRENGTH (string) Called\n");
#endif // DEBUG_VER_ 
	if(PCPComm->PCAsk(PCRQRST)) {
		if(PCPComm->PCHear(PCPTemp)) {
			return (const char*)PCPTemp;
		}
	}

	return 0;
}
/*! \fn const char *PCP :: PCPSigStrengthStr()
	\brief Signal strength query. (const char * version)

	Querys radio to read the current signal strength.

	\return null on failure, otherwise a character string
	with the current signal strenth. This includes the I1
	header, plus the last two characters which is the
	\b hex value from \a 00-99	

	\sa PCPSigStrength PCRQRST pcrdef.h
*/


int PCP :: PCPSigStrength()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPSIGSTRENGTH (int) Called\n");
#endif // DEBUG_VER_

	int sigstr;
	char digit;
	if(PCPComm->PCAsk(PCRQRST)) {
		if(PCPComm->PCHear(PCPTemp)) {
			digit = PCPTemp[2];
			if((digit >= 'A') && (digit <= 'F'))
				sigstr = (digit - 'A' + 1) * 16;
		 	else
				sigstr = atoi(&digit) * 16;

			digit = PCPTemp[3];
			if((digit >= 'A') && (digit <= 'F'))
				sigstr += digit - 'A' + 1;
			else
				sigstr += atoi(&digit);

			return sigstr;
		}
	}

	return 0;
}
/*! \fn int PCP :: PCPSigStrength()
	\brief Querys the signal strength (int version)
	
	\return integer value of 0-255 on signal strength.

	\sa PCPSigStrengthStr PCRQRST pcrdef.h
*/

bool PCP :: PCPQueryOn()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PCP: PCPQUERYON Called\n");
#endif // DEBUG_VER_
	if(PCPComm->PCAsk("H1")) {
		if(PCPComm->PCHear(PCPTemp)) {
			if(strcmp(PCPTemp, "H101") == 0)
				return true;
			else
				return false;
		}
	}

	return false;
}
/*! \fn PCP :: PCPQueryOn()
	\brief Querys radio acutator status

	Actually querys the radio for a status on it's
	receiver state (on or off). This differs from
	#PCPIsOn in that it makes a call to the radio,
	rather than checking a local variable

	\return true if the radio's receiver is on, 
	false otherwise.

	\sa PCPIsOn	
*/

bool PCP :: PCPQuerySquelch()
{
	char __tempvar1[8];
	memset(__tempvar1, '\0', sizeof(__tempvar1));
	strncpy(__tempvar1, PCRASQL, sizeof(__tempvar1)-1);
	strncat(__tempvar1, PCRASQLCL, sizeof(__tempvar1)-strlen(__tempvar1)-1);

	if (PCPComm->PCAsk(PCRQSQL)) {
		if(PCPComm->PCHear(PCPTemp)) {
			if (strncmp(PCPTemp, __tempvar1, 4)==0) {
				return false;
			} else {
				return true;
			}
		}
	}
	return false;
}
/*! \fn bool PCP :: PCPQuerySquelch()
        \brief Querys radio's squelch status

        Actually querys the radio for a status on it's
        squelch state (open or closed). 
                                           
        \return true if the radio's squelch is open or
        false otherwise.
                       
        \sa PCPIsOn     
*/

const char *PCP :: PCPGetPort()
// return a character string representing the current
// port setting /dev/*
{
    return PCPRadio->PCPPort;
}
/*! \fn PCP :: PCPGetPort()
	\brief Gets current port / serial device setting

	Checks the #PCPRadio struct for member #PCPPort
	for the current port (serial) device setting
	(pathname).

	\return character string consisting of the current
	session's device name (pathname)

	\sa PCPSetPort

*/

tcflag_t PCP :: PCPGetSpeed_t()
{
	return PCPRadio->PCPSpeed;
}
/*! \fn tcflag_t PCP :: PCPGetSpeed_t()
	\brief Gets current speed (tcflag_t version)
	
	Checks #PCPRadio struct for member #PCPSpeed
	for the current speed (baudrate) setting.

	\return tcflag_t baudrate.

	\sa PCPGetSpeed	
*/

const char *PCP :: PCPGetSpeed()
{
	switch(PCPRadio->PCPSpeed) {
		case B300: return "300"; break;
		case B600: return "600"; break;
		case B1200: return "1200"; break;
		case B1800: return "1800"; break;
		case B2400: return "2400"; break;
		case B4800: return "4800"; break;
		case B9600: return "9600"; break;
		case B19200: return "19200"; break;
		case B38400: return "38400"; break;
		case B57600: return "57600"; break;
		default: return "unknown"; break;
	}
	return "unknown";
}
/*! \fn const char *PCP :: PCPGetSpeed()
	\brief Gets current speed (const char* version)
	
	Checks #PCPRadio struct for member #PCPSpeed
	for the current speed (baudrate) setting. Decodes
	the tcflag_t baudrate in the struct

	\return const character string baudrate

	\sa PCPGetSpeed_t	
*/


int PCP :: PCPGetVolume()
{
    return (PCPRadio->PCPVolume);
}
/*! \fn int PCP :: PCPGetVolume()
	\brief Gets current session's volume setting (int version)

	Checks #PCPRadio struct for member #PCPVolume
	for the current volume \b (hex) setting. 

	\return the integer value from \a 00-ff 

	\sa PCPGetVolumeStr
*/

const char *PCP :: PCPGetVolumeStr()
{
    sprintf(PCPTemp, "%d", PCPRadio->PCPVolume);
    return (PCPTemp);
}
/*! \fn const char *PCP::PCPGetVolumeStr()
	\brief Gets current session's volume setting (const char* version)

	Checks #PCPRadio struct for member #PCPVolume
	for the current volume \b (hex) setting. Decodes
	the hex to a character string

	\return the character string of current volume setting

	\sa PCPGetVolume
*/

int PCP :: PCPGetSquelch()
{
    return (PCPRadio->PCPSquelch);
}
/*! \fn int PCP::PCPGetSquelch()
	\brief Gets current session's squelch setting (int version)
	
	Checks #PCPRadio struct for member #PCPSquelch
	for the current squelch \b (hex) setting. 

	\return the integer value from \a 00-ff 

	\sa PCPGetSquelchStr
*/

const char *PCP :: PCPGetSquelchStr()
{
    sprintf(PCPTemp, "%d", PCPRadio->PCPSquelch);
    return (PCPTemp);
}
/*! \fn const char *PCP :: PCPGetSquelchStr()
	\brief Gets current session's squelch setting (const char* version)
	
	Checks #PCPRadio struct for member #PCPSquelch
	for the current squelch \b (hex) setting. Decodes
	the integer into a character string

	\return the character string of current squelch setting

	\sa PCPGetSquelchStr
*/

pcrfreq_t PCP :: PCPGetFreq()
{
    return PCPRadio->PCPFreq;
}
/*! \fn pcrfreq_t PCP::PCPGetFreq()
	\brief Gets current session's frequency setting (pcrfreq_t version)
	
	Checks #PCPRadio struct for member #PCPFreq
	for the current frequency setting. 

	\return the pcrfreq_t of current frequency setting

	\sa PCPGetFreqStr
*/



const char *PCP :: PCPGetFreqStr()
{
    sprintf(PCPTemp, "%0.10lu", PCPRadio->PCPFreq);
    return PCPTemp;
}
/*! \fn const char *PCP :: PCPGetFreqStr()
	\brief Gets current session's frequency setting (const char* version)
	
	Checks #PCPRadio struct for member #PCPFreq
	for the current frequency setting. It converts the
	pcrfreq_t into a character string.

	\return the character string of current frequency setting

	\sa PCPGetFreq
*/


const pcrcmd_t *PCP :: PCPGetMode()
{
    return (PCPRadio->PCPMode);
}
/*! \fn const pcrcmd_t *PCP::PCPGetMode()
	\brief Gets current session's mode setting (pcrcmd_t version)
	
	Checks #PCPRadio struct for member #PCPMode
	for the current mode setting.

	\return the pcrcmd_t of current mode setting

	\sa PCPGetModeStr
*/

const char *PCP :: PCPGetModeStr()
{
    memset(PCPTemp, '\0', sizeof(PCPTemp));

    if (strcmp(PCRMODWFM, PCPRadio->PCPMode)==0) {
	strcpy(PCPTemp, "WFM");
	return PCPTemp;
    } else if (strcmp(PCRMODNFM, PCPRadio->PCPMode)==0) {
	strcpy(PCPTemp, "NFM");
	return PCPTemp;
    } else if (strcmp(PCRMODCW, PCPRadio->PCPMode)==0) {
	strcpy(PCPTemp, "CW");
	return PCPTemp;
    } else if (strcmp(PCRMODAM, PCPRadio->PCPMode)==0) {
	strcpy(PCPTemp, "AM");
	return PCPTemp;
    } else if (strcmp(PCRMODUSB, PCPRadio->PCPMode)==0) {
	strcpy(PCPTemp, "USB");
	return PCPTemp;
    } else if (strcmp(PCRMODLSB, PCPRadio->PCPMode)==0) {
	strcpy(PCPTemp, "LSB");
	return PCPTemp;
    } 
    
    strcpy(PCPTemp, "UNKNOWN");
    return PCPTemp;

}
/*! \fn const char *PCP::PCPGetModeStr()
	\brief Gets current session's mode setting (const char* version)
	
	Checks #PCPRadio struct for member #PCPMode
	for the current mode setting. Decodes the mode
	setting to plain english equivalent.

	\return the plain english of current mode setting

	\sa PCPGetMode PCPSetMode
*/

const pcrcmd_t *PCP :: PCPGetFilter()
{
    return (PCPRadio->PCPFilter);
}
/*! \fn const pcrcmd_t *PCP :: PCPGetFilter()
	\brief Gets current session's filter setting (pcrcmd_t version)
	
	Checks #PCPRadio struct for member #PCPFilter
	for the current filter setting. 

	\return the pcrcmd_t of the current filter setting

	\sa PCPGetFilterStr PCPSetFilter
*/

const char *PCP :: PCPGetFilterStr()
{
    memset(PCPTemp, '\0', sizeof(PCPTemp));

    if (strcmp(PCRFLTR230, PCPRadio->PCPFilter)==0) {
	strcpy(PCPTemp, "230");
	return PCPTemp;
    } else if (strcmp(PCRFLTR50, PCPRadio->PCPFilter)==0) {
	strcpy(PCPTemp, "50");
	return PCPTemp;
    } else if (strcmp(PCRFLTR15, PCPRadio->PCPFilter)==0) {
	strcpy(PCPTemp, "15");
	return PCPTemp;
    } else if (strcmp(PCRFLTR6, PCPRadio->PCPFilter)==0) {
	strcpy(PCPTemp, "6");
	return PCPTemp;
    } else if (strcmp(PCRFLTR3, PCPRadio->PCPFilter)==0) {
	strcpy(PCPTemp, "3");
	return PCPTemp;
    } 

    return PCPRadio->PCPFilter;
}
/*! \fn const char *PCP :: PCPGetFilterStr()
	\brief Get the current session's filter setting (const char* version)

	Checks #PCPRadio struct for member #PCPFilter
	for the current filter setting. It then decodes
	the #pcrcmd_t version into standard string values.

	\return the character string of the current filter setting
		
	\sa PCPGetFilter PCPSetFilter
*/

const char *PCP :: PCPGetToneSq()
{
    return PCPRadio->PCPToneSq;
}
/*! \fn const char *PCP :: PCPGetToneSq() 
  	\brief Gets the current session's tone squelch (undecoded version)

	Checks #PCPRadio struct for member #PCPToneSq
	for the current tone squelch setting.

	\return the character string of the current tone squelch setting

	\sa PCPGetToneSqStr PCPSetToneSq
 */

const char *PCP :: PCPGetToneSqStr()
{
    memset(PCPTemp, '\0', sizeof(PCPTemp));
    sprintf(PCPTemp, "%f", PCPRadio->PCPToneSqFloat);
    return PCPTemp;
}
/*! \fn const char *PCP :: PCPGetToneSqStr() 
  	\brief Gets the current session's tone squelch (decoded version)

	Checks #PCPRadio struct for member #PCPToneSq
	for the current tone squelch setting. It is decoded
	into plain english and it's value returned.

	\return the character string of the current tone squelch setting

	\sa PCPGetToneSq PCPSetToneSq
 */

bool PCP :: PCPGetAutoGain()
{
    return PCPRadio->PCPAutoGain;
}	
/*! \fn bool PCP :: PCPGetAutoGain()
	\brief Get current session's autogain value (bool version)
	
	Checks #PCPRadio struct for member #PCPAutoGain
	for the current auto-gain setting. 

	\return the boolean of the current setting. True/false :: On/off.

	\sa PCPGetAutoGainStr PCPSetAutoGain
*/

const char *PCP :: PCPGetAutoGainStr()
{
    memset(PCPTemp, '\0', sizeof(PCPTemp));
    (PCPGetAutoGain())?(strcpy(PCPTemp,"1")):(strcpy(PCPTemp,"0"));
    return PCPTemp;
}
/*! \fn const char *PCP :: PCPGetAutoGainStr()
	\brief Get current session's autogain value (const char* version)
	
	Checks #PCPRadio struct for member #PCPAutoGain
	for the current auto-gain setting. Decodes true
	and false into string values "1" and "0".

	\return the character string of the current setting.

	\sa PCPGetAutoGain PCPSetAutoGain
*/

bool PCP :: PCPGetNB()
{
    return PCPRadio->PCPNoiseBlank;
}
/*! \fn bool PCP :: PCPGetNB()
	\brief Get current session's noiseblank value (bool version)
	
	Checks #PCPRadio struct for member #PCPNoiseBlank
	for the current auto-gain setting. 

	\return the boolean of the current setting. True/false :: On/off.

	\sa PCPGetNBStr PCPSetNB
*/

const char *PCP :: PCPGetNBStr()
{
    memset(PCPTemp, '\0', sizeof(PCPTemp));
    (PCPGetNB())?(strcpy(PCPTemp,"1")):(strcpy(PCPTemp,"0"));
    return PCPTemp;
}
/*! \fn const char *PCP :: PCPGetNBStr()
	\brief Get current session's noiseblank value (const char* version)
	
	Checks #PCPRadio struct for member #PCPNoiseBlank
	for the current auto-gain setting. Decodes the boolean
	value into the string "1" for true or "0" for false

	\return the character string of the current setting. True/false :: "1"/"0"

	\sa PCPGetNB PCPSetNB
*/

bool PCP :: PCPGetRFAttenuator()
{
    return PCPRadio->PCPRFAttenuator;
}
/*! \fn bool PCP :: PCPGetRFAttenuator()
	\brief Get current session's RF Attenuation value (bool version)
	
	Checks #PCPRadio struct for member #PCPRFAttenuator
	for the current RF Attenuation setting. 

	\return the boolean of the current setting. True/false :: On/off.
	
	\sa PCPGetRFAttenuatorStr PCPSetRFAttenuator
*/

const char *PCP :: PCPGetRFAttenuatorStr()
{
    memset(PCPTemp, '\0', sizeof(PCPTemp));
    (PCPGetRFAttenuator())?(strcpy(PCPTemp,"1")):(strcpy(PCPTemp,"0"));
    return PCPTemp;
}
/*! \fn const char *PCP :: PCPGetRFAttenuatorStr()
	\brief Get current session's RF Attenuation value (const char* version)

	Checks #PCPRadio struct for member #PCPRFAttenuator
	for the current session's RF Attenuation setting. Decodes
	the boolean value into "1" for true or "0" for false.

	\return the character string of the current setting. True/false :: "1"/"0"

	\sa PCPGetRFAttenuator PCPSetRFAttenuator
*/

const PRadInf PCP :: PCPGetRadioInfo()
{
    return *PCPRadio;
}

bool PCP :: PCPCheckResponse()
{
    PCPComm->PCHear(PCPTemp);
    if (strncmp(PCPTemp, PCRAOK, 4)==0) {
        PCPErrRead=false;
        return true;
    } else if (strncmp(PCPTemp, PCRABAD, 4)==0) {
        PCPErrRead=false;
        return false;
    }

    PCPErrRead = true;
    return false;
}
/*! \fn bool PCP :: PCPCheckResponse()
	\brief Internally called method to check radio response.
    
	read from the radio for the #PCRAOK and #PCRABAD reply.

    \return
        \a true    - for PCRAOK
        \a false   - for PCRABAD
        \a false   - and sets ErrRead to true if garbage was read.

 */
