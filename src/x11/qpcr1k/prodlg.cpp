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


#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qsplitter.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qpushbutton.h>

#include <stdlib.h> // for free() against the malloc()

#include "prodlg.h"
#include "proedit.h"
#include "pfileio.h"

ProDlg :: ProDlg (qpcrRc *passedRc, QWidget *parent, const char *name, bool modalness)
         : QDialog(parent, name, modalness)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "ProDlg: Creating...\n");
    fflush(stderr);
    #endif // DEBUG_VER_

    tempRc = new qpcrRc;
    bzero(tempRc, sizeof(tempRc));
    memcpy(tempRc, passedRc, sizeof(qpcrRc));
    origRc = passedRc;

    // create a layout object (in this case a grid layout 1x1) so
    // that our widget sizes can be resized automagically. Then
    // create a splitter, and add the splitter to the grid layout.
    PDlMainGL   = new QGridLayout( this, 1, 1, 1, 1, "pdlmaingl");
    PDlMainSplit = new QSplitter( QSplitter::Horizontal, this );
    PDlMainGL->addWidget( PDlMainSplit, 0, 0 );

    // create the listview to left of the splitter, add a column
    // and set some pretty options, then connect the listview's
    // signal (selectionChanged(QListViewItem *)) to _this_'s
    // slot (PDlSelectionChangedSlot(QListViewItem *))
    PDlOptsLV   = new QListView( PDlMainSplit );
    PDlOptsLV->addColumn("Main Options");
    PDlOptsLV->addColumn("Details");
    PDlOptsLV->addColumn("Description");

    PDlOptsLV->setRootIsDecorated( TRUE );
    connect (PDlOptsLV, SIGNAL(selectionChanged( QListViewItem * )),
		this, SLOT(PDlSelectionChangedSlot( QListViewItem * )) );
    connect (PDlOptsLV, SIGNAL(doubleClicked( QListViewItem *)),
		this, SLOT(PDlDoubleClickedSlot( QListViewItem * )) );	

	// populate our listviews...
	PDlGlobalsLVI[0] = 0;
	PDlChannelsLVI[0] = 0;
	PDlPopulate();
	
    // make a group box that will gather our editing abilities
    // together. Make its parent the splitter so it will appear
    // on the right of the splitter. Give the group box one column
    // and set the orientation to horizontal.
    PDlEditGB = new QGroupBox("Info", PDlMainSplit);
    PDlEditGB->setColumnLayout(1, QGroupBox::Horizontal);

    // set some default stuff. While making its parent the group box
    PDlNameLabel = new QLabel("Drag the slider on\n"
                              "the left to access\n"
                              "multiple options",   PDlEditGB);

    PDlCommitPB     = new QPushButton("Commit", PDlEditGB );
    connect(PDlCommitPB, SIGNAL(clicked()), SLOT(PDlCommitPBSlot()));
    PDlRollbackPB   = new QPushButton("Rollback", PDlEditGB );
    connect(PDlRollbackPB, SIGNAL(clicked()), SLOT(PDlRollbackPBSlot()));

    // activate grid layout where all of this will live
    PDlMainGL->activate();
    resize(382,210);

	setCaption("QPcr1k - Profile Editor");

    #ifdef DEBUG_VER_
    fprintf(stderr, "ProDlg: Created!\n");
    #endif // DEBUG_VER_
}

ProDlg :: ~ProDlg()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "ProDlg: Destroyed!\n");
    #endif // DEBUG_VER_

    free(tempRc);
}

void ProDlg :: PDlSelectionChangedSlot( QListViewItem *currItem )
/*
 *  This slot is connected to the listview. Basically, the way it
 *  works ir rather simple. If there are any name/value/info boxes
 *  in the edit group box in the right pane, delete them.
 *
 *  If we were fed a QListViewItem then set the groupbox title to
 *  the listitem's main name (the name in col index 0). Then create
 *  the appropriate name/value pairs.
 *
 *  If we werent fed a QListViewItem then set it to the default
 *  description box.
 */
{
#ifdef DEBUG_VER_
    fprintf( stderr, "PRODLG: PDLSELECTIONCHANGEDSLOT\n");
    fflush( stderr );
#endif // DEBUG_VER_

    if (currItem!=0) {
        PDlEditGB->setTitle(currItem->text(0));
        if (strcmp(currItem->text(0),"Device")==0) {
            PDlNameLabel->setText("Device\nDouble click to edit");
            PDlCommitPB->show();
            PDlRollbackPB->show();
        } else if (strcmp(currItem->text(0),"BaudRate")==0) {
            PDlNameLabel->setText("BaudRate\nDouble click to edit");
            PDlCommitPB->show();
            PDlRollbackPB->show();
        } else if (strcmp(currItem->text(0),"Globals")==0) {
            PDlNameLabel->setText("Global Settings");
            PDlCommitPB->show();
            PDlRollbackPB->show();
        } else if (strcmp(currItem->text(0),"Channels")==0) {
            PDlNameLabel->setText("Memory Channels\nDouble click to edit");
            PDlCommitPB->show();
            PDlRollbackPB->show();
        } else {
            PDlEditGB->setTitle("Info");
            PDlNameLabel->setText("Drag the slider on\n"
                              "the left to access\n"
                              "multiple options");
            PDlCommitPB->show();
            PDlRollbackPB->show();
        }
    }

}

void ProDlg :: PDlDoubleClickedSlot( QListViewItem *currItem )
{
	if ( currItem->parent() != NULL ) {
		QString __parentStr(currItem->parent()->text(0));
		QString __childStr(currItem->text(0));
		if (strncmp((const char*)__parentStr,"Channels",4)==0) {
			ProEdit *memEditor = new ProEdit(&__childStr, tempRc, 0,
					  this, "memeditor", TRUE);
			memEditor->exec();
			delete memEditor;
		} else if (strncmp((const char*)__parentStr, "Globals",4)==0) {
			if (strncmp((const char*)__childStr, "Device", 4)==0) {
				ProEdit *devEditor = new ProEdit(&__childStr, tempRc, 1,
					  	this, "deveditor", TRUE);
				devEditor->exec();
				delete devEditor;
        		} else if (strncmp((const char*)__childStr, "BaudRate", 4)==0) {
				ProEdit *baudEditor = new ProEdit(&__childStr, tempRc, 2, 
					   	this, "baudeditor", TRUE);
				baudEditor->exec();
				delete baudEditor;
			}
		} 
		PDlPopulate();		
	}
}

void ProDlg :: PDlCommitPBSlot()
// commit changes in this edit screen
{
	bzero(origRc, sizeof(qpcrRc));
	memcpy(origRc, tempRc, sizeof(qpcrRc));
    accept();
}

void ProDlg :: PDlRollbackPBSlot()
{
    reject();
}

void ProDlg :: PDlPopulate()
{
	if (PDlGlobalsLVI[0] != 0) {    delete PDlGlobalsLVI[0]; }
	if (PDlChannelsLVI[0]!= 0) {	delete PDlChannelsLVI[0];}

    // add the two parent listview items. There may be more later.
    QString channels("Channels");
    QString globals("Globals");
    PDlGlobalsLVI[0] = new QListViewItem(PDlOptsLV, globals);
    PDlChannelsLVI[0] = new QListViewItem(PDlOptsLV, channels);

        int i=0;
        QString strRcMem;
        while ((i<=tempRc->RcMaxCh)&&(i<40)) {
		// assign the qstring the current memory channel
		// so that we can add it to the listview. Then if
		// we have less than two chars, pad with a '0' on
		// the left so that it will sort correctly on the
		// listview. 
		strRcMem.setNum(i);
		strRcMem = strRcMem.rightJustify(2, '0'); 
		PDlChannelsLVI[i+1] = new QListViewItem (PDlChannelsLVI[0],
			strRcMem,
			tempRc->RcMem[i].ChFreq,
			tempRc->RcMem[i].ChDesc);
		i++;
        }

        QString globals1("Device");
        QString globals2("BaudRate");
        PDlGlobalsLVI[1] = new QListViewItem (PDlGlobalsLVI[0], globals1);
        PDlGlobalsLVI[2] = new QListViewItem (PDlGlobalsLVI[0], globals2);

	PDlOptsLV->setOpen( PDlGlobalsLVI[0], TRUE );
	PDlOptsLV->setOpen( PDlChannelsLVI[0], TRUE );
}
