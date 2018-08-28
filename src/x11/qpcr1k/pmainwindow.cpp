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

#if defined (SunOS) || (Irix)
#include <strings.h>
#elif defined (Linux) || defined (BSD)
#include <string.h>
#endif

#include "pmainwindow.h"
#include "pfileio.h"
#include "prodlg.h"
#include "pui.h"
#include "pmenu.h"
#include "ptools.h"
#include "pstatus.h"

#include <pcp.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qframe.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qtimer.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

PMainWindow :: PMainWindow (QWidget *parent, const char *name)
             : QMainWindow(parent, name)
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: Creating...\n");
#endif // DEBUG_VER_
	__first_time 	= true;	
	PMClosing	= false;
    // create our main menubar and connect the dots... ;)
    // ok connect the signals emitted from PMMenuBar object
    // with the slots in this object
    PMMenuBar = new PMenu ( this, "menu" );
    connect (PMMenuBar, SIGNAL(newProfileCall()),   SLOT(PMNewProfile())   );
    connect (PMMenuBar, SIGNAL(loadProfileCall()),  SLOT(PMLoadProfile())  );
    connect (PMMenuBar, SIGNAL(saveasProfileCall()),SLOT(PMSaveAsProfile()));
    connect (PMMenuBar, SIGNAL(saveProfileCall()),  SLOT(PMSaveProfile())  );
    connect (PMMenuBar, SIGNAL(editProfileCall()),  SLOT(PMEditProfile())  );
//    connect (PMMenuBar, SIGNAL(initRadioCall()),    SLOT(PMInit())         );
//    connect (PMMenuBar, SIGNAL(powerRadioCall()),   SLOT(PMPower())        );
    connect (PMMenuBar, SIGNAL(aboutBoxCall()),     SLOT(PMAbout())        );
    connect (PMMenuBar, SIGNAL(exitProgramCall()),  SLOT(PMQuit())         );

    // create the status bar. This object will know that we are creating a
    // status bar because we said that *this* is our parent when creating
    // the statusbar. Hence we dont have to `add' anything. Since it is a
    // derivative of QStatusBar we overload the constructor so that our
    // text widgets are created and the *setter* methods (as slots) allow
    // access to each of the text widgets (which are actually QLabels).
    // -- see pstatus.h for more info.
    PMStatusBar = new PStatus(this, "pmstatusbar_pstatus");

    // create the tool bar... ptool will automatically insert
    // the toolbuttons and all that jazz.
    QString temp("Main Tool Bar");
    PMToolBar = new PTools(temp, this, this);

    // toolbar's been created. add it to *this* object.
    addToolBar(PMToolBar);

    // connect the toolbar's signals to their respective slots (note that
    // both above and here, we ommited the third argument to connect, which
    // *should* be the name of the object whose slot we are connecting to.
    // This is a neat (or not-so-neat) shortcut. Omitting the third object
    // assumes that we are talking about slots located in *this* object).
    connect (PMToolBar, SIGNAL(newProfileCall()),   SLOT(PMNewProfile())   );
    connect (PMToolBar, SIGNAL(loadProfileCall()),  SLOT(PMLoadProfile())  );
    connect (PMToolBar, SIGNAL(saveProfileCall()),  SLOT(PMSaveProfile())  );
    connect (PMToolBar, SIGNAL(editProfileCall()),  SLOT(PMEditProfile())  );
//    connect (PMToolBar, SIGNAL(initRadioCall()),    SLOT(PMInit())         );
//    connect (PMToolBar, SIGNAL(powerRadioCall()),   SLOT(PMPower())        );
    connect (PMToolBar, SIGNAL(aboutBoxCall()),     SLOT(PMAbout())        );
    connect (PMToolBar, SIGNAL(exitProgramCall()),  SLOT(PMQuit())         );
    connect (PMToolBar, SIGNAL(miniModeCall()),     SLOT(PMMiniMode())     );

    // create and add the new central widget. 
    PMInterface = new PUI (this, "pminterface_pui");
    setCentralWidget(PMInterface);

    // setup the defaults for the central widget,
    // first let's check for the file... if not, then set defaults.
    QString __fname(".qpcr1krc");
    if (!readProfile(__fname)) {
        // no rc file found... dont worry, we'll use these
        // defaults for now, and then create an rcfile
        // when we quit
        PMRadio = new PCP(PCPDefaultPort);
        PMRadio->PCPInit();
        PMRadio->PCPSetSpeed(B38400);
        delete PMRadio;
        PMRadio = new PCP(PCPDefaultPort, B38400);
        PMStatus=PMRadio->PCPQueryOn();
        if (!PMStatus) {
        	PMRadio->PCPPowerUp();
        }
        execRadioCommand("FREQUENCY", "0146000000");
        execRadioCommand("SQUELCH",   "00");
        execRadioCommand("VOLUME",    "00");
        execRadioCommand("MODE",      "NFM");
        execRadioCommand("FILTER",    "15");
        execRadioCommand("CTCSS",     "OFF");
        execRadioCommand("AGC",       "0");
        execRadioCommand("NBL",       "0");
        execRadioCommand("RFA",       "0");
    }

    // set your tool bar information
    QString __gotport(PMRadio->PCPGetPort());
    QString __gotspeed(PMRadio->PCPGetSpeed());
    PMStatusBar->setDevice(__gotport);
    PMStatusBar->setPower(PMRadio->PCPQueryOn());
    PMStatusBar->setBaudRate(__gotspeed);

    // create a timer that will update the signal strength
    // this timer will trigger every 250 milliseconds, and is
    // set to not (false) be a single shot timer.
    PMStatTimer = new QTimer(this, "pmstattimer_qtimer");
    connect(PMStatTimer, SIGNAL(timeout()), SLOT(updateStatusSlot()));
    PMStatTimer->start(250, FALSE);

	// set some default variables on the scanner...
	PMScanMode = 0;
	PMSigOk = true;
	// create a scanner timer and a linger (scan wait) timer
	// the scanner timer is 16ms for (16 ch/second) 
	PMScanTimer = new QTimer(this, "pmscantimer_qtimer");
	PMScWaitTimer = new QTimer(this, "pmscwaittimer_qtimer");
	connect(PMScanTimer, SIGNAL(timeout()), 
		SLOT(updateScanStatSlot()) );
	connect(PMScWaitTimer, SIGNAL(timeout()), 
		SLOT(updateScanStatSlot()) );
    
    // connect the signal emitted from the PUI object (which would be a 
    // radio call) to a local slot for the actual execution of the 
    // radio command
    connect(PMInterface, SIGNAL(execRadioCommand(const char*, const char*)),
		    SLOT(execRadioCommandSlot(const char*, const char*)) );
    connect(PMInterface, SIGNAL(scanCall(const char)),
		    SLOT(scanSlot(const char)) );

#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: Created\n");
#endif // DEBUG_VER_

	resize(600,600);
}

PMainWindow :: ~PMainWindow()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PMainWindow: Destroyed!\n");
    #endif // DEBUG_VER_

    PMRadio->PCPSetSpeed(B9600);
    delete PMRadio;
}

void PMainWindow :: closeEvent ( QCloseEvent *event )
{
	if (!PMClosing) {
		PMClosing=TRUE;
		this->PMQuit();
		event->ignore();
	} else {
		event->accept();
	}
}

void PMainWindow :: execRadioCommandSlot(const char *name, const char *value)
// it calls a private function to execRadioCommand() becuase that way
// i can connect this as a slot, /and/ call the private function in a
// standard c++ call, hence reusing code.
{
#ifdef DEBUG_VER_
    fprintf(stderr, "PMainWindow: EXECRADIOCOMMANDSLOT - %s <-> %s\n", name, value);
#endif DEBUG_VER_
    execRadioCommand(name, value);

}

void PMainWindow :: scanSlot(const char value)
// handles the scan requests
{
	if ((PMScanMode!=0) && (value!='S')) {
		// if it's already scanning /and/ we
		// are not being asked to stop
		// *This is useful if we are on a channel
		// *or frequency that is continuously active
		// *or a birdie. It skips to the next step
		// *or channel, and continues to scan below.
		// NOTE: we should stop the timers, just in
		// case.
		PMScanTimer->stop();
		PMScWaitTimer->stop();
		char __ss[2];
		__ss[0]=value;
		__ss[1]=0;

		if ((value == 'v')||(value == '^')) {
			execRadioCommand("STEP", __ss);
		} else if ((value == '<')||(value == '>')) {
			execRadioCommand("CHANNEL", __ss); 
		}
	}
		

	if (value!='S') {
		// if we are not being asked to stop
		// then let's stop signal strength checks,
		// and start the scantimer for single
		// shot mode, we dont want a race condition
		// where the radio cannot keep up with the
		// timer.
		PMSigOk=false;
		PMScanTimer->start(16, TRUE);
		PMScanMode = value;
	} else {
		// if we are being asked to stop, drop
		// the timers, and set the scan mode to 0 
		// also tell others that it's ok to query
		// for signal strenght
		PMSigOk=true;
		PMScanTimer->stop();
		PMScWaitTimer->stop();
		PMScanMode = 0;
	}
}

void PMainWindow :: updateStatusSlot()
{
	if (PMSigOk) {
		// if it's ok to check for signal strength
		// then let's do it.
		QString foo((const char*)PMRadio->PCPSigStrengthStr());
		QString foor(foo.right(2));
		PMStatusBar->setComm(foor);
		PMInterface->valueChangeSlot("SIGNAL", foo.right(2));
	}
}

void PMainWindow :: updateScanStatSlot()
// updates the scanner status. Basically every time we are called
// we want to stop the scan qtimer, and the scwait qtimer, check
// to see if the squelch is open, and if the squelch _is_ open
// then start the scwait timer, tell the app it's ok to query
// for signal strength, and return. Otherwise, we want to restart
// the 16ms timer, tell the app to hold of on checking for signal
// strength, increment the frequency/channel and return.
{
	char __uss_tempvar1[2];
	__uss_tempvar1[0]=PMScanMode;
	__uss_tempvar1[1]=0;
	
	if (PMRadio->PCPQuerySquelch()) {
		// yay, we picked up a signal, let's kick off the
		// 5s timer before next check.
		PMSigOk = TRUE;
		PMScWaitTimer->start(5000, TRUE);
	} else {
		// bummer, no signal, but fear NOT! For
		// we just increment the step/channel
		// and keep moving along.
		PMSigOk = FALSE;
		if ((PMScanMode == 'v')||(PMScanMode == '^')) {
			execRadioCommand("STEP", __uss_tempvar1);
		} else if ((PMScanMode == '<')||(PMScanMode == '>')) {
			execRadioCommand("CHANNEL", __uss_tempvar1);
		} else {
			fprintf(stderr, "PMainWindow::updateScanStatSlot() *INVALID SCAN MODE REQUEST*\n");
			return;
		}
		PMScanTimer->start(16, TRUE);
	}
}

void PMainWindow :: PMAbout()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: SLOT-PMABOUT\n");
#endif // DEBUG_VER_

    QMessageBox::message("QPcr1k for X-Windows",
			 "QPcr1k for X-Windows (C) 1999, 2000.\n"
			 "[by PolyWog and Javaman for Ghetto.Org]\n"
			 "GUI for the PCP/PCOMM API to control\n"
			 "the Icom PCR-1000 All-Band All-Mode \n"
			 "communications receiver.");
}

void PMainWindow :: PMNewProfile()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: SLOT-PMNEWPROFILE\n");
#endif // DEBUG_VER_
}

void PMainWindow :: PMLoadProfile()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PMainWindow: SLOT-PMLOADPROFILE\n");
    #endif // DEBUG_VER_
	QString __fname( QFileDialog::getOpenFileName() ); 
	fileName = &__fname;
	if (!__fname.isNull()) {
#ifdef DEBUG_VER_
		fprintf(stderr, "PMainWindow: PMLoadProfile(): got open filename %s", 
				(const char *)__fname);
#endif 
		readProfile(__fname,TRUE);
	}
}

void PMainWindow :: PMSaveAsProfile()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PMainWindow: SLOT-PMSAVEASPROFILE\n");
    #endif // DEBUG_VER_
	QString __fname ( QFileDialog::getSaveFileName() );
	fileName = &__fname;
	if (!__fname.isNull()) {
#ifdef DEBUG_VER_
		fprintf(stderr, "PMainWindow: PMSaveProfile(): got Save filename %s", 
				(const char *)__fname);
#endif
		writeProfile (__fname,TRUE);
	}
}

void PMainWindow :: PMSaveProfile()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: SLOT-PMSAVEPROFILE\n");
#endif // DEBUG_VER_
	QString __fname(".qpcr1k");
	writeProfile(__fname);
}

void PMainWindow :: PMQuit()
// write the radio settings to a file before quitting. We want to save
// the current state of the receiver as the default VFO setting. In this
// case _RcLastCh_. The VFO will always be channel 0
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PMainWindow: SLOT-PMQUIT\n");
    #endif // DEBUG_VER_

	QString __fname(".qpcr1krc");
	writeProfile(__fname);

    if (PMRadio != NULL) {
    	PMRadio->PCPPowerDown();
	    delete PMRadio;
    }
    qApp->quit();
}

void PMainWindow :: PMEditProfile()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: SLOT-PMEDITPROFILE\n");
#endif // DEBUG_VER_
    // create the profile dialog box now, so we can save some time later.
    // make _this_ the parent, send it it's internal name, and make it MODAL
    ProDlg PMProfileDlg (&PMRc, this, "pmprofiledlg_prodlg", true);

#ifdef DEBUG_VER_
    if ( PMProfileDlg.exec() ) {
        fprintf(stderr, "PMainWindow: PMProfileDlg->accept();\n");
    } else {
        fprintf(stderr, "PMainWindow: PMProfileDlg->reject();\n");
    }
#else // DEBUG_VER_
	PMProfileDlg.exec();
#endif // DEBUG_VER_


}

/* These are being a bitch, so fuq it...

void PMainWindow :: PMInit()
// send a radio initialization command to the unit
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: SLOT-PMINIT\n");
#endif // DEBUG_VER_

	// send the radio initialization command	
	PMRadio->PCPInit();

}

void PMainWindow :: PMPower()
{
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow: SLOT-PMPOWER\n");
#endif // DEBUG_VER_

	// send the radio power on command
	// PMStatus=PMRadio->PCPQueryOn();

	if (PMRadio->PCPQueryOn()) { 
#ifdef DEBUG_VER_
		fprintf(stderr, "Attempting to stop radio via PMPower()\n");
#endif // DEBUG_VER_	
		PMRadio->PCPPowerDown();
		PMStatus=false;
		PMStatTimer->stop();
	} else {
#ifdef DEBUG_VER_
		fprintf(stderr, "Attempting to start radio via PMPower()\n");
#endif // DEBUG_VER_	
		PCP *PMRadio1 = new PCP(PMRc.RcDevice);
		PMRadio1->PCPInit();
		PMRadio1->PCPSetSpeed(PMRc.RcSpeed);
		delete PMRadio1;
		PMRadio1 = new PCP(PMRc.RcDevice, PMRc.RcSpeed);
		delete PMRadio;
		PMRadio = PMRadio1;
		
		PMStatus=PMRadio->PCPQueryOn();
		PMStatTimer->start(1*1000, FALSE);
	}
	PMStatusBar->setPower(PMStatus);
	
}
*/

void PMainWindow :: PMMiniMode()
{
	PMInterface->setMiniMode(!PMInterface->miniMode());
	if (PMInterface->miniMode()) {
		resize(200,200);
	}
}

bool PMainWindow :: execRadioCommand(const char *name, const char *value)
// parses the command coming in and makes a call to the PCP
// object for the correct radio call. We will only call for a
// value change in the display if the exec'd command for the
// radio returns an ``ok'' code
//
// What's interesting is that we reference the VFO channel all
// throughout the following method. This has no bearing on whatever
// channel that we are on. If we are on channel 1, the frequency
// change call will be set even though the ``data'' is temporarily
// being stored in memory channel 0. Only if you press '+' on
// a channel other than zero will you be able to ``save'' the new
// frequency on that channel.
//
// With that in mind, for stepping up, we can also use channel 0
// (the VFO) to store each frequency change, without having to
// worry about which channel we are on to get the data. Because
// we know that channel 0 will always have the most current data.
//
{
    if (value==NULL) {
        // make sure that we WERE passed a value
        return false;
    }

    QString temp(value);
    int tempCh=0;
    bool val=false;

    if (strncmp(name, "VOLUME", 3)==0) {
        if (!PMRadio->PCPSetVolume(temp.toInt()))
          return false;
	strncpy(PMRc.RcVol, PMRadio->PCPGetVolumeStr(), sizeof(PMRc.RcVol)-1);
    } else if (strncmp(name, "SQUELCH", 3)==0) {
        if (!PMRadio->PCPSetSquelch(temp.toInt()))
          return false;
	strncpy(PMRc.RcSql, PMRadio->PCPGetSquelchStr(), sizeof(PMRc.RcSql)-1);
    } else if (strncmp(name, "FREQUENCY", 3)==0) {
        if (!PMRadio->PCPSetFreq(temp.toLong())) { return false; }
        strncpy(PMRc.RcMem[0].ChFreq,
                PMRadio->PCPGetFreqStr(),
                sizeof(PMRc.RcMem[0].ChFreq)-1   );
    } else if (strcmp(name, "MODE")==0) {
        if (!PMRadio->PCPSetMode(value)) { return false; }
        strncpy(PMRc.RcMem[0].ChMode,
                PMRadio->PCPGetModeStr(),
                sizeof(PMRc.RcMem[0].ChMode)-1   );
    } else if (strcmp(name, "FILTER")==0) {
        if (!PMRadio->PCPSetFilter(value)) { return false; }
        strncpy(PMRc.RcMem[0].ChFilter,
                PMRadio->PCPGetFilterStr(),
                sizeof(PMRc.RcMem[0].ChFilter)-1 );
    } else if (strncmp(name, "CTCSS", 3)==0) {
        if (!PMRadio->PCPSetToneSq(temp.toFloat())) {  return false; }
        strncpy(PMRc.RcMem[0].ChToneSq,
                PMRadio->PCPGetToneSqStr(),
                sizeof(PMRc.RcMem[0].ChToneSq)-1 );
    } else if (strcmp(name, "STEP")==0) {
	if (strncmp(value, "^", 1)==0) {
		// we want to go up one step
		QString __tempvar1(PMRc.RcMem[0].ChFreq);
		QString __tempvar2(ranger(0));
		int __tempvar3 = __tempvar1.toInt()+__tempvar2.toInt();
		sprintf(PMTemp, "%0.10d", __tempvar3);
		// i tested this and the string gets converted nicely
		execRadioCommand("FREQUENCY",PMTemp);
	} else if (strncmp(value, "v", 1)==0) {
		// we want to go down one step
		// we want to go up one step
		QString __tempvar1(PMRc.RcMem[0].ChFreq);
		QString __tempvar2(ranger(0));
		sprintf(PMTemp, "%0.10d", __tempvar1.toInt()-__tempvar2.toInt());
		// i tested this and the string gets converted nicely
		execRadioCommand("FREQUENCY",PMTemp);
	}
    } else if (strcmp(name, "MEMORIZE")==0) {
	// if we are in VFO mode, then we autoselect the next
	// highest unused channel. (which would be PMRc.RcMaxCh+1) if
	// we have it the max channel limit == 40.. then revert
	// to channel 40 and overwrite the stuff in the VFO with
	// what we want to store in memory (alert the user to the 
	// fact that his memories are full)
	// if we are overwriting a predetermined channel, then 
	// we have to do none of this.
	if (PMRc.RcCurrCh == 0) {
		PMRc.RcMaxCh++;
		if (PMRc.RcMaxCh>40) {
			PMRc.RcMaxCh = 40;
			PMRc.RcLastCh=PMRc.RcMaxCh;
			PMRc.RcCurrCh=0;
		} else {
			PMRc.RcLastCh=PMRc.RcMaxCh;
			PMRc.RcCurrCh=PMRc.RcMaxCh;
		}
	} 
        strncpy(PMRc.RcMem[PMRc.RcCurrCh].ChFreq,           
                PMRadio->PCPGetFreqStr(),       
                sizeof(PMRc.RcMem[PMRc.RcCurrCh].ChFreq)-1   );
        strncpy(PMRc.RcMem[PMRc.RcCurrCh].ChMode,
                PMRadio->PCPGetModeStr(),
                sizeof(PMRc.RcMem[PMRc.RcCurrCh].ChMode)-1   );
        strncpy(PMRc.RcMem[PMRc.RcCurrCh].ChFilter,
                PMRadio->PCPGetFilterStr(),
                sizeof(PMRc.RcMem[PMRc.RcCurrCh].ChFilter)-1 );
        strncpy(PMRc.RcMem[PMRc.RcCurrCh].ChToneSq,
                PMRadio->PCPGetToneSqStr(),
                sizeof(PMRc.RcMem[PMRc.RcCurrCh].ChToneSq)-1 );
        strcpy (PMRc.RcMem[PMRc.RcCurrCh].ChDesc, "New Memory");   // a nifty description
	execRadioCommand("CHANNEL", temp);
    } else if (strncmp(name, "REMOVE", 3)==0) {
	// let's remove the current channel

	char __channel[4]; // channel that we will eventually go to
	if (PMRc.RcCurrCh == 0) {
		// if we are in the VFO we cannot change that so
		// lets just return.
		return false;
	} else if (PMRc.RcCurrCh != PMRc.RcMaxCh) {
		// if we are not on the last channel... 
		// then start the memory moves.
		sprintf(__channel, "%d", PMRc.RcCurrCh);
		for (int ctr=PMRc.RcCurrCh; ctr < PMRc.RcMaxCh; ctr++) {
			// number of channel copies to be done are
			// done so in this loop
			PMRc.RcMem[ctr] = PMRc.RcMem[ctr+1];
		}
	} else {
		// if we haven't satisfied the above conditions
		// then we must be on the last channel. Go to the next
		// to last channel 
		sprintf(__channel, "%d", (PMRc.RcCurrCh-1));
	}

	// deincriment the last channel, and actually go to __channel
	PMRc.RcMaxCh--;
	execRadioCommand("CHANNEL", __channel);
    } else if (strncmp(name, "CHANNEL", 3)==0) {
        // set the temp to current channel so that we may
        // increment, or decrement it... otherwise set it
        // to the value passed in  or to the current channel
	// for the case of '+' which is a `newly added' memory
	// channel
        tempCh = PMRc.RcCurrCh;
        if ( strcmp(temp, "<") == 0 ) {
            tempCh--;
        } else if ( strcmp(temp, ">") == 0) {
            tempCh++;
	} else if ( strcmp(temp, "+") == 0) {
	    tempCh = PMRc.RcCurrCh;
        } else {	
	    tempCh = temp.toInt();
        }

	if ((tempCh>=40)||(tempCh>PMRc.RcMaxCh)) {
		tempCh=0;
	} else if (tempCh<0) {
		tempCh=PMRc.RcMaxCh;
	}

//	if ( (tempCh<40) && (tempCh>=0) && (tempCh<=PMRc.RcMaxCh) ) {
            // if it's a valid channel set it and execute
            PMRc.RcLastCh = PMRc.RcCurrCh;
            PMRc.RcCurrCh = tempCh;
            temp.setNum(tempCh);
            execRadioCommand("FREQUENCY", PMRc.RcMem[PMRc.RcCurrCh].ChFreq);
            execRadioCommand("MODE",      PMRc.RcMem[PMRc.RcCurrCh].ChMode);
            execRadioCommand("FILTER",    PMRc.RcMem[PMRc.RcCurrCh].ChFilter);
            execRadioCommand("CTCSS",     PMRc.RcMem[PMRc.RcCurrCh].ChToneSq);
            PMInterface->valueChangeSlot(name, (const char*)temp);
            return true;
//	} else {
//		// otherwise do nothing and return false
//	    return false;
//        }
    } else if (strncmp(name, "AGC", 3)==0) {
        (temp.toInt()==1)?(val=true):(val=false);
        if (!PMRadio->PCPSetAutoGain(val))
          return false;
	strncpy(PMRc.RcAgc, PMRadio->PCPGetAutoGainStr(), sizeof(PMRc.RcAgc)-1);
    } else if (strncmp(name, "NBL", 3)==0) {
        (temp.toInt()==1)?(val=true):(val=false);
        if (!PMRadio->PCPSetNB(val))
          return false;
	strncpy(PMRc.RcNbl, PMRadio->PCPGetNBStr(), sizeof(PMRc.RcNbl)-1 );
    } else if (strncmp(name, "RFA", 3)==0) {
        (temp.toInt()==1)?(val=true):(val=false);
        if (!PMRadio->PCPSetRFAttenuator(val))
          return false;
	strncpy(PMRc.RcRfa, PMRadio->PCPGetRFAttenuatorStr(), sizeof(PMRc.RcRfa)-1);
    } else if (strcmp(name, "STEPVAL")==0) {
	strncpy(PMRc.ScMem[0].ScStepV,
		value,
		sizeof(PMRc.ScMem[0].ScStepV)-1);
    } else if (strcmp(name, "STEPRANGE")==0) {
	strncpy(PMRc.ScMem[0].ScStepR,
		value,
		sizeof(PMRc.ScMem[0].ScStepR)-1);
    } else {
        fprintf(stderr, "Unknown Command: %s\n", value);
        return false;
    }

    PMInterface->valueChangeSlot(name, value);
    return true;
}

bool PMainWindow :: writeProfile (QString fname, bool force)
/* writes the profile out to a file */
{
    PMRcFile = new PFileIO();
    if (PMRcFile->PFOpen((const char*)fname,"w",force)) {
	// if we can open this file for writing.
	if ( PMRc.RcMagic == NULL) {
		bzero(&PMRc, sizeof(PMRc)); // this is always a safe thing to do
	}
	strcpy (PMRc.RcMagic,   QPCR1K_VERSION); // our magic number ;)
	PMRc.RcLastCh = PMRc.RcCurrCh;          // the channel we were last at
	strncpy(PMRc.RcMem[0].ChFreq,           // current VFO data below goes into
	        PMRadio->PCPGetFreqStr(),       // memory channel 0
                sizeof(PMRc.RcMem[0].ChFreq)-1   );
        strncpy(PMRc.RcMem[0].ChMode,
                PMRadio->PCPGetModeStr(),
                sizeof(PMRc.RcMem[0].ChMode)-1   );
        strncpy(PMRc.RcMem[0].ChFilter,
                PMRadio->PCPGetFilterStr(),
                sizeof(PMRc.RcMem[0].ChFilter)-1 );
        strncpy(PMRc.RcMem[0].ChToneSq,
                PMRadio->PCPGetToneSqStr(),
                sizeof(PMRc.RcMem[0].ChToneSq)-1 );
        strcpy (PMRc.RcMem[0].ChDesc, "VFO");   // a nifty description

	if ( PMRc.RcSql[0] == '\0' ) {
        	strncpy(PMRc.RcSql,
        		PMRadio->PCPGetSquelchStr(),
        		sizeof(PMRc.RcSql)-1);
	}

	if ( PMRc.RcVol[0] == '\0' ) {
        	strncpy(PMRc.RcVol,
        		PMRadio->PCPGetVolumeStr(),
        		sizeof(PMRc.RcVol)-1);
	}

	if ( PMRc.RcAgc[0] == '\0' ) {
        	strncpy(PMRc.RcAgc,
        		PMRadio->PCPGetAutoGainStr(),
        		sizeof(PMRc.RcAgc)-1);
	}

	if ( PMRc.RcNbl[0] == '\0' ) {
        	strncpy(PMRc.RcNbl,
        		PMRadio->PCPGetNBStr(),
        		sizeof(PMRc.RcNbl)-1 );
	}

	if ( PMRc.RcRfa[0] == '\0' ) {
        	strncpy(PMRc.RcRfa,
			PMRadio->PCPGetRFAttenuatorStr(),
			sizeof(PMRc.RcRfa)-1);
	}

	if ( PMRc.RcDevice[0] == '\0' ) {
        	strncpy(PMRc.RcDevice,
        		PMRadio->PCPGetPort(),
        		sizeof(PMRc.RcDevice)-1 );
	}

	if ( PMRc.RcSpeed == 0 ) {
        	PMRc.RcSpeed = PMRadio->PCPGetSpeed_t();
	}
	
	if ( (PMRc.ScMem[0].ScStepV[0]=='\0')||
		(PMRc.ScMem[0].ScStepR[0]=='\0') ) { // default value is 10 kHz
		
		strncpy(PMRc.ScMem[0].ScStepV,
			"10",
			sizeof(PMRc.ScMem[0].ScStepV));
			
		// range == 0, 1, 2 for Hz, kHz, and MHz		
		strncpy(PMRc.ScMem[0].ScStepR,
			"1",
			sizeof(PMRc.ScMem[0].ScStepR)-1);
	}
	
        // now that we have gathered _state_ information into our struct... and
        // assuming that all the channels were saved correctly by the profile
        // editor dialog box.(if not we have made sure that all null members get
	// populated above.) We shall write the contents of memory to file.
        PMRcFile->PFWrite(&PMRc, sizeof(PMRc));
        PMRcFile->PFClose();
	delete PMRcFile;
    } else {
    	delete PMRcFile;
	return false;
    }

	return true;

}	

bool PMainWindow :: readProfile (QString fname, bool force)
/* reads profile from file */
{
    bzero(&PMRc, sizeof(PMRc));

    if (__first_time) { // we only have to run this once
	__first_time=false;
	// to check the qpcr1k version
	__chk_fd=NULL;
	char __chk_name[128];
	bzero(__chk_name, sizeof(__chk_name));
	strncpy(__chk_name, (const char*)fname, sizeof(__chk_name)-1);
	char __chk_open[128];
	bzero(__chk_open, sizeof(__chk_open));
	if (!force) {
		strcpy(__chk_open, getenv("HOME"));
		strcat(__chk_open, "/");
		strcat(__chk_open, __chk_name);
	} else {
		strcpy(__chk_open, __chk_name);
	}
#ifdef DEBUG_VER_
	fprintf(stderr, "***checking magic number in %s***\n", __chk_open);
	fflush(stderr);
#endif // DEBUG_VER_ //
	char __chk_version[128];
	bzero(__chk_version, sizeof(__chk_version));
	__chk_fd = fopen(__chk_open, "r");
#ifdef DEBUG_VER_
	perror(__chk_open);
#endif // DEBUG_VER_
	if ( __chk_fd != NULL ) {
		// we found the file, now, let's check the
		// magic number
		fread(__chk_version, sizeof(char), 8, __chk_fd);
#ifdef DEBUG_VER_
		perror("read");
		fprintf(stderr, "***read magic number %s***\n", __chk_version);
		fflush(stderr);
#endif // DEBUG_VER_ //
		if (strncmp(__chk_version, QPCR1K_VERSION,8)!=0) {
			// opps wrong magic number
			fprintf(stderr, "***Wrong Magic Number***\n");
			fflush(stderr);
			fclose(__chk_fd);
			perror("fclose");
			return false;
		}  else {
#ifdef DEBUG_VER_
			// ok magic number
			fprintf(stderr, "***Magic Number Checks Out***\n");
			fflush(stderr);
#endif // DEBUG_VER_ //
			fclose(__chk_fd);
#ifdef DEBUG_VER_
			fprintf(stderr, "***closing profile***\n");
			fflush(stderr);
#endif // DEBUG_VER_ //
		}
	} else {
		// if no file was found
		return false;
	}

    } // this ``if'' above is run only once to make sure we
	// have the right version of the qpcr1krc.

    PMRcFile = new PFileIO();
    bzero(&PMRc, sizeof(PMRc));

    if (PMRcFile->PFOpen((const char*)fname,"r", force)) {
        // we have an rc file.
        if ( PMRcFile->PFRead(&PMRc, sizeof(PMRc)) ) {
            // tell the radio we want to change stuff
            // but only if we can read from the rc file ok.
		
            QString __RcLastCh;

            // create the radio device and configure it for the stored
            // speed. We must start at 9600 baud and then up it
            PMRadio = new PCP(PMRc.RcDevice);
            if (!PMRadio->PCPQueryOn()) { PMRadio->PCPInit(); }
            PMRadio->PCPSetSpeed(PMRc.RcSpeed);
            delete PMRadio;
            PMRadio = new PCP(PMRc.RcDevice, PMRc.RcSpeed);
            PMStatus = PMRadio->PCPQueryOn();
            if (!PMStatus) { PMRadio->PCPPowerUp(); }

            execRadioCommand("CHANNEL",   __RcLastCh.setNum(PMRc.RcLastCh));
            execRadioCommand("SQUELCH",   PMRc.RcSql);
            execRadioCommand("VOLUME",    PMRc.RcVol);
            execRadioCommand("AGC",       PMRc.RcAgc);
            execRadioCommand("NBL",       PMRc.RcNbl);
            execRadioCommand("RFA",       PMRc.RcRfa);
            execRadioCommand("STEPRANGE", PMRc.ScMem[0].ScStepR);
            execRadioCommand("STEPVAL",	  PMRc.ScMem[0].ScStepV);
        }
        delete PMRcFile;
    } else {
        delete PMRcFile;
        return false;
    }

    return true;
}

const char *PMainWindow::ranger(int value)
{
 	bzero(PMTemp, sizeof(PMTemp));
 	
 	QString __tempvar1(PMRc.ScMem[value].ScStepV);
 	QString __tempvar2(PMRc.ScMem[value].ScStepR);
	int __tempvar3=0;

 	
 	switch(__tempvar2.toInt()) {
 		case 0:
 			// for MHz
			__tempvar3 = __tempvar1.toFloat()*1000000;
 			sprintf(PMTemp, "%0.10d", __tempvar3);
 			break;
 		case 1:
 			// for kHz
			__tempvar3 = __tempvar1.toFloat()*1000;
 			sprintf(PMTemp, "%0.10d", __tempvar3);
 			break;
 		case 2:
 			// for Hz
			__tempvar3 = __tempvar1.toInt();
			sprintf(PMTemp, "%0.10d", __tempvar3);
 			break;
 		default:
 			return NULL;
 			break;
 	}
#ifdef DEBUG_VER_
	fprintf(stderr, "PMainWindow::Ranger() %s\n", PMTemp);
#endif // DEBUG_VER_
	return PMTemp;
}
