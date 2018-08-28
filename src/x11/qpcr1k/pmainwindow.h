#ifndef PMAINWINDOW_H_
#define PMAINWINDOW_H_
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
#include <qmainwindow.h>

#include "pfileio.h"

#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

#define QPCR1K_VERSION "QP_V1001"

class QTimer;
class PCP;
class PUI;
class PMenu;
class PTools;
class PStatus;
class PFileIO;

class PMainWindow : public QMainWindow
{
	Q_OBJECT

	public:
	 	PMainWindow(QWidget *parent=0, const char *name=0);
	 	~PMainWindow();

	private slots:
   	 	void execRadioCommandSlot(const char *, const char*);
   	 	void scanSlot(const char);
	 	void updateStatusSlot();
		void updateScanStatSlot();
		void closeEvent(QCloseEvent *);

 	 	void PMAbout();
 	 	void PMNewProfile();
 	 	void PMLoadProfile(); 
	 	void PMSaveAsProfile(); 
	 	void PMSaveProfile(); 
	 	void PMQuit(); 
	 	void PMEditProfile(); 
//	 	void PMInit();
//	 	void PMPower();
		void PMMiniMode();

	private:
	 	bool	execRadioCommand(const char*, const char*);
	 	bool	writeProfile(QString, bool force=FALSE);
	 	bool	readProfile(QString, bool force=FALSE);
		const char *ranger(int value=0);

		PFileIO	*PMRcFile;
		FILE * __chk_fd;
		bool __first_time;
	 	qpcrRc	PMRc;
	 	QString	*fileName;
	
	 	char	PMTemp[256];
	 	char	PMScFreq[32];
	
		bool	PMClosing;	/* are we being closed? */
	 	bool	PMStatus;      /* radio on/off (true/false) */ 
	 	PCP	*PMRadio;       /* PCR1k Communication Device */ 
	 	PMenu	*PMMenuBar;     /* Main Menu Bar */ 
	 	PTools	*PMToolBar;     /* Main Tool Bar */ 
	 	PUI	*PMInterface;   /* User Interface Widget */ 
	 	PStatus	*PMStatusBar;   /* Main Status Bar */ 

	 	QTimer	*PMStatTimer;   /* Status Bar Timer */
		QTimer	*PMScanTimer;	/* Sanner timer */
		QTimer	*PMScWaitTimer;	/* Scanner linger timer */
		bool	PMSigOk;	/* is it ok to query for Sig strength */
		char	PMScanMode;	/* how are we scanning, NULL if not */
};

#endif /* PMAINWINDOW_H_ */
