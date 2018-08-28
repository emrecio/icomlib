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

#include <qstring.h>
#include <qlabel.h>

#include "pstatus.h"

PStatus :: PStatus (QWidget *parent, const char *name)
         : QStatusBar(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: Creating...\n");
    #endif // DEBUG_VER_

    // create some labels and place them in the status bar
    PSMBaudRate = new QLabel (this, "psmbaudrate_label");
    addWidget (PSMBaudRate, 15);
    PSMBaudRate->setText("9600");

    PSMComm     = new QLabel (this, "psmcomm_label"    );
    addWidget (PSMComm,     10);
    PSMComm->setText("IDL");

    PSMPower    = new QLabel (this, "psmpower_label"   );
    addWidget (PSMPower,    25);
    PSMPower->setText("UNK");

    PSMDevice   = new QLabel (this, "psmdevice_label"  );
    addWidget (PSMDevice,   50);
    PSMDevice->setText("/dev/?");

    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: Created!\n");
    #endif // DEBUG_VER_
}

PStatus :: ~PStatus()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: Destroyed!");
    #endif // DEBUG_VER_
}

void PStatus :: setDevice ( QString &text )
// sets the active text for this particular message
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: SLOT-SETDEVICE\n");
    #endif // DEBUG_VER_

    PSMDevice->setText(text);
}

void PStatus :: setPower (bool status)
// sets the active text for this particular message
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: SLOT-SETPOWER\n");
    fflush(stderr);
    #endif // DEBUG_VER_

    if (status) {
        PSMPower->setText("On");
    } else {
        PSMPower->setText("Off");
    }
}

void PStatus :: setBaudRate(QString &text)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: SLOT-SETBAUDRATE\n");
    fflush(stderr);
    #endif // DEBUG_VER_

    PSMBaudRate->setText(text);
}

void PStatus :: setComm(QString &text)
{
    #ifdef DEBUG_VER_2
    fprintf(stderr, "PStatus: SLOT-SETCOMM\n");
    #endif // DEBUG_VER_

    PSMComm->setText(text);
}
