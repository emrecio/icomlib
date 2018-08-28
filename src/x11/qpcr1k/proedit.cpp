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

#include "pfileio.h"
#include "proedit.h"

#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qstring.h>
#include <qmessagebox.h>

#if defined(SunOS) || defined (Irix)
#include <strings.h>
#else if defined (Linux) || defined (BSD)
#include <string.h>
#endif


ProEdit :: ProEdit (QString *chNo, qpcrRc *passedPtr,int mode, QWidget *parent, 
			const char *name, bool modal)
         : QDialog(parent, name, modal)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "ProEdit: Creating in mode %d...\n", mode);
    #endif // DEBUG_VER_

	// set our current mode (for when we commit changes)
	currMode = mode;

	// create our grid layout and items to insert into the layout
	// a six by two grid, with a border spacing of 5 pixels and
	// an inter-widget spacing of the same (by default it uses
	// the border spacing if one is not specified)
	PEMainGL = new QGridLayout(this, 6, 2, 5);

	switch (mode) {
		case 0:
		/**********************************************************/
		/* TO EDIT FREQUENCY **************************************/
		/**********************************************************/
#ifdef DEBUG_VER_
		fprintf(stderr, "ProEdit: editing channels\n");
#endif

		origRc = passedPtr;
		setCaption("Edit Channel Information");

		__currCh = chNo->toInt();

		PEChannelLE  = new QLineEdit(this, "pechannelle");
		PEChannelLE->setText(passedPtr->RcMem[__currCh].ChDesc);
		PEChannelLbl = new QLabel(PEChannelLE, *chNo, 
			this, "pechannellbl",0);

		PEFreqLE = new QLineEdit(this, "pefreqle");
		tempString = new QString(passedPtr->RcMem[__currCh].ChFreq);
		PEConvFreq(tempString, TRUE);
		PEFreqLE->setText(*tempString);
		PEFreqLbl = new QLabel(PEFreqLE, "Frequency", 
			this, "pefreqlbl", 0);

		PEModeLE = new QLineEdit(this, "pemodele");
		PEModeLE->setText(passedPtr->RcMem[__currCh].ChMode);
		PEModeLbl = new QLabel(PEModeLE, "Mode", 
			this, "pemodelbl",0);

		PEFilterLE = new QLineEdit(this, "pefilterle");
		PEFilterLE->setText(passedPtr->RcMem[__currCh].ChFilter);
		PEFilterLbl = new QLabel(PEFilterLE, "Filter", 
			this, "pefilterlbl",0);

		PEToneSqLE = new QLineEdit(this, "petonesqle");
		PEToneSqLE->setText(passedPtr->RcMem[__currCh].ChToneSq);
		PEToneSqLbl = new QLabel(PEToneSqLE, "Tone Squelch", 
			this, "petonesqlbl",0);

		// now add the newly created items to the grid layout
		PEMainGL->addWidget(PEChannelLbl, 0, 0);
		PEMainGL->addWidget(PEChannelLE,  0, 1);
		PEMainGL->addWidget(PEFreqLbl,    1, 0);
		PEMainGL->addWidget(PEFreqLE,     1, 1);
		PEMainGL->addWidget(PEFilterLbl,  2, 0);
		PEMainGL->addWidget(PEFilterLE,   2, 1);
		PEMainGL->addWidget(PEToneSqLbl,  3, 0);
		PEMainGL->addWidget(PEToneSqLE,   3, 1);
		PEMainGL->addWidget(PEModeLbl,    4, 0);
		PEMainGL->addWidget(PEModeLE,     4, 1);

		break;

		case 1:
		/**********************************************************/
		/* TO EDIT DEVICE *****************************************/
		/**********************************************************/
#ifdef DEBUG_VER_
		fprintf(stderr, "ProEdit: editing device\n");
#endif
		origRc = passedPtr;

		setCaption("Edit Device");
		PEDeviceLE = new QLineEdit(this, "pedevicele");
		PEDeviceLE->setText(passedPtr->RcDevice);
		PEDeviceLbl = new QLabel(PEDeviceLE, "Device Name", 
			this, "pedevicelbl", 0);
		PEMainGL->addWidget(PEDeviceLbl, 0, 0);
		PEMainGL->addWidget(PEDeviceLE,  0, 1);
		break;

		case 2:
		/**********************************************************/
		/* TO EDIT BAUDRATE ***************************************/
		/**********************************************************/
#ifdef DEBUG_VER_
		fprintf(stderr, "ProEdit: editing baudrate\n");
#endif
		origRc = passedPtr;

		setCaption("Edit Baudrate");
		PESpeedLE = new QLineEdit(this, "pespeedle");
		PESpeedLE->setText(PEConvSpeed(passedPtr->RcSpeed));
		PESpeedLbl = new QLabel(PESpeedLE, "Baud Rate", this, 
			"pespeedlbl", 0);
		PEMainGL->addWidget(PESpeedLbl, 0, 0);
		PEMainGL->addWidget(PESpeedLE,  0, 1);
		break;

		default:
		// we should never be called here...
		reject();
	}

		
	
	// create the commit and rollback buttons for this form
	// connect their clicked signal with a local commit slot
	// the rollback button will just return rejected();
	PECommitBtn = new QPushButton("Commit", this, "pecommitbtn");
	PERollBkBtn = new QPushButton("Rollback", this, "perollbkbtn");
	connect ( PECommitBtn, SIGNAL(clicked()), SLOT(PECommitSlot()) );
	connect ( PERollBkBtn, SIGNAL(clicked()), SLOT(reject())       );
	PEMainGL->addWidget(PECommitBtn,  5, 0);
	PEMainGL->addWidget(PERollBkBtn,  5, 1);

    #ifdef DEBUG_VER_
    fprintf(stderr, "ProEdit: Created!\n");
    #endif // DEBUG_VER_
}

ProEdit :: ~ProEdit()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "ProEdit: Destroyed!");
    #endif // DEBUG_VER_
}

void ProEdit :: PECommitSlot()
{
#ifdef DEBUG_VER_
		fprintf(stderr, "ProEdit: PECommitSlot()\n");
#endif

	int foo = 0;

	switch (currMode) {
		case 0:
		/* save frequency information */
			// copies all of the necessary information from the 
			// line edits into the struct
			if ( PEFreqLE->text().length() < 11 ) {
				QMessageBox errMesg("Frequency Error",
					"Error!\nYour frequency must be of the format:\n"
					"\tGMMM.KKKhhh (ie: 0449.775000)",
					QMessageBox::Critical, QMessageBox::Ok, 0, 0 );
				errMesg.show();
			} else {
				//save the information
				QString __tempStr(PEFreqLE->text());
				PEConvFreq(&__tempStr, FALSE);
				bzero(&origRc->RcMem[__currCh], sizeof(qpcrCh));
				strncpy(origRc->RcMem[__currCh].ChFreq, (const char*)__tempStr,
					sizeof(origRc->RcMem[__currCh].ChFreq)-1);
				strncpy(origRc->RcMem[__currCh].ChMode,   PEModeLE->text(),    
					sizeof(origRc->RcMem[__currCh].ChMode)-1);
				strncpy(origRc->RcMem[__currCh].ChFilter, PEFilterLE->text(),  
					sizeof(origRc->RcMem[__currCh].ChFilter)-1);
				strncpy(origRc->RcMem[__currCh].ChToneSq, PEToneSqLE->text(),  
					sizeof(origRc->RcMem[__currCh].ChToneSq)-1);
				strncpy(origRc->RcMem[__currCh].ChDesc,   PEChannelLE->text(), 
					sizeof(origRc->RcMem[__currCh].ChDesc)-1);
				accept();
			}
			break;

		case 1:
		/* save device information */
			// copies all of the necessary information from the
			// form into the struct
			bzero(origRc->RcDevice, sizeof(origRc->RcDevice));
			strncpy(origRc->RcDevice, PEDeviceLE->text(),
				sizeof(origRc->RcDevice)-1);
			accept();
			break;

		case 2:
		/* save baudrate information */
			// copies all of the necessary information from
			// the form into the struct
			foo = PEConvSpeed(PESpeedLE->text());
			if (foo == (0)) {
				QMessageBox errMesg("BaudRate Error",
					"Error!\nYour baudrate must be one of:"
					"\t57600, 38400, 19200, 9600, 2400, \n"
					"\t1200",
                                        QMessageBox::Critical, 
					QMessageBox::Ok, 0, 0 );
                                errMesg.show();
				return;
			} else { 
				origRc->RcSpeed = foo;
				accept();
			}

			break;

		default:
		/* we shouldnt get to this point */
			reject();
	}

}

void ProEdit :: PEConvFreq (QString *freq, bool toHuman)
{
	// converts the frequency from computer readable,
	// to human readable and vice versa
	if ( toHuman ) {
		// human readable (adds dots)
		freq->insert(4, '.');
	} else {
		// computer readable (removes dots)
		freq->remove(4, 1);
	}
}

char *ProEdit :: PEConvSpeed (tcflag_t passedSpeed)
{
	// converts speed to human readable
	// returns the character string or 
	// UNK if it could not be decoded

	bzero(tempSpeed_Str, sizeof(tempSpeed_Str));

	switch (passedSpeed) {
		case B9600:
			strncpy(tempSpeed_Str, "9600", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		case B38400:
			strncpy(tempSpeed_Str, "38400", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		case B19200:
			strncpy(tempSpeed_Str, "19200", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		case B4800:
			strncpy(tempSpeed_Str, "4800", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		case B2400:
			strncpy(tempSpeed_Str, "2400", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		case B1200:
			strncpy(tempSpeed_Str, "1200", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		case B300:
			strncpy(tempSpeed_Str, "300", 
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
		default:
			// we should never get here 
			strncpy(tempSpeed_Str, "UNK",
				sizeof(tempSpeed_Str)-1);
			return tempSpeed_Str;
			break;
	}
}

tcflag_t ProEdit :: PEConvSpeed (const char *passedSpeed)
{
	// converts speed to computer readable
	// returns the tcflag_t value of the baudrate,
	// or `0' if could not decode the baudrate

	if (strncmp(passedSpeed, "57600", 3)==0) {
		return B57600;
	} else if (strncmp(passedSpeed, "38400", 3)==0) {
		return B38400;
	} else if (strncmp(passedSpeed, "19200", 3)==0) {
		return B19200;
	} else if (strncmp(passedSpeed, "9600", 3)==0) {
		return B9600;
	} else if (strncmp(passedSpeed, "2400", 3)==0) {
		return B2400;
	} else if (strncmp(passedSpeed, "4800", 3)==0) {
		return B4800;
	} else if (strncmp(passedSpeed, "1200", 3)==0) {
		return B1200;
	} else if (strncmp(passedSpeed, "300", 3)==0) {
		return B300;
	} else {
		return 0;
	}
	
}
