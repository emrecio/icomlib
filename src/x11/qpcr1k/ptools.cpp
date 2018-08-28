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

#include <qpixmap.h>
#include <qiconset.h>
#include <qtoolbutton.h>
#include <qstring.h>

#include "ptools.h"
#include "images.xpm"

//PTools is derived from QToolBar
PTools::PTools(const QString &label, QMainWindow *mainWindow, QWidget *parent,
               bool newline, const char *name, WFlags f)
      :QToolBar(label, mainWindow, parent, newline, name, f)
{
#ifdef DEBUG_VER_
    fprintf(stderr, "PTools: Creating...\n");
#endif // DEBUG_VER_

    // we have created a tool bar. Now let's populate the 
    // toolbar with the correct icons, etc...

    // create QPixmap objects, based on their respective names in images.xpm
    // feel the QPixmap objects into the QToolButtons. QToolButtons have been
    // created such that, their tooltip names, status bar desc appear. They
    // have been connected to local slots. Again, i wanted to keep this object
    // totally separate from its parent (hence OOP) so what i have done is
    // have the local slots emit a signal. Then the parent of us will connect
    // our emitted signals to their respective slots. QToolButton's parent has
    // been set to this object (a derivative of QToolBar) and their internal
    // name has been set respectively.

	QPixmap PTPNew  ( filenew_xpm  );
	QPixmap PTPSave ( filesave_xpm );
	QPixmap PTPLoad ( fileopen_xpm );
	QPixmap PTPExit ( exit_xpm     );
	QPixmap PTPEdit ( edit_xpm     );
//	QPixmap PTPInit ( init_xpm     );
//	QPixmap PTPPower( power_xpm    );
	QPixmap PTPHelp ( help_xpm     );
	QPixmap PTPMini ( minime_xpm   );

	PTBNew = new QToolButton(PTPNew,  "New",  "New profile / reset to defaults",
			this, SLOT(PTNewSlot()),  this, "new_toolbutton"   );
	PTBSave= new QToolButton(PTPSave, "Save", "Save profile to file",
			this, SLOT(PTSaveSlot()), this, "save_toolbutton"  );
	PTBLoad= new QToolButton(PTPLoad, "Load", "Load profile from file",
			this, SLOT(PTLoadSlot()), this, "load_toolbutton"  );
	PTBEdit= new QToolButton(PTPEdit, "Edit", "Edit profile",
			this, SLOT(PTEditSlot()), this, "edit_toolbutton"  );
//	PTBInit= new QToolButton(PTPInit, "Init", "Initialize Radio",
//			this, SLOT(PTInitSlot()), this, "init_toolbutton"  );
//	PTBPower = new QToolButton(PTPPower,"Power","Power up/down radio",
//			this, SLOT(PTPowerSlot()),this, "power_toolbutton" );
	PTBExit  = new QToolButton(PTPExit, "Quit", "Quit Program",
			this, SLOT(PTExitSlot()), this, "exit_toolbutton"  );
	PTBHelp  = new QToolButton(PTPHelp, "Help", "Help",
			this, SLOT(PTHelpSlot()), this, "help_toolbutton"  );
	PTBMini = new QToolButton(PTPMini, "Mini", "Mini-me mode",
			this, SLOT(PTMiniSlot()), this, "mini_toolbutton" );

#ifdef DEBUG_VER_
    fprintf(stderr, "PTools: Created!\n");
#endif // DEBUG_VER_
}

PTools :: ~PTools()
{
#ifdef DEBUG_VER_
    fprintf(stderr, "PTools: Destroyed!\n");
#endif // DEBUG_VER_
}

void PTools :: PTNewSlot()
{
    emit newProfileCall();
}

void PTools :: PTSaveSlot()
{
    emit saveProfileCall();
}

void PTools :: PTLoadSlot()
{
    emit loadProfileCall();
}

void PTools :: PTEditSlot()
{
    emit editProfileCall();
}

//
//void PTools :: PTInitSlot()
//{
//    emit initRadioCall();
//}
//
//void PTools :: PTPowerSlot()
//{
//    emit powerRadioCall();
//}
//

void PTools :: PTExitSlot()
{
    emit exitProgramCall();
}

void PTools :: PTHelpSlot()
{
    emit aboutBoxCall();
}

void PTools :: PTMiniSlot()
{
	emit miniModeCall();
}
