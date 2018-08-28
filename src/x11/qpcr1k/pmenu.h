#ifndef PMENU_H_
#define PMENU_H_
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
#include <qmenubar.h>

#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

class QPopupMenu;

class PMenu : public QMenuBar
{
    Q_OBJECT

    public:
        PMenu(QWidget *parent=0, const char *name=0);
        ~PMenu();

    public slots:
        // File Popup Menu
	void PMFNew();
        void PMFLoad();     // load profile
        void PMFSaveAs();   // save as profile
        void PMFSave();     // save profile
        void PMFExit();     // exit program
        // Edit Popup Menu
        void PMEProfile();  // edit profile
        // Command Popup Menu
//	void PMCInit();     // (re)Initialise radio
//	void PMCPower();    // power up/down
        // Help Popup Menu
        void PMHAbout();    // about box

    signals:
        // corresponding signals
	void newProfileCall();
        void loadProfileCall();
        void saveasProfileCall();
        void saveProfileCall();
        void exitProgramCall();
        void editProfileCall();
//	void initRadioCall();
//	void powerRadioCall();
        void aboutBoxCall();

    private:
        QPopupMenu  *PMFile;
        QPopupMenu  *PMEdit;
        QPopupMenu  *PMCommand;
        QPopupMenu  *PMHelp;
};

#endif /* PMENU_H_ */
