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

#include <stdio.h>
#if defined (SunOS) || (Irix)
#include <strings.h>
#else if defined (BSD) || (Linux)
#include <string.h>
#endif

#include <qlayout.h>
#include <qstrlist.h>
#include <qslider.h>
#include <qlistbox.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qlcdnumber.h>
#include <qgroupbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>

#include "pctrl.h"

PControl :: PControl (QWidget *parent, const char *name)
         : QWidget(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: Creating...\n");
    #endif // DEBUG_VER_

	// create string lists that will hold our
	// combobox data. In this case, one for mode, 
	// and the other for filters and tone squelch 
	PCModList = new QStrList ( TRUE );
	PCFltList = new QStrList ( TRUE );
	PCTSqList = new QStrList ( TRUE );

	// these three lists will be auotmagically populated 
	// into the step list combo box.
	// when the user selects the MHz/kHz/Hz button 
	PCMHzLOV = new QStrList ( TRUE );
	PCkHzLOV = new QStrList ( TRUE );
	PCHzLOV  = new QStrList ( TRUE );

	PCModList->append("LSB");
	PCModList->append("USB");
	PCModList->append("AM");
	PCModList->append("CW");
	PCModList->append("UNUSED");
	PCModList->append("NFM");
	PCModList->append("WFM");

	PCFltList->append("3");
	PCFltList->append("6");
	PCFltList->append("15");
	PCFltList->append("50");
	PCFltList->append("230");	

	PCTSqList->append("Off");
	PCTSqList->append("67.0"); PCTSqList->append("69.3");
	PCTSqList->append("71.0"); PCTSqList->append("71.9");
	PCTSqList->append("74.4"); PCTSqList->append("77.0");
	PCTSqList->append("79.7"); PCTSqList->append("82.5");
	PCTSqList->append("85.4"); PCTSqList->append("88.5");
	PCTSqList->append("91.5"); PCTSqList->append("94.8");
	PCTSqList->append("97.4"); PCTSqList->append("100.0");
	PCTSqList->append("103.5"); PCTSqList->append("107.2");
	PCTSqList->append("110.9"); PCTSqList->append("114.8");
	PCTSqList->append("118.8"); PCTSqList->append("123.0");
	PCTSqList->append("127.3"); PCTSqList->append("131.8");
	PCTSqList->append("136.5"); PCTSqList->append("141.3");
	PCTSqList->append("146.2"); PCTSqList->append("151.4");
	PCTSqList->append("156.7"); PCTSqList->append("159.8");
	PCTSqList->append("162.2"); PCTSqList->append("165.5");
	PCTSqList->append("167.9"); PCTSqList->append("171.3");
	PCTSqList->append("173.8"); PCTSqList->append("177.3");
	PCTSqList->append("179.9"); PCTSqList->append("183.5");
	PCTSqList->append("186.2"); PCTSqList->append("189.9");
	PCTSqList->append("192.8"); PCTSqList->append("196.6");
	PCTSqList->append("199.5"); PCTSqList->append("203.5");
	PCTSqList->append("206.5"); PCTSqList->append("210.7");
	PCTSqList->append("218.1"); PCTSqList->append("225.7");
	PCTSqList->append("229.1"); PCTSqList->append("233.6");
	PCTSqList->append("241.8"); PCTSqList->append("250.3");
	PCTSqList->append("254.1");

	// population of the list of values for the
	// Hertz mode of the combo box
	PCHzLOV->append("10");	PCHzLOV->append("50");
	PCHzLOV->append("100");	PCHzLOV->append("250");
	PCHzLOV->append("500");

	// population of the list of values for the 
	// kilohertz mode of the combo box
	PCkHzLOV->append("1"); 		PCkHzLOV->append("2.5");
	PCkHzLOV->append("5");		PCkHzLOV->append("10");
	PCkHzLOV->append("12.5");	PCkHzLOV->append("15");
	PCkHzLOV->append("20"); 	PCkHzLOV->append("25");
	PCkHzLOV->append("30"); 	PCkHzLOV->append("50");
	PCkHzLOV->append("100");	PCkHzLOV->append("125");
	PCkHzLOV->append("200"); 	PCkHzLOV->append("500");
	PCkHzLOV->append("750");	

	// population of the list of values for the
	// megahertz mode of the combo box
	PCMHzLOV->append("1");		PCMHzLOV->append("2");
	PCMHzLOV->append("2.5");	PCMHzLOV->append("5");
	PCMHzLOV->append("10"); 	PCMHzLOV->append("12");

	// create the main layout, in this case it's a horizontal
	// layout as the ``main'' layout, whose parent is 
	// this widget.
	PCMainLayout = new QHBoxLayout( this, 5, -1 );
	PCVolGrpBx = new QGroupBox("Vol", this, "pcvolgrpbx");
	PCSqlGrpBx = new QGroupBox("Sql", this, "pcsqlgrpbx");
	
	PCVolGrpBx->setColumnLayout(1, Horizontal);
	PCSqlGrpBx->setColumnLayout(1, Horizontal);

	// create two sliders such that, the min: 0, the max: 99, the
	// step size is 5, and the initial value is 0, and the 
	// orientation of both these sliders are vertical
	PCVolLcd    = new QLCDNumber(2, PCVolGrpBx, "pcvollcd");
	PCSqlLcd    = new QLCDNumber(2, PCSqlGrpBx, "pcsqllcd");	
	PCVolSlider = new QSlider( 0,99,5,1, QSlider::Vertical, PCVolGrpBx,
	                                "pcvolslider_qslider");
	PCSqlSlider = new QSlider( 0,99,5,1, QSlider::Vertical, PCSqlGrpBx,
	                                "pcsqlslider_qslider");
	
	// give them ticks
	PCVolSlider->setTickmarks( QSlider::Right );
	PCSqlSlider->setTickmarks( QSlider::Right );
	
	// configure the lcd numbers (see pdisp.cpp it's identical)
	PCVolLcd->setSegmentStyle( QLCDNumber::Flat );
	PCSqlLcd->setSegmentStyle( QLCDNumber::Flat );
    QPalette currPalette (PCVolLcd->palette());
    currPalette.setColor(QColorGroup::Foreground, QColor( 0x5f, 0xfb, 0x17 ));
    currPalette.setColor(QColorGroup::Background, QColor( 0x00, 0x00, 0x00 ));
    PCVolLcd->setPalette( (const QPalette)currPalette, true );
    PCSqlLcd->setPalette( (const QPalette)currPalette, true );
	
    // left half add
    PCMainLayout->addWidget(PCVolGrpBx);
    PCMainLayout->addWidget(PCSqlGrpBx);	
	// create the right half of the widget. 
	PCRightLayout	= new QVBoxLayout(PCMainLayout, 5, "pcrightlayout");

	// since checkboxes have their own labels, we dont have to 
	// make layout containers for them, we can just create them
	// and add them directly to the VBoxLayout for the right side.
	PCAgcChkBx	= new QCheckBox("AutoGain Control", this, "agc_chkbx");
	PCNBlankChkBx	= new QCheckBox("Noise Blanking",   this, "nb_chkbx");
	PCRfAttnChkBx	= new QCheckBox("RF Attenuator",    this, "rfa_chkbx");
	PCRightLayout->addWidget(PCAgcChkBx);
	PCRightLayout->addWidget(PCNBlankChkBx);
	PCRightLayout->addWidget(PCRfAttnChkBx);
	
	// filter layout and a mode layout. Except, create these horizontally
	// so that we can place the description to the left of the  widget
	PCFltLayout	= new QHBoxLayout(PCRightLayout);
	PCModLayout	= new QHBoxLayout(PCRightLayout);
	PCTSqLayout	= new QHBoxLayout(PCRightLayout);

	PCFltComboBox	= new QComboBox( this, "pcfltcombobox");
	PCModComboBox	= new QComboBox( this, "pcmodcombobox");
	PCTSqListBox	= new QListBox ( this, "pctsqcombobox");
	PCFltLabel	= new QLabel("Filter", this, "pcvollabel", 0);
	PCModLabel	= new QLabel("Mode", this, "pcvollabel", 0);
	PCTSqLabel	= new QLabel("CTCSS", this, "pctsqlabel", 0);

	PCFltLabel->setAlignment( AlignRight | AlignVCenter );
	PCModLabel->setAlignment( AlignRight | AlignVCenter );
	PCTSqLabel->setAlignment( AlignRight | AlignVCenter );
	// now that they have been created, insert the stringlist
	PCFltComboBox->insertStrList( PCFltList );
	PCModComboBox->insertStrList( PCModList );
	PCTSqListBox->insertStrList( PCTSqList );
	// PCTSqListBox->setMinimumHeight( PCTSqListBox->minimumSizeHint().height()*2 );

	// now create the step group box and add it
	// to the layout on the righthand side then
	// create the combo box that's going to hold the
	// numeric values of the step size
	PCStpGrpBx = new QGroupBox("Step Size", this, "pcstpgrpbx");
	PCStpGrpBx->setColumnLayout(1, Horizontal);

	PCStpComboBox = new QComboBox( PCStpGrpBx, "pcstpcombobox");

	// create the button group that's going to 
	// organize our MHz, kHz, and Hz radio buttons.
	// make it's parent the step group box
	// then create the button groups such that their parent
	// is the button group.
	PCStpBtnGrp   = new QButtonGroup(3, Horizontal, PCStpGrpBx);
	PCMHzRadioBtn = new QRadioButton("MHz", PCStpBtnGrp, "pcmhzradiobtn");
	PCkHzRadioBtn = new QRadioButton("kHz", PCStpBtnGrp, "pckhzradiobtn");
	PCHzRadioBtn  = new QRadioButton("Hz", PCStpBtnGrp, "pchzradiobtn");

	// now get the id's of the buttons in the group this is just in case
	// we should happen to add more buttons to the group later
	PCMHzRadioBtnId = PCStpBtnGrp->id( PCMHzRadioBtn );
	PCkHzRadioBtnId = PCStpBtnGrp->id( PCkHzRadioBtn );
	PCHzRadioBtnId  = PCStpBtnGrp->id( PCHzRadioBtn );

	// lets set some default values
	// insert the kHz lov first and set the selected button to kHz
	// and set our current LOV pointer to the currently set one
	PCStpComboBox->insertStrList( PCkHzLOV ); 
	PCCurLOV = PCkHzLOV;
	PCStpBtnGrp->setButton( PCkHzRadioBtnId );

	// and add the widgets to the `right' layout
	PCFltLayout->addWidget(PCFltLabel);
	PCFltLayout->addWidget(PCFltComboBox);
	PCModLayout->addWidget(PCModLabel);
	PCModLayout->addWidget(PCModComboBox);
	PCTSqLayout->addWidget(PCTSqLabel);
	PCTSqLayout->addWidget(PCTSqListBox);
	PCRightLayout->addWidget(PCStpGrpBx);

	// now that we have created and added all of our
	// widgets, lets connect the signals that are 
	// interesting to us, to our slots
	// NOTE: the reason why we are using sliderMoved() 
	//       signal for the sliders, is because i want
	//       to set the volume realtime, instead of 
	//       whenever the slider is released (and has 
	//       assumed a new value.) However, i also
	//       want to set the value in case the user
	//       uses a keyboard... in that case i ALSO
	//       connect valueChanged(int)
	connect (PCVolSlider, SIGNAL(sliderMoved(int)), 
			SLOT(volSliderSlot(int)));
	connect (PCSqlSlider, SIGNAL(sliderMoved(int)), 
			SLOT(sqlSliderSlot(int)));
	connect (PCModComboBox, SIGNAL(activated(const QString &)), 
			SLOT(modComboBoxSlot(const QString &)));
	connect (PCFltComboBox, SIGNAL(activated(const QString &)), 
			SLOT(fltComboBoxSlot(const QString &)));
	connect (PCTSqListBox, SIGNAL(selected(const QString &)),
			SLOT(tsqComboBoxSlot(const QString &)));
	connect (PCAgcChkBx,       SIGNAL(clicked()), SLOT(agcChkBxSlot())    );
	connect (PCRfAttnChkBx,    SIGNAL(clicked()), SLOT(rfattnChkBxSlot()) );
	connect (PCNBlankChkBx,    SIGNAL(clicked()), SLOT(nblankChkBxSlot()) );

	connect (PCStpComboBox,  SIGNAL(activated(const QString &)), 
			SLOT(pcStpComboBoxSlot(const QString &)) );
	connect (PCStpBtnGrp,	SIGNAL(clicked(int)),
			SLOT(pcStpBtnGrpSlot(int)) );

    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: Created!\n");
    #endif // DEBUG_VER_
}

PControl :: ~PControl()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: Destroyed!\n");
    #endif // DEBUG_VER_
}

void PControl :: valueChangeSlot(const char *name, const char*value)
// parses name/value pairs passed along to it
// and sets the correspodning widgets. we could
// have done individual slots for each widget
// that we created, but that would have involved
// more work for the parent object as we addedd
// additional display widgets.
{
    #ifdef DEBUG_VER_2
    fprintf(stderr, "PControl: VALUECHANGESLOT - %s is now %s\n", 
            name, value);
    #endif // DEBUG_VER_

    QString temp(value);
    PCModList->first();
    PCFltList->first();
    int index;

    if (strncmp(name, "VOL", 3)==0) {
	// 99-temp.toInt() is a bad hack to fight
	// against upside down slider problem
            PCVolSlider->setValue(99-temp.toInt());
        	// AND THIS IS FOR YOU JAVAMAN
        	QToolTip::remove(PCVolSlider);
        	QToolTip::add(PCVolSlider, PCVolSlider->sliderRect(), temp);
        	PCVolLcd->display(temp);
    } else if (strncmp(name, "SQU", 3)==0) {
	// 99-temp.toInt() is a bad hack to fight
	// against upside down slider problem
		PCSqlSlider->setValue(99-temp.toInt());
        	PCSqlLcd->display(temp);
    } else if (strncmp(name, "FIL", 3)==0) {
	    index = PCFltList->find((const char*)temp);
            PCFltComboBox->setCurrentItem(index);
    } else if (strncmp(name, "MOD", 3)==0) {
	    index = PCModList->find((const char *)temp);
            PCModComboBox->setCurrentItem(index);
    } else if (strncmp(name, "CTCSS", 3)==0) {
	    index = PCTSqList->find((const char*)temp);
	    PCTSqListBox->setCurrentItem(index);
    } else if (strncmp(name, "RFATTENU", 3)==0) {
	(strncmp(value, "1", 1)==0)?
	  (PCRfAttnChkBx->setChecked(true)):
	  (PCRfAttnChkBx->setChecked(false));
    } else if (strncmp(name, "NBLANKING", 3)==0) {
	(strncmp(value, "1", 1)==0)?
	  (PCNBlankChkBx->setChecked(true)):
	  (PCNBlankChkBx->setChecked(false));
    } else if (strncmp(name, "AGC", 3)==0) {
	(strncmp(value, "1", 1)==0)?
	  (PCAgcChkBx->setChecked(true)):
	  (PCAgcChkBx->setChecked(false));
    } else if (strcmp(name, "STEPVAL")==0) {
		index = PCCurLOV->find((const char*)temp);
		PCStpComboBox->setCurrentItem(index);
    } else if (strcmp(name, "STEPRANGE")==0) {
		pcStpBtnGrpSelect(value);
    } else {
	return;
    }
}

void PControl :: volSliderSlot(int value)
// this is the slot for the volume control
// we change _value_ to a char string padded
// with one zero on the left if the strlen
// is less than two converted. and then emit
// the command string with the value
// of temp
{
    #ifdef DEBUG_VER_
    assert(value>=0 && value<=99);
    fprintf(stderr, "PControl: VOLSLIDERSLOT - %d\n", value);
    #endif // DEBUG_VER_

    // bad hack to take care of the ``upside down slider''
    value=99-value;

    char temp[8];
    if (value!=0) {
    	sprintf(temp, "%2.0d", value);
    } else {
        strcpy(temp,"0");
    }

    emit(valueChangeCall("VOLUME", temp));
}

void PControl :: sqlSliderSlot(int value)
// this is the slot for the sqlech control
// we change _value_ to a char string padded
// with one zero on the left if the strlen
// is less than two converted. and then emit
// the command string with the value
// of temp
{
    #ifdef DEBUG_VER_
    assert(value>=0 && value<=99);
    fprintf(stderr, "PControl: SQLSLIDERSLOT - %d\n", value);
    #endif // DEBUG_VER_

    // bad hack to take care of the ``upside down slider''
    value=99-value;

    char temp[8];
    if (value!=0) {
    	sprintf(temp, "%2.0d", value);
    } else {
        strcpy(temp,"0");
    }
    emit(valueChangeCall("SQUELCH", temp));
}

void PControl :: fltComboBoxSlot(const QString &value)
// here we just emit what was changed and
// its new value... we dont want to do any
// conversion here... let's leave that to 
// PMainWindow
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: FLTCOMBOBOXSLOT - %s\n", (const char*)value);
    #endif // DEBUG_VER_

    emit(valueChangeCall("FILTER", (const char*)value));
}

void PControl :: modComboBoxSlot(const QString &value)
// here we just emit what was changed and
// its new value... we dont want to do any
// conversion here... let's leave that to 
// PMainWindow
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: MODCOMBOBOXSLOT - %s\n", (const char*)value);
    #endif // DEBUG_VER_

    emit(valueChangeCall("MODE", (const char*)value));
}

void PControl :: tsqComboBoxSlot( const QString &value )
// here we just emit what was changed and
// its new value... we dont want to do any
// conversion here... let's leave that to 
// PMainWindow
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: TSQCOMBOBOXSLOT - %s\n", (const char*)value);
    #endif // DEBUG_VER_

    emit(valueChangeCall("CTCSS", (const char *)value));

}

void PControl :: agcChkBxSlot()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: AGCCHKBX \n");
    #endif // DEBUG_VER_ 

    (PCAgcChkBx->isChecked())?
	(strcpy(PCTemp, "1")):(strcpy(PCTemp,"0"));

    emit(valueChangeCall("AGC", PCTemp));
}

void PControl :: nblankChkBxSlot()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: NBLANKCHKBX \n");
    #endif // DEBUG_VER_ 

    (PCNBlankChkBx->isChecked())?
	(strcpy(PCTemp, "1")):(strcpy(PCTemp,"0"));

    emit(valueChangeCall("NBLANK", PCTemp));
}

void PControl :: rfattnChkBxSlot()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PControl: RFATTNCHKBX \n");
    #endif // DEBUG_VER_ 

    (PCRfAttnChkBx->isChecked())?
	(strcpy(PCTemp, "1")):(strcpy(PCTemp,"0"));

    emit(valueChangeCall("RFATTENUATOR", PCTemp));
}

void PControl :: pcStpComboBoxSlot(const QString &value)
// sets the current step when the user selects
// a new step setting by emitting the character
// string equivalent of what was selected.
{
	emit(valueChangeCall("STEPVAL", (const char*)value));
}

void PControl :: pcStpBtnGrpSlot(int value)
// sets the current step range when the selects
// a new step range button by emitting the
// character string of the value passed along
// the value would be the ID of the button pressed
// in this group.
{
	bzero(PCTemp, sizeof(PCTemp));
	sprintf(PCTemp, "%d", value);
	emit(valueChangeCall("STEPRANGE", PCTemp));
}

void PControl :: pcStpBtnGrpSelect(const char *value)
// it's passed a character string value for the 
// button that is to be selected. The character
// string gets converted in the switch statement
// into an integer. Then if the case matches, it
// sets the PCCurLOV pointer to the current selection
// clears the current string list in the combo box
// and inserts the new one. It then sets the active
// button to the passed integer value. If it cannot
// find the button id in the button group, it leaves
// everything untouched and returns to the calling
// function
{
	QString __temp(value);
        switch ( __temp.toInt() ) {
                case 0: // PCMHzRadioBtnId:
                        PCCurLOV = PCMHzLOV;
                        break;
                case 1: // PCkHzRadioBtnId:
                        PCCurLOV = PCkHzLOV;
                        break;
                case 2: // PCHzRadioBtnId:
                        PCCurLOV = PCHzLOV;
                        break;
                default:
                        return;
                        break;
        }

        PCStpComboBox->clear();
        PCStpComboBox->insertStrList( PCCurLOV );
	PCStpBtnGrp->setButton(__temp.toInt());
}
