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

#include <qpushbutton.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qstring.h>
#include <qlabel.h>
#include <qtooltip.h>
#include <qlineedit.h>
#include <qpixmap.h>
#include <qtimer.h>

#include "pkeybd.h"
// #include "btn_img.xpm"

PKeyBoard :: PKeyBoard (QWidget *parent, const char *name)
           : QWidget(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PKeyBoard: Creating...\n");
    #endif // DEBUG_VER_

	PKScrollTimer = new QTimer(this, "pkscrolltimer");
	connect(PKScrollTimer, 	SIGNAL(timeout()),
		SLOT(scanEffectSlot()) );
	PKQueryTimer = new QTimer(this, "pkquerytimer");
	connect(PKQueryTimer,	SIGNAL(timeout()),
		SLOT(queryEffectSlot()) );

	PKGrid = new QGridLayout(this, 5, 4, 0, -1, "pkgrid_qlayout");
	PKChMgrHBL = new QHBoxLayout();
	PKChBrwHBL = new QHBoxLayout();
	PKFrStpHBL = new QHBoxLayout();

/* We are not ready to add pooffyness yet... ;)

	QPixmap PKNo01 	( no_1_xpm  );	
	QPixmap PKNo02	( no_2_xpm  );
	QPixmap PKNo03	( no_3_xpm  );
	QPixmap PKNo04	( no_4_xpm  );
	QPixmap PKNo05	( no_5_xpm  );
	QPixmap PKNo06	( no_6_xpm  );
	QPixmap PKNo07 	( no_7_xpm  );
	QPixmap PKNo08	( no_8_xpm  );
	QPixmap PKNo09	( no_9_xpm  );
	QPixmap PKNo00	( no_0_xpm  );
	QPixmap PKNoCl	( no_cl_xpm );
	QPixmap PKNoDt	( no_dt_xpm );
	QPixmap PKNoEn	( no_en_xpm );
	QPixmap PKNoSc	( no_sc_xpm );
	QPixmap PKChAd	( ch_ad_xpm );
	QPixmap PKChDn	( ch_dn_xpm );
	QPixmap PKChRm	( ch_rm_xpm );
	QPixmap PKChUp	( ch_up_xpm );
*/
	

	PK01 = new QPushButton("1",     this, "pk01_qpushbutton");
	PK02 = new QPushButton("2",     this, "pk02_qpushbutton");
	PK03 = new QPushButton("3",     this, "pk03_qpushbutton");
	PK04 = new QPushButton("4",     this, "pk04_qpushbutton");
	PK05 = new QPushButton("5",     this, "pk05_qpushbutton");
	PK06 = new QPushButton("6",     this, "pk06_qpushbutton");
	PK07 = new QPushButton("7",     this, "pk07_qpushbutton");
	PK08 = new QPushButton("8",     this, "pk08_qpushbutton");
	PK09 = new QPushButton("9",     this, "pk09_qpushbutton");
	PKST = new QPushButton("<<",    this, "pk11_qpushbutton");
        	QToolTip::add(PKST, "Channel Down");
	PK00 = new QPushButton("0",     this, "pk10_qpushbutton");
	PKPD = new QPushButton(">>",    this, "pk12_qpushbutton");
        	QToolTip::add(PKPD, "Channel Up");
	PKEN = new QPushButton("Enter", this, "pken_qpushbutton");
		QToolTip::add(PKEN, "Activate the selected frequency");
	PKDT = new QPushButton(".",     this, "pkdt_qpushbutton");
		QToolTip::add(PKDT, "Dot");
	PKCR = new QPushButton("Clr", this, "pkcr_qpushbutton");
		QToolTip::add(PKCR, "Clear Entry");
	PKMP = new QPushButton("+", this, "pkmp_qpushbutton");
		QToolTip::add(PKMP, "Channel Alter/Add");
	PKMM = new QPushButton("-", this, "pkmm_qpushbutton");
		QToolTip::add(PKMM, "Channel Remove");
	PKScanTB = new QPushButton("Scan", this, "pkscantb_qpushbutton");
		PKScanTB->setToggleButton(TRUE);
		QToolTip::add(PKScanTB, "(de)Activate Scan Mode");
	PKSU = new QPushButton("^", this, "pksu_qpushbutton");
		QToolTip::add(PKSU, "Up one Step");
	PKSD = new QPushButton("v", this, "pksd_qpushbutton");
		QToolTip::add(PKSD, "Down one Step");

	PKDirEntLE = new QLineEdit(this, "pkdirentle");

	PKGrid->addWidget(PK01, 0, 0);
	PKGrid->addWidget(PK02, 0, 1);
	PKGrid->addWidget(PK03, 0, 2);
	PKGrid->addWidget(PK04, 1, 0);
	PKGrid->addWidget(PK05, 1, 1);
 	PKGrid->addWidget(PK06, 1, 2);
	PKGrid->addWidget(PK07, 2, 0);
	PKGrid->addWidget(PK08, 2, 1);
	PKGrid->addWidget(PK09, 2, 2);
	PKGrid->addWidget(PKCR, 3, 0);
	PKGrid->addWidget(PK00, 3, 1);
	PKGrid->addWidget(PKDT, 3, 2);

	PKGrid->addWidget(PKEN,       0, 3);
	PKGrid->addWidget(PKScanTB,   1, 3);
	PKGrid->addLayout(PKChBrwHBL, 2, 3);
	 PKChBrwHBL->addWidget(PKST);
	 PKChBrwHBL->addWidget(PKPD);
	PKGrid->addLayout(PKChMgrHBL, 3, 3);
	 PKChMgrHBL->addWidget(PKMP);
	 PKChMgrHBL->addWidget(PKMM);
	PKGrid->addMultiCellWidget(PKDirEntLE, 4, 4, 0, 2);
	PKGrid->addLayout(PKFrStpHBL, 4, 3);
	 PKFrStpHBL->addWidget(PKSU);
	 PKFrStpHBL->addWidget(PKSD);

	PKGrid->activate();

    connect(PK01, SIGNAL(clicked()), SLOT(key1PressSlot()) );
    connect(PK02, SIGNAL(clicked()), SLOT(key2PressSlot()) );
    connect(PK03, SIGNAL(clicked()), SLOT(key3PressSlot()) );
    connect(PK04, SIGNAL(clicked()), SLOT(key4PressSlot()) );
    connect(PK05, SIGNAL(clicked()), SLOT(key5PressSlot()) );
    connect(PK06, SIGNAL(clicked()), SLOT(key6PressSlot()) );
    connect(PK07, SIGNAL(clicked()), SLOT(key7PressSlot()) );
    connect(PK08, SIGNAL(clicked()), SLOT(key8PressSlot()) );
    connect(PK09, SIGNAL(clicked()), SLOT(key9PressSlot()) );
    connect(PKST, SIGNAL(clicked()), SLOT(keySPressSlot()) );
    connect(PK00, SIGNAL(clicked()), SLOT(key0PressSlot()) );
    connect(PKPD, SIGNAL(clicked()), SLOT(keyPPressSlot()) );
    connect(PKDT, SIGNAL(clicked()), SLOT(keyDPressSlot()) );
    connect(PKEN, SIGNAL(clicked()), SLOT(keyEPressSlot()) );
    connect(PKCR, SIGNAL(clicked()), SLOT(keyRPressSlot()) );
    connect(PKMP, SIGNAL(clicked()), SLOT(keyMPressSlot()) );
    connect(PKMM, SIGNAL(clicked()), SLOT(keyNPressSlot()) );
    connect(PKSU, SIGNAL(clicked()), SLOT(keySUPressSlot()));
    connect(PKSD, SIGNAL(clicked()), SLOT(keySDPressSlot()));
	connect(PKScanTB, 	SIGNAL(clicked()),
		SLOT(keyScanTBPressSlot()) );
	connect(PKDirEntLE, 	SIGNAL(returnPressed()), 
		SLOT(directEntrySlot()) );

#ifdef DEBUG_VER_
    fprintf(stderr, "PKeyBoard: Created!\n");
#endif // DEBUG_VER_
}

PKeyBoard :: ~PKeyBoard()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PKeyBoard: Destroyed!\n");
    #endif // DEBUG_VER_
}

void PKeyBoard :: key1PressSlot()
{

    emit keyPressCall("1");
}

void PKeyBoard :: directEntrySlot()
{
	PKTemp = PKDirEntLE->text();
	emit directEntryCall((const char*)PKTemp);
	PKDirEntLE->clear();
}

void PKeyBoard :: key2PressSlot()
{
    emit keyPressCall("2");
}

void PKeyBoard :: key3PressSlot()
{
    emit keyPressCall("3");
}

void PKeyBoard :: key4PressSlot()
{
    emit keyPressCall("4");
}

void PKeyBoard :: key5PressSlot()
{
    emit keyPressCall("5");
}

void PKeyBoard :: key6PressSlot()
{
    emit keyPressCall("6");
}

void PKeyBoard :: key7PressSlot()
{
    emit keyPressCall("7");
}

void PKeyBoard :: key8PressSlot()
{
    emit keyPressCall("8");
}

void PKeyBoard :: key9PressSlot()
{
    emit keyPressCall("9");
}

void PKeyBoard :: keySPressSlot()
{
	if (PKScanTB->isOn()) {
		PKQueryTimer->stop();
		PKScrollTimer->start(1000, FALSE);
		emit massiveChCall('<');
	} else {
		emit massiveChCall('S');
		emit keyPressCall("<");
	}
}

void PKeyBoard :: key0PressSlot()
{
	emit keyPressCall("0");
}

void PKeyBoard :: keyPPressSlot()
{
	if (PKScanTB->isOn()) {
		PKQueryTimer->stop();
		PKScrollTimer->start(1000, FALSE);
		emit massiveChCall('>');
	} else {
		emit massiveChCall('S');
		emit keyPressCall(">");
	}
}

void PKeyBoard :: keyDPressSlot()
{
    emit keyPressCall(".");
}

void PKeyBoard :: keyEPressSlot()
{
    emit keyPressCall(";");
}

void PKeyBoard :: keyRPressSlot()
{
    emit keyPressCall("R");
}

void PKeyBoard :: keyMPressSlot()
{
	emit keyPressCall("+");
}	

void PKeyBoard :: keyNPressSlot()
{
	emit keyPressCall("-");
}

void PKeyBoard :: keySUPressSlot()
{
	// step up emission
	if (PKScanTB->isOn()) {
		PKQueryTimer->stop();
		PKScrollTimer->start(1000, FALSE);
		emit massiveChCall('^');
	} else {
		emit massiveChCall('S');
		emit keyPressCall("^");
	}
}

void PKeyBoard :: keySDPressSlot()
{
	// step down emission
	if (PKScanTB->isOn()) {
		PKQueryTimer->stop();
		PKScrollTimer->start(1000, FALSE);
		emit massiveChCall('v');
	} else {
		emit massiveChCall('S');
		emit keyPressCall("v");
	}
}

void PKeyBoard :: keyScanTBPressSlot()
{
	// disables scanning
	if (PKScanTB->isOn()) {
		enableKeys(FALSE);
		PKQueryTimer->start(1000, FALSE);
	} else {
		enableKeys(TRUE);
		PKQueryTimer->stop();
		PKScrollTimer->stop();
		PKScanTB->setText("Scan");
		emit massiveChCall('S');
	}
}

void PKeyBoard :: scanEffectSlot()
{
	// provides for the scanner effect
	if (strcmp(" ", PKScanTB->text())==0) {
		PKScanTB->setText("Scanning");
	} else {
		PKScanTB->setText(" ");
	}
}	

void PKeyBoard :: queryEffectSlot()
{
	// provides to ask user how to scan
	if (strcmp("?", PKScanTB->text())==0) {
		PKScanTB->setText("v ^ << >>");
	} else {
		PKScanTB->setText("?");
	}
}

void PKeyBoard :: enableKeys(bool value)
// activates or deactivates the keys that are not used
// when in scan mode. True = activate, False = deactivate
{
	PKDirEntLE->setEnabled(value);
	PKEN->setEnabled(value);
	PKDT->setEnabled(value);
	PKMP->setEnabled(value);
	PKMM->setEnabled(value);
	PK01->setEnabled(value);
	PK02->setEnabled(value);
	PK03->setEnabled(value);
	PK04->setEnabled(value);
	PK05->setEnabled(value);
	PK06->setEnabled(value);
	PK07->setEnabled(value);
	PK08->setEnabled(value);
	PK09->setEnabled(value);
	PK00->setEnabled(value);
	PKCR->setEnabled(value);
}
