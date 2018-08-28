#ifndef PSTATUS_H_
#define PSTATUS_H_
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
#include <qstatusbar.h>
#ifdef DEBUG_VER_
#include <stdio.h>
#endif // DEBUG_VER_

class QString;
class QLabel;

class PStatus : public QStatusBar
{
    Q_OBJECT

    public:
        PStatus(QWidget *parent=0, const char *name=0);
        ~PStatus();

    public slots:
        void setDevice  (QString &);
        void setPower   (bool);
        void setBaudRate(QString &);
        void setComm    (QString &);
/*
    // There are no signals in this class.
    signals:

*/
    private:
        QLabel  *PSMDevice;     // "/dev/?"/"..."
        QLabel  *PSMPower;      // ON/OFF/UNSET
        QLabel  *PSMBaudRate;   // Baud
        QLabel  *PSMComm;       // TX/RX/IDLE

};

#endif /* PSTATUS_H_ */
