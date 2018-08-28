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

#if defined(SunOS) || defined(Irix)
#include <strings.h>
#else
#include <string.h>
#endif

#include <qwidget.h>
#include <qstring.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qtimer.h>
#include <qregexp.h>

#include "pui.h"
#include "pkeybd.h"
#include "pdisp.h"
#include "pctrl.h"

PUI :: PUI (QWidget *parent, const char *name)
         : QWidget(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PUI: Creating...\n");
    #endif // DEBUG_VER_

    // create the timer that we will be using to trigger a 
    // display reset
    PUTimeOut = new QTimer ( this, "putimeout_qtimer");

    // first thing we do is layout the grid for placing the
    // custom complex widgets. In this case, the simplest thing
    // that we can do is create a 2x2 QGridLayout.
    PUGrid  = new QGridLayout(this, 2, 2, 0, 2);

    // then we create pretty group boxes for each item. beveled
    // edges with their respective titles.
    PUKeyGrpBx = new QGroupBox("Direct Key Entry",this, "pukeygrpbx_groupbox" );
    PUCtrlGrpBx= new QGroupBox("Radio Controls",  this, "puctrlgrpbx_groupbox");
    PUDispGrpBx= new QGroupBox("Radio Display",   this, "pudispgrpbx_groupbox");

    // since we are only placing one widget, let's set each of
    // these group boxes to one col wide, in this case orientation
    // doesnt matter, so let's select Horizontal
    PUKeyGrpBx->setColumnLayout (1, Horizontal);
    PUCtrlGrpBx->setColumnLayout(1, Horizontal);
    PUDispGrpBx->setColumnLayout(1, Horizontal);

    // now lets create the custom widgets that are actually going
    // to be placed in the grid we created above. Note that their
    // parents arent the grid, but the groupboxes. Doing this,
    // tells the groupbox that we want our widget to be a part of it.
    PUKeyBd   = new PKeyBoard(PUKeyGrpBx,  "pukeybd_pkeyboard");
    PUDisplay = new PDisplay (PUDispGrpBx, "pudisplay_pdisplay");
    PUControl = new PControl (PUCtrlGrpBx, "pucontrol_pcontrol");

    // and finally add the *groupbox* (NOT the widget) to the grid
    // layout.
    PUGrid->addWidget(PUKeyGrpBx, 1, 1);
    PUGrid->addWidget(PUCtrlGrpBx,1, 0);
    PUGrid->addMultiCellWidget(PUDispGrpBx,0, 0, 0, 1);

    PUGrid->setResizeMode( QLayout::Minimum );
    PUGrid->activate();

    // ok, now lets connect the signals that may be emitted from the
    // control box to the display
    connect (PUControl, SIGNAL(valueChangeCall(const char*, const char*)),
	                SLOT(valueChangeMgrSlot(const char*, const char*)) );

    // the keypad has to be handled in a special way before being
    // sent to the freqdisplay slot.
    connect (PUKeyBd,   SIGNAL(keyPressCall(const char*)),
	                SLOT(keyPressMgrSlot(const char*)) );
    connect (PUKeyBd,	SIGNAL(directEntryCall(const char*)),
			SLOT(directEntrySlot(const char*)) );
    connect (PUKeyBd,	SIGNAL(massiveChCall(const char)),
			SLOT(massiveChSlot(const char))    );
    connect (PUTimeOut, SIGNAL(timeout()), SLOT(keyPressTOSlot()) );

    // before we leave let's set some default values 
    // as well, as declare the qtimer used for the ``special'' keypad
    bzero(curFreq, sizeof(curFreq));
    bzero(newFreq, sizeof(newFreq));
    strcpy(curFreq, "0146000000");
    strcpy(newFreq, "0000000000");
    settingFreq=FALSE;
    mhzSet=FALSE;

    #ifdef DEBUG_VER_
    fprintf(stderr, "PUI: Created!\n");
    #endif // DEBUG_VER_
}

PUI :: ~PUI()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PUI: Destroyed!");
    #endif // DEBUG_VER_
}

void PUI :: valueChangeMgrSlot(const char *name, const char *value)
// the point of this manager is to emit a radio command.
// when the user enters a radio command, it will not 
// display right away. Only after an ``ok'' has been read
// this widget's parent should call valueChangeSlot()
{

    emit execRadioCommand(name, value);
}

void PUI :: valueChangeSlot( const char *name, const char *value )
// this is called (by our parent) after the parent 
// has received our execRadioCommand signal, and 
// has sent and received the proper reply from the radio
{
    #ifdef DEBUG_VER_2
    fprintf(stderr, "PUI: VALUECHANGESLOT - %s is now %s\n",
            name, value);
    #endif // DEBUG_VER_

	if (strncmp(name, "FREQ", 3)!=0) {
		PUDisplay->valueChangeSlot(name, value);
		PUControl->valueChangeSlot(name, value);
	} else {
		PUDisplay->freqChangeSlot(value);
		strcpy(curFreq, value);
	}

}

void PUI :: keyPressTOSlot()
// if the five second timer times out,
// then it resets the current frequency, and
// sets the entering-frequency mode to false
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PUI: KEYPRESSTOSLOT - TIMEOUT!");
#endif /* DEBUG_VER_ */
	settingFreq=FALSE;
	mhzSet=FALSE;
	PUDisplay->freqChangeSlot(curFreq);
}

void PUI :: keyPressMgrSlot(const char *value)
// this function is rather simple when you think about
// it. Basically it's a way to display what the user
// is entering on the keypad ``live'' with the added
// bonus of the timer.
{
	bzero(temp, sizeof(temp));

	if (strncmp(value, ".", 1)==0) {
		// if someone pressed a DOT
		if (settingFreq && !mhzSet) {
			if (strlen(newFreq)<4) {
				int padding = (4-strlen(newFreq));
				for (int ctr=0; ctr<padding; ctr++) {
					strcat(temp,"0");
				}
			}
			strcat(temp, newFreq);
			strcpy(newFreq, temp);
			mhzSet=TRUE;
			PUDisplay->freqChangeSlot(newFreq);
			PUTimeOut->start(5*1000, TRUE);
		}
	} else if (strncmp(value, "R", 1)==0) {
		// if someone pressed clear
		if (settingFreq) {
			settingFreq=FALSE;
			mhzSet=FALSE;
			PUDisplay->freqChangeSlot(curFreq);
			PUTimeOut->stop();
		}
	} else if ( strncmp(value, ";", 1)==0 )    {
                // if someone pressed enter
		if (settingFreq) {
			settingFreq = FALSE;
			int freqlen = strlen(newFreq);
			int padding = (10 - freqlen);
			for (int ctr=0; ctr<padding; ctr++) {
				strcat(newFreq, "0");
			}
			PUTimeOut->stop();
			curFreq=newFreq;
			mhzSet=FALSE;
			PUDisplay->freqChangeSlot(curFreq);
			emit execRadioCommand("FREQ", newFreq);
		} 
	} else if (strncmp(value, "+", 1)==0) {
                // if we pressed add/alter channel
		if (settingFreq) {
			settingFreq=FALSE;
			mhzSet=FALSE;
			PUDisplay->freqChangeSlot(curFreq);
			PUTimeOut->stop();
		}
		emit execRadioCommand("MEMORIZE", value);
	} else if (strncmp(value, "-", 1)==0) {
		// if we pressed remove channel
		if (settingFreq) {
                       settingFreq=FALSE;
                        mhzSet=FALSE;
                        PUDisplay->freqChangeSlot(curFreq);
                        PUTimeOut->stop();
                }
                emit execRadioCommand("REMOVE", value);
	} else if ((strncmp(value, "<", 1)==0) ||
	           (strncmp(value, ">", 1)==0)) {
                // if we pressed channel up or down?
		if (settingFreq) {
			settingFreq=FALSE;
			mhzSet=FALSE;
			PUDisplay->freqChangeSlot(curFreq);
			PUTimeOut->stop();
		}	
                emit execRadioCommand("CHANNEL", value);
	} else if ((strncmp(value, "^", 1)==0) ||
		   (strncmp(value, "v", 1)==0)) {
		// if we pressed step up or down?
		if (settingFreq) {
			settingFreq=FALSE;
			mhzSet=FALSE;
			PUDisplay->freqChangeSlot(curFreq);
			PUTimeOut->stop();
		}
		emit execRadioCommand("STEP", value);
	} else {
                // if someone pressed 0-9
		if (settingFreq) {
			strcat(newFreq, value);
			PUDisplay->freqChangeSlot(newFreq);
			PUTimeOut->start(5*1000, TRUE );
		} else {
			settingFreq=TRUE;
			strcpy(newFreq, value);
			PUDisplay->freqChangeSlot(newFreq);
			PUTimeOut->start(5*1000, TRUE );
		}
	}
#ifdef DEBUG_VER_
	fprintf(stderr, "PUI : KEYPRESSMGRSLOT - SET: %s NEW: %s\n", 
		curFreq, newFreq);
#endif // DEBUG_VER_
}

void PUI :: directEntrySlot(const char *value)
{
	bzero(newFreq, sizeof(newFreq));
	bzero(curFreq, sizeof(curFreq));

	QString __temp(value);

	QRegExp r = QRegExp("[a-z]", FALSE, FALSE);
	if (__temp.contains( r )>0) {
#ifdef DEBUG_VER_
		fprintf(stderr, "PKEYBOARD: DIRECTENTRYSLOT(): bad string\n");
#endif
		return;
	}

	int __index = (-1);		// index starts at 0
	int __length = __temp.length();	// length starts at 1

	__index = __temp.find('.');
	if (__index != (-1)) {
#ifdef DEBUG_VER_
		fprintf(stderr, "PKEYBOARD: DIRECTENTRYSLOT(): found decimal\n");
#endif
		QString __tempR;
		QString __tempL;
		__tempL = __temp.left(__index);
		// -1 b/c we want the characters after the '.'
		__tempR = __temp.right(__length-__index-1); 

		// since the max freq we can enter will have 4 chars
		// to the left of the decimal point (decimal is in MHz)
		// pad the left substring with zero
		__tempL = __tempL.rightJustify(4, '0');
		__tempR = __tempR.leftJustify(6, '0');
		__temp = __tempL ;
		__temp.append(__tempR);
	} else {
#ifdef DEBUG_VER_
		fprintf(stderr, "PKEYBOARD: DIRECTENTRYSLOT(): no decimal\n");
#endif
		if (__temp.length() > 4) {
#ifdef DEBUG_VER_
			fprintf(stderr,"PKEYBOARD: DIRECTENTRYSLOT(): too many digits\n");
#endif
			// if we dont have a decimal point, and
			// we are trying to assign a frequency
			// higher than four digits then poop out.
			return;
		} else {
#ifdef DEBUG_VER_
			fprintf(stderr, "PKEYBOARD: DIRECTENTRYSLOT(): padding %s\n", (const char*)__temp);
#endif
			// otherwise padd the frequency and get
			// it ready for being set.

			// first we have to left pad it to four 
			// digits, THEN we can pad the whole thing
			// with zeroes on the right to 10 digits.
			// b/c once we know that the length is left 
			// justified at a length of four, then we
			// can add the rest of the zeroes on the right
			// by right justifying the thing 

			// so for example, if 13 is entered, after
			__temp = __temp.rightJustify(4,'0');
			// it becomes 0013, but we know that they
			// meant to enter 13 MHz so that after this
			__temp = __temp.leftJustify(10,'0');
			// we now have 0013000000
#ifdef DEBUG_VER_
			fprintf(stderr, "PKEYBOARD: DIRECTENTRYSLOT(): %s\n", (const char *)__temp);
#endif
		}
	}

	strncpy(newFreq, (const char*)__temp, sizeof(newFreq)-1);
	strncpy(curFreq, (const char*)__temp, sizeof(curFreq)-1);
#ifdef DEBUG_VER_
	fprintf(stderr, "PKEYBOARD: DIRECTENTRYSLOT(): %s\n", newFreq);
#endif 

	PUDisplay->freqChangeSlot(newFreq);
	emit execRadioCommand("FREQ", curFreq);
}

void PUI :: massiveChSlot(const char value)
// handles the scanner calls
{
	emit scanCall(value);
}

void PUI :: setMiniMode (bool value)
// engage mini-me mode
{
	if (value) {
		PUKeyGrpBx->hide();
		PUCtrlGrpBx->hide();
		// setMaximumSize(PUDispGrpBx->minimumSize());
	} else {
		PUKeyGrpBx->show();
		PUCtrlGrpBx->show();
		// setMaximumSize(PUGrid->sizeHint());
	}
}

bool PUI :: miniMode ()
// are we set to mini-me mode?
{
	return (!PUKeyGrpBx->isVisible());
}
