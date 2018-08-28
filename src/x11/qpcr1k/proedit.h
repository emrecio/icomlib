#ifndef PROEDIT_H_
#define PROEDIT_H_
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

#include <qdialog.h>
#include <termios.h>

#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class QString;
struct qpcrRc;

class ProEdit : public QDialog
{
    Q_OBJECT

    public:
        ProEdit(QString *, qpcrRc *, int, QWidget *parent=0, 
		const char *name=0, bool modal=FALSE);
        ~ProEdit();

/*  
    signals:
        // corresponding signals
 */

    private slots:
	void PECommitSlot();

    private:
	void PEConvFreq(QString *, bool);
	char *PEConvSpeed(tcflag_t);
	tcflag_t PEConvSpeed(const char*);

	qpcrRc		*origRc;
	int		 currMode;
	int		 __currCh;
	tcflag_t	 tempSpeed_t;
	char 		 tempSpeed_Str[16];

	QGridLayout	*PEMainGL;

	/* for case 0 */
	QLabel		*PEChannelLbl;
	QLineEdit	*PEChannelLE;
	QLabel		*PEFreqLbl;
	QLineEdit	*PEFreqLE;
	QLabel		*PEModeLbl;
	QLineEdit	*PEModeLE;
	QLabel		*PEFilterLbl;
	QLineEdit	*PEFilterLE;
	QLabel		*PEToneSqLbl;
	QLineEdit	*PEToneSqLE;

	/* for case 1 */
	QLabel		*PEDeviceLbl;
	QLineEdit	*PEDeviceLE;

	/* for case 2 */
	QLabel		*PESpeedLbl;
	QLineEdit	*PESpeedLE;

	QPushButton	*PECommitBtn;
	QPushButton	*PERollBkBtn;

	QString		*tempString;

};

#endif /* PROEDIT_H_ */
