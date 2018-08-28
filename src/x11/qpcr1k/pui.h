#ifndef PUI_H_
#define PUI_H_
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

#include <qapp.h>
#include <qwidget.h>
#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

class QTimer;
class QGridLayout;
class QGroupBox;

class PKeyBoard;
class PDisplay;
class PControl;

class PUI : public QWidget
{
    Q_OBJECT

    public:
        PUI(QWidget *parent=0, const char *name=0);
        ~PUI();
	void setMiniMode(bool);
	bool miniMode();

    public slots:
	void valueChangeSlot(const char*, const char*);

    signals:
        // exec radio command
	void execRadioCommand(const char*, const char*);
	void scanCall(const char);

    private slots:
	void keyPressTOSlot();
	void keyPressMgrSlot(const char*);
	void valueChangeMgrSlot(const char*, const char*);
	void directEntrySlot(const char*);
	void massiveChSlot(const char); // used for scanning

    private:
	char 	    temp[256];
	char        curFreq[32]; // currently set freq
	char        newFreq[32]; // new freq to set
	bool        settingFreq; // are we in middle of setting freq?
	bool	    mhzSet;	 // have the MHz been set already with 
				 // the decimal point.
	QTimer      *PUTimeOut;  // inter-key timer for keyboard

        QGridLayout *PUGrid;

        QGroupBox   *PUKeyGrpBx;
        QGroupBox   *PUDispGrpBx;
        QGroupBox   *PUCtrlGrpBx;

        PKeyBoard   *PUKeyBd;
        PDisplay    *PUDisplay;
        PControl    *PUControl;

};

#endif /* PUI_H_ */
