#ifndef PTOOLS_H_
#define PTOOLS_H_
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
#include <qtoolbar.h>

#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

class QIconSet;
class QPixmap;
class QToolButton;
class QString;

class PTools : public QToolBar
{
    Q_OBJECT

    public:
        PTools(const QString &label, QMainWindow *mainWindow, QWidget *parent=0,
               bool newLine=false, const char *name=0, WFlags f=0);
        ~PTools();

    signals:
        // corresponding signals
        void newProfileCall();
        void loadProfileCall();
        void saveProfileCall();
        void editProfileCall();
//        void initRadioCall();
//        void powerRadioCall();
        void exitProgramCall();
        void aboutBoxCall();
	void miniModeCall();

    private slots:
        // Save file
        void PTNewSlot();
        void PTSaveSlot();
        void PTLoadSlot();
        void PTEditSlot();
//        void PTInitSlot();
//        void PTPowerSlot();
        void PTExitSlot();
        void PTHelpSlot();
	void PTMiniSlot();

    private:
     QToolButton *PTBNew;
     QToolButton *PTBSave;
     QToolButton *PTBLoad;
     QToolButton *PTBEdit;
     QToolButton *PTBInit;
     QToolButton *PTBPower;
     QToolButton *PTBExit;
     QToolButton *PTBHelp;
	QToolButton *PTBMini;

};

#endif /* PTOOLS_H_ */
