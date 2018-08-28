#ifndef PCTRL_H_
#define PCTRL_H_
/*
 *   QPcr1k 
 *   Copyright (C) 1999,2000 by PolyWog and Javaman for Ghetto.Org
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
#include <assert.h>
#endif // DEBUG_VER_

class QLabel;
class QHBoxLayout;
class QComboBox;
class QCheckBox;
class QButtonGroup;
class QRadioButton;
class QListBox;
class QSlider;
class QVBoxLayout;
class QStrList;
class QLCDNumber;
class QGroupBox;

class PControl : public QWidget
{
    Q_OBJECT

    public:
        PControl(QWidget *parent=0, const char *name=0);
        ~PControl();

    public slots:
        // this slot is to change the initial value 
	// for each widget located in the control block	
	void valueChangeSlot(const char*, const char*);

    signals:
        // corresponding signals to send out of the object
	void valueChangeCall(const char*, const char*);

    private slots:
	// these are slots that will be used internally
	// by the individual widget(s)
	void volSliderSlot(int);
	void sqlSliderSlot(int);
	void fltComboBoxSlot(const QString &);
	void modComboBoxSlot(const QString &);
	void tsqComboBoxSlot(const QString &);
	void agcChkBxSlot();
	void nblankChkBxSlot();
	void rfattnChkBxSlot();
	void pcStpComboBoxSlot(const QString &);
	void pcStpBtnGrpSlot(int);

    private:
	void 		pcStpBtnGrpSelect(const char*);
	char		PCTemp[128];

	QStrList	*PCModList;
	QStrList	*PCFltList;
	QStrList	*PCTSqList;

	QStrList	*PCCurLOV;
	QStrList	*PCMHzLOV;
	QStrList	*PCkHzLOV;
	QStrList	*PCHzLOV;

	QGroupBox	*PCVolGrpBx;
	QGroupBox	*PCSqlGrpBx;
	QGroupBox	*PCStpGrpBx;
	
	QHBoxLayout	*PCMainLayout;
	QHBoxLayout	*PCModLayout;
	QHBoxLayout	*PCFltLayout;
	QHBoxLayout	*PCTSqLayout;
	QVBoxLayout	*PCRightLayout;

	QLCDNumber	*PCVolLcd;
	QLCDNumber	*PCSqlLcd;
	QLabel		*PCFltLabel;
	QLabel		*PCModLabel;
	QLabel		*PCTSqLabel;

	QSlider		*PCVolSlider;
	QSlider		*PCSqlSlider;
	QComboBox	*PCFltComboBox;
	QComboBox	*PCModComboBox;
	QListBox	*PCTSqListBox;

	QCheckBox	*PCAgcChkBx;
	QCheckBox 	*PCNBlankChkBx;
	QCheckBox	*PCRfAttnChkBx;

	QComboBox	*PCStpComboBox;
	QButtonGroup	*PCStpBtnGrp;
	QRadioButton	*PCMHzRadioBtn;
	int		PCMHzRadioBtnId;
	QRadioButton	*PCkHzRadioBtn;
	int		PCkHzRadioBtnId;
	QRadioButton	*PCHzRadioBtn;
	int		PCHzRadioBtnId;
	
};

#endif /* PCTRL_H_ */
