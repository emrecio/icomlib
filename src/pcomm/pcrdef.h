#ifndef PCRDEF_H
#define PCRDEF_H
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
	This is the PCR-1000 Command Set define file. Basically this file
	consists of all of the pertinent command prefixes that are sent to
	the radio.
 */

//! pcr1000 command string 
typedef char pcrcmd_t;
//! pcr1000 frequency length
typedef unsigned long int pcrfreq_t;

//! Suffix for Radio Query 
#define PCRQST		"\?"	   
//! Suffix for execute command 
#define PCRECMD		"\x0d\x0a" 

//! Init, manual probe 
/*!
	\b Warning: after issueing an init DO NOT
         read(). If you do, the read() will block and wont return.
         the radio doesn't return data after an initialization. You
         must close the socket, and reopen it. You wont have to reopen
         the socket with wierd opts, unless you reset the socket to 
         the state as it was before .
	\sa PCRINITA
*/
#define PCRINITM	"H101\x0d\x0aG300\x0d\x0a" 
//! Init, Auto probe
/*!
	\b Warning: after issueing an init DO NOT
         read(). If you do, the read() will block and wont return.
         the radio doesn't return data after an initialization. You
         must close the socket, and reopen it. You wont have to reopen
         the socket with wierd opts, unless you reset the socket to 
         the state as it was before .
	\sa PCRINITM
*/
#define PCRINITA	"H101\x0d\x0aG301\x0d\x0a" 

//! Signal Update (G3)
#define PCRSIG		"G3"	
//! Program should poll status from radio (G300)
#define PCRSIGOFF 	"G300"  
//! Radio sends status automagically when a change (G301)
#define PCRSIGON	"G301"  
//! Binary mode off (G302)
#define PCRSIGBOFF	"G302" 	
//! Binary mode on  (G303)
#define PCRSIGBON	"G303"	

//! Power (H1)
#define PCRPWR		"H1"	
//! Power radio down (H100)
#define PCRPWROFF	"H100" 	
//! Power radio up   (H101)
#define PCRPWRON	"H101" 	

//! Volume prefix (J40) 
#define PCRVOL		"J40"	
//! Volume at 75 % (J4075)
#define PCRVOLON	"J4075"
//! Volume at MUTE (J4000)
#define PCRVOLOFF	"J4000"

//! Squelch Prefix (J41) 
#define PCRSQL		"J41"	
//! Fully Open (J4100) 
#define PCRSQLO		"J4100"	
//! Closed squelch at 45% (J4145) 
#define PCRSQLC		"J4145"

//! IF Shift Prefix (J43) 
#define PCRIF		"J43"	
//! IF Centered (J4380)
#define PCRIFC		"J4380"	

//! Automatic Gain Control Prefix (J45) 
#define PCRAGC		"J45"	
//! AGC Off (J4500) 
#define PCRAGCOFF	"J4500"	
//! AGC On  (J4501) 
#define PCRAGCON	"J4501"	

//! Noise Blanking Prefix (J46) 
#define PCRNB		"J46"	
//! Noise Blanking Off (J4600) 
#define PCRNBOFF	"J4600"	
//! Noise Blanking On  (J4601) 
#define PCRNBON		"J4601"	

//! RF Attenuator Prefix 
#define PCRRFA		"J47"	
//! RF Attenuator Off (J4700) 
#define PCRRFAOFF	"J4700"	
//! RF Attenuator On (J4701) 
#define PCRRFAON	"J4701"

//! VSC Prefix (J50) 
#define PCRVSC		"J50"	
//! VSC Off (J5000) 
#define PCRVSCOFF	"J5000"	
//! VSC On  (J5001) 
#define PCRVSCON	"J5001"	

//! CTCSS - Tone Squelch Prefix (J51) 
#define PCRTSQL		"J51"	
//! CTCSS - Tone Squelch Off (J5100) 
#define PCRTSQLOFF	"J5100"	

#define PCRUNK01	"J4A"	/* Unknown - 1 */
#define PCRUNK02	"J4A80"	/* Unknown - 2 */

//! Tracking filter Prefix (LD082) 
#define PCRTFLTR	"LD82"	
//! Automagic Tracking Filter (LD8200) 
#define PCRTFLTR00	"LD8200"
//! Manual Tracking Filter (LD8201) 
#define PCRTFLTR01	"LD8201"

//! Freq. Header (K0) 
#define PCRFRQ		"K0"
//! freq. len. 10 bytes (padded) <GMMMKKKHHH> (10) 
#define MAXFRQLEN	10	
//! lower bounds for frequency 50 kHz (50000) 
#define LOWERFRQ	50000		
//! upper bound for frequency 1.3 GHz (1300000000) 
#define UPPERFRQ	1300000000	

//! Lower sideband (00) 
#define PCRMODLSB	"00"	
//! Upper sideband (01) 
#define PCRMODUSB	"01"	
//! Amplitude Modulated (02) 
#define PCRMODAM	"02"	
//! Continuous Mode (03) 
#define PCRMODCW	"03"	
//! unknown mode -- (04) 
#define PCRMODUNK	"04"	
//! Narrowband FM (05) 
#define PCRMODNFM	"05" 	
//! Wideband FM (06) 
#define PCRMODWFM	"06"	

//! 3 kHz Filter (00)	
#define PCRFLTR3	"00"	
//! 6 kHz Filter (01) 	
#define PCRFLTR6	"01"	
//! 15 kHz Filter (02) 	
#define PCRFLTR15	"02"	
//! 50 kHz Filter (03) 	
#define PCRFLTR50	"03"	
//! 230 kHz Filter (04)
#define PCRFLTR230	"04"	


//! Query Squelch Setting (I0)
#define PCRQSQL		"I0"	
//! Query Signal Strength (I1)
#define PCRQRST		"I1"	
//! Query Frequency Offset (I2)
#define PCRQOFST	"I2"	
//! Query presense of DTMF Tone (I3)
#define PCRQDTMF	"I3"	
//! Query Firmware revision (I4)
#define PCRQWAREZ	"G4"	
//! Query Presense of DSP (I5)
#define PCRQDSP		"GD"	
//! Query country / region (I6)
#define PCRQCTY		"GE"	

//! Reply: Ok (G000)
#define PCRAOK		"G000"	
//! Reply: There was an error (G001)
#define PCRABAD		"G001"	
//! DSP Header (PCRQDSP)
#define PCRADSP		PCRQDSP	
//! Not present (GD00)
#define PCRADSPNO	"GD00"	
//! Present (GD01)
#define PCRADSPOK	"GD01"	
//! Squelch Header (PCRQSQL)
#define	PCRASQL		PCRQSQL	
//! Sqlch Closed (04)
#define PCRASQLCL	"04"	
//! Sqlch Open (07)
#define PCRASQLOPN	"07"	
//! Signal Strength (PCRQRST)
/*! \b note: You have this header
				   plus 00-FF from weak to strong */
#define PCRARST		PCRQRST	
//! Frequency offset Header (PCRQOFST)
/*!	\b note: plus 00-7F from extreme (-) to near ctr OR
				   plus 81-FF from near ctr to extreme (+) */
#define PCRAOFST	PCRQOFST
//! Frequency (offset) centered (I280)
#define PCRAOFSTCTR	"I280"	

//! DTMF Header (PCRQDTMF)
#define PCRADTMF	PCRQDTMF
//! DTMF Not Heard (I300)
#define PCRADTMFNO	"I300"	
//! DTMF 0 (I310)
#define PCRADTMF0	"I310"	
//! DTMF 1 (I311)
#define PCRADTMF1	"I311"	
//! DTMF 2 (I312)
#define PCRADTMF2	"I312"	
//! DTMF 3 (I313)
#define PCRADTMF3	"I313"	
//! DTMF 4 (I314) 	
#define PCRADTMF4	"I314"	
//! DTMF 5 (I315)
#define PCRADTMF5	"I315"	
//! DTMF 6 (I315)
#define PCRADTMF6	"I316"	
//! DTMF 7 (I316)
#define PCRADTMF7	"I317"	
//! DTMF 8 (I318)
#define PCRADTMF8	"I318"	
//! DTMF 9 (I319)
#define PCRADTMF9	"I319"	
//! DTMF A (I31A)
#define PCRADTMFA	"I31A"	
//! DTMF B (I31B)
#define PCRADTMFB	"I31B"	
//! DTMF C (I31C)
#define PCRADTMFC	"I31C"	
//! DTMF D (I31D)
#define PCRADTMFD	"I31D"	
//! DTMF * (I31E)
#define PCRADTMFS	"I31E"	
//! DTMF # (I31F)
#define PCRADTMFP	"I31F"	

/* Radio miscellaneous functions */
//! Baud Rate Header (G1)
#define PCRBD		"G1"	
//! 300 baud (G100)
#define PCRBD300	"G100"	
//! 1200 baud (G101)
#define PCRBD1200	"G101"	
//! 2400 baud (G102)
#define PCRBD2400	"G102"	
//! 9600 baud (G103)
#define PCRBD9600	"G103"	
//! 19200 baud (G104)
#define PCRBD19200	"G104"	
//! 38400 baud (G105)
#define PCRBD38400	"G105"	

/* BandScope functions */
//! bandscope prefix SENT (ME00001)
#define PCRSBSC		"ME00001"	
//! bandscope prefix RECV (NE1)
#define PCRRBSC		"NE1"		
//! packet 0 (NE100)
#define PCRRBSC0	"NE100"		
//! packet 1 (NE110)
#define PCRRBSC1	"NE110"		
//! packet 2 (NE120)
#define PCRRBSC2	"NE120"		
//! packet 3 (NE130)
#define PCRRBSC3	"NE130"		
//! packet 4 (NE140)
#define PCRRBSC4	"NE140"		
//! packet 5 (NE150)
#define PCRRBSC5	"NE150"		
//! packet 6 (NE160)
#define PCRRBSC6	"NE160"		
//! packet 7 (NE170)
#define PCRRBSC7	"NE170"		
//! packet 8 (NE180)
#define PCRRBSC8	"NE180"		
//! packet 9 (NE190)
#define PCRRBSC9	"NE190"		
//! packet 10 (NE1A0)
#define PCRRBSCA	"NE1A0"		
//! packet 11 (NE1B0)
#define PCRRBSCB	"NE1B0"		
//! packet 12 (NE1C0)
#define PCRRBSCC	"NE1C0"		
//! packet 13 (NE1D0)
#define PCRRBSCD	"NE1D0"		
//! packet 14 (NE1E0)
#define PCRRBSCE	"NE1E0"		
//! packet 15 (NE1F0)
#define PCRRBSCF	"NE1F0"		



#endif /* PCRDEF_H */
