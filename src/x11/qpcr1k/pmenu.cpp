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

#include <qkeycode.h>		//header file to map the accelerator keys
#include <qpopmenu.h>		//to add a pop up when mouseclick

#include "pmenu.h"

//PMenu is derived from QMenuBar
PMenu::PMenu(QWidget *parent, const char *name)
      :QMenuBar(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PMenu: Creating...\n");
    #endif // DEBUG_VER_

    // create the popup menu's
    PMFile = new QPopupMenu();
    PMEdit = new QPopupMenu();
    PMCommand = new QPopupMenu();
    PMHelp = new QPopupMenu();

    // check/confirm pointer's existance
    CHECK_PTR( PMFile );
    CHECK_PTR( PMEdit );
    CHECK_PTR( PMCommand );
    CHECK_PTR( PMHelp );

    // insert the File popup menu (_this_ before the first insertItem
    // is implied, since this is derived from QMenuBar) and its menuitems
    insertItem( "&File", PMFile );
//    PMFile->insertItem("&New",                this, SLOT(PMFNew()),      ALT+Key_N);
    PMFile->insertItem("&Load Profile...",    this, SLOT(PMFLoad()),     ALT+Key_L);
    PMFile->insertItem("&Export Profile...",  this, SLOT(PMFSaveAs()),   ALT+Key_E);
    PMFile->insertItem("&Save Profile",       this, SLOT(PMFSave()),     ALT+Key_S);
    PMFile->insertSeparator();
    PMFile->insertItem("E&xit",          this, SLOT(PMFExit()),     ALT+Key_X);
    // insert the Edit popup menu and its menuitems
    insertItem( "&Edit", PMEdit );
    PMEdit->insertItem("&Profile...",    this, SLOT(PMEProfile()),  ALT+Key_P);
    // PMEdit->insertItem("Style",          this, SLOT(PMEStyle())              );
    // insert the command popup menu and its menuitems
//    insertItem( "&Commands", PMCommand );
//    PMCommand->insertItem("&Initialize", this, SLOT(PMCInit()),     ALT+Key_I);
//    PMCommand->insertItem("Power &Up",   this, SLOT(PMCPower()),    ALT+Key_U);
//    PMCommand->insertItem("Power &Down", this, SLOT(PMCPower()),    ALT+Key_D);
    // insert the help popup menu and its menuitems
    insertItem( "&Help", PMHelp );
    PMHelp->insertItem("&About...",      this, SLOT(PMHAbout())              );

    #ifdef DEBUG_VER_
    fprintf(stderr, "PMenu: Created!\n");
    #endif // DEBUG_VER_

}

PMenu :: ~PMenu()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PMenu: Destroyed!\n");
    #endif // DEBUG_VER_
}

// Associate Our MenuBar's Slots with their emitted signals
void PMenu :: PMFNew()
{
    emit newProfileCall();
}

void PMenu :: PMFLoad()
{
    emit loadProfileCall();
}

void PMenu :: PMFSaveAs()
{
    emit saveasProfileCall();
}

void PMenu :: PMFSave()
{
    emit saveProfileCall();
}

void PMenu :: PMFExit()
{
    emit exitProgramCall();
}

void PMenu :: PMEProfile()
{
    emit editProfileCall();
}

// we'll be able to set the style from the menu in here
// void PMenu :: PMEStyle()
// {
//    qApp->setStyle(
// }
//
// void PMenu :: PMCInit()
//{
//    emit initRadioCall();
//}
//
//void PMenu :: PMCPower()
//{
//    emit powerRadioCall();
//}
//
void PMenu :: PMHAbout()
{
    emit aboutBoxCall();
}

