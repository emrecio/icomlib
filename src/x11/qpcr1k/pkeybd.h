#ifndef PKEYBD_H_
#define PKEYBD_H_
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

class QPushButton;
class QGridLayout;
class QHBoxLayout;
class QLineEdit;
class QTimer;

class PKeyBoard : public QWidget
{
    Q_OBJECT

    public:
        PKeyBoard(QWidget *parent=0, const char *name=0);
        ~PKeyBoard();

    signals:
        // corresponding signals
        void keyPressCall(const char*);
	void directEntryCall(const char*);
	void massiveChCall(const char);

    private slots:
        // available slots to call
	void directEntrySlot();		// direct entry slot
        void key1PressSlot();		// various key press slots
        void key2PressSlot();
        void key3PressSlot();
        void key4PressSlot();
        void key5PressSlot();
        void key6PressSlot();
        void key7PressSlot();
        void key8PressSlot();
        void key9PressSlot();
        void keySPressSlot();
        void key0PressSlot();
        void keyPPressSlot();		// >>
        void keyDPressSlot();		// dot
        void keyEPressSlot();		// enter
	void keyRPressSlot();		// <<
	void keyMPressSlot();		// m+ (add channel)
	void keyNPressSlot();		// m- (sub channel)
	void keySUPressSlot();		// Step up
	void keySDPressSlot();		// Step down
	void keyScanTBPressSlot();	// start/stop scanning
	void scanEffectSlot();		// scan button effects
	void queryEffectSlot();		// scan button effects

    private:
    	void enableKeys(bool);		// enable/disable non-scanning keys
        QGridLayout     *PKGrid;	// main grid
	QHBoxLayout	*PKChMgrHBL;	// Horiz box layout for +/- buttons
	QHBoxLayout	*PKChBrwHBL;	// channel browser buttons </>
	QHBoxLayout	*PKFrStpHBL;	// frequency stepper buttons

	QTimer		*PKScrollTimer;
	QTimer		*PKQueryTimer;

	QLineEdit	*PKDirEntLE; 	// direct entry line edit
	QString		PKTemp;

        QPushButton     *PKEN; 		// enter
        QPushButton     *PKDT; 		// dot
	QPushButton	*PKMP; 		// add channel
	QPushButton	*PKMM; 		// remove channel
	QPushButton	*PKSU;		// step up one 
	QPushButton	*PKSD;		// step down one
	QPushButton	*PKScanTB;	// scan toggle button

        QPushButton     *PK01;
        QPushButton     *PK02;
        QPushButton     *PK03;
        QPushButton     *PK04;
        QPushButton     *PK05;
        QPushButton     *PK06;
        QPushButton     *PK07;
        QPushButton     *PK08;
        QPushButton     *PK09;
        QPushButton     *PK00;
        QPushButton     *PKST; // lower one channel
        QPushButton     *PKPD; // higher one channel
	QPushButton	*PKCR;

};

#endif /* PKEYBD_H_ */
