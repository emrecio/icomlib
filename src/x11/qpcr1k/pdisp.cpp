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

#include <qlayout.h>
#include <qstring.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qframe.h>
#include <qpalette.h>
// #include <qpixmap.h>
// #include "background.xpm"
#include "pdisp.h"

PDisplay :: PDisplay (QWidget *parent, const char *name)
           : QWidget(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PDisplay: Creating...\n");
    #endif // DEBUG_VER_

	// create two layouts to manage our data
	// +---------------------+---------------------+
	// |                     |+----------+--------+|
	// |   Main Grid         ||detail gri|d       ||
	// |                     |+----------+--------+|
	// you get the picture...
	PDMainGrid  = new QGridLayout(this, 1, 2, 0, -1, "pdmaingrid_qlayout"  );
	PDDetailGrid= new QGridLayout(4, 4, -1,  "pddetailgrid_qlayout");

	// first lets create the label widgets that we are going
	// to be using. setting thier default text, parents, and 
	// internal names. Please note the PDModValue *is* a qlabel
	// because of it's alpha chars. 
	PDPlLabel	= new QLabel("CTCSS",  this, "pdpllabel_qlabel");
	PDSigLabel  = new QLabel("SIGNAL", this, "pdsiglabel_qlabel");
	PDChLabel   = new QLabel("Channel",this, "pdchlabel_qlabel");
		
	// set frame styles for the label widgets. A box with a raised
	// border (thin) around it. While we are here, let's set the
	// alignment for the labels too.
	PDPlLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
	PDPlLabel->setAlignment( AlignRight | AlignVCenter);
	PDSigLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
	PDSigLabel->setAlignment( AlignRight | AlignVCenter);
	PDChLabel->setFrameStyle( QFrame::Box | QFrame::Raised );
	PDChLabel->setAlignment( AlignRight | AlignVCenter);
	
	// let's set some minimumsizes for the labels... we dont
	// want to get clipped
	PDPlLabel->setMinimumSize ( PDPlLabel->sizeHint() );
	PDSigLabel->setMinimumSize( PDSigLabel->sizeHint() );
	PDChLabel->setMinimumSize ( PDChLabel->sizeHint() );
	
	// now create the lcd's that we are going to use, while setting
	// their #of digits, parents, and internal names.
	PDFreqLcd	= new QLCDNumber(12, this, "pdfreqlcd_qlcdnumber");
	PDPlLcd	    = new QLCDNumber(5,  this, "pdpllcd_qlcdnumber"  );
	PDSigLcd	= new QLCDNumber(3,  this, "pdsiglcd_qlcdnumber" );
	PDChLcd     = new QLCDNumber(3,  this, "pdchlcd_qlcdnumber"  );
	
	// let's touch up the lcd screen, make it prettier ;)
	// set filled = setting foreground colors to the raised bits
	PDFreqLcd->setSegmentStyle( QLCDNumber::Filled );
	PDPlLcd->setSegmentStyle  ( QLCDNumber::Filled );
	PDSigLcd->setSegmentStyle ( QLCDNumber::Filled );
	PDSigLcd->setMode( QLCDNumber::Hex );
	PDChLcd->setSegmentStyle  ( QLCDNumber::Filled );

    // set the frequency display colors
    QPalette currPalette (PDFreqLcd->palette());
    currPalette.setColor(QColorGroup::Foreground, QColor( 0x5f, 0xfb, 0x17 ));
    currPalette.setColor(QColorGroup::Background, QColor( 0x00, 0x00, 0x00 ));
    PDFreqLcd->setPalette( (const QPalette)currPalette, true );
    PDPlLcd->setPalette  ( (const QPalette)currPalette, true );
    PDSigLcd->setPalette ( (const QPalette)currPalette, true );
    PDChLcd->setPalette  ( (const QPalette)currPalette, true );
	
	// add the frequency lcd widget into the main grid layout
	// in this case it will be added to the left box
	// and normally we would add the detailgrid layout to the main
	// layout here... but we have created PDDetailGrid to be a child
	// of PDMainGrid. There is no need for addLayout()
	PDMainGrid->addWidget(PDFreqLcd, 0, 0);
	PDMainGrid->addLayout(PDDetailGrid, 0, 1); 

	// insert the name/value pairs into the detail layout
	PDDetailGrid->addWidget(PDPlLabel,  0, 0); // label at row 1, col 1
	PDDetailGrid->addWidget(PDPlLcd,    0, 1); // value(lcd) at row 1, col 2
	PDDetailGrid->addWidget(PDSigLabel, 1, 0);
	PDDetailGrid->addWidget(PDSigLcd,   1, 1);
	PDDetailGrid->addWidget(PDChLabel,  0, 2);
	PDDetailGrid->addWidget(PDChLcd,    0, 3);
	
	// there are no signals emitted from these
	// widgets that we are interested in. Only thing
	// that we are interested in is the slots

    #ifdef DEBUG_VER_
    fprintf(stderr, "PDisplay: Created!\n");
    #endif // DEBUG_VER_
}

PDisplay :: ~PDisplay()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PDisplay: Destroyed!\n");
    #endif // DEBUG_VER_
}

void PDisplay :: valueChangeSlot(const char *name, const char *value)
// parses name/value pairs passed along to it
// and sets the correspodning widgets. we could
// have done individual slots for each widget
// that we created, but that would have involved
// more work for the parent object as we addedd
// additional display widgets.
{

    QString temp(value);


    if (strncmp(name, "CTCSS", 3)==0) {
	    PDPlLcd->display(temp.left(5));
    } else if (strncmp(name, "SIGNAL", 4)==0) {	
        PDSigLcd->display(temp);
    } else if (strncmp(name, "CHANNEL", 4)==0) {	
        PDChLcd->display(temp);
    } else {
	    return ;
    }


}

void PDisplay :: freqChangeSlot(const char *value)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PDisplay: FREQCHANGESLOT - %s\n", value);
    #endif // DEBUG_VER_

    QString temp(value);
	// copy the value into a qstring, insert a
	// decimal point for the khz, and mhz. 
	temp.insert(7,'.');
	temp.insert(4,'.');
	// check to see if the first three chars are
	// 0 , then first two, else just the first one
	// and replaces them with spaces
	if (temp.at(0)=='0') {
		if (temp.at(1)=='0') {
			if (temp.at(2)=='0') {
				temp.replace(0, 3, "   ");
			} else {
				temp.replace(0, 2, "  ");
			}
		} else {
			temp.replace(0, 1, " ");
		}
	}
    PDFreqLcd->display(temp);
}
