#ifndef PRODLG_H_
#define PRODLG_H_
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

class QLabel;
class QLineEdit;
class QListView;
class QListViewItem;
class QGroupBox;
class QSplitter;
class QGridLayout;
struct qpcrRc;



#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

class ProDlg : public QDialog
{
    Q_OBJECT

    public:
        ProDlg(qpcrRc *, QWidget *parent=0, 
		const char *name=0, bool modalness=TRUE);
        ~ProDlg();

/*
    public slots:
        // File 
*/
    private slots:
        // wooooooooy
        void PDlSelectionChangedSlot( QListViewItem * );
	void PDlDoubleClickedSlot( QListViewItem * );
        void PDlCommitPBSlot();
        void PDlRollbackPBSlot();

    signals:
        // corresponding signals

    private:
	void PDlPopulate();
        qpcrRc      *tempRc;
	qpcrRc      *origRc;

        QGridLayout *PDlMainGL;

        QGroupBox   *PDlEditGB;
        QLabel      *PDlNameLabel;

        QPushButton *PDlCommitPB;
        QPushButton *PDlRollbackPB;

        QSplitter   *PDlMainSplit;
        QListView   *PDlOptsLV;
        QListViewItem   *PDlChannelsLVI[41];
        QListViewItem   *PDlGlobalsLVI[8];


};

#endif /* PRODLG_H_ */
