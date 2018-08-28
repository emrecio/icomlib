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

#include "background.xpm"
#include "pstatus.h"

PStatus :: PStatus (QWidget *parent, const char *name)
         : QStatusBar(parent, name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PStatus: Creating...\n");
    #endif // DEBUG_VER_

    setBackgroundMode( QWidget::FixedPixmap );
    QPixmap bg( background_xpm );
    setBackgroundPixmap( bg );


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
