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
#include <qpixmap.h>


#ifdef WINDOWS_STYLE
#include <qwindowsstyle.h>
#endif
#ifdef MOTIF_STYLE
#include <qmotifstyle.h>
#endif
#ifdef SGI_STYLE
#include <qplatinumstyle.h>
#endif
#ifdef CDE_STYLE
#include <qcdestyle.h>
#endif

#include "pmainwindow.h"
#include "appicon1.xpm"

#ifdef DEBUG_VER_
#include <stdio.h>
#endif DEBUG_VER_

int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
#if defined (WINDOWS_STYLE)
    a.setStyle( new QWindowsStyle ); 
#endif
#if defined (SGI_STYLE)
    a.setStyle( new QPlatinumStyle );
#endif
#if defined (MOTIF_STYLE)
    a.setStyle( new QMotifStyle );
#endif
#if defined (CDE_STYLE)
     a.setStyle( new QCDEStyle );  
#endif
    PMainWindow * mw = new PMainWindow();
    QPixmap appIcon( appicon1_xpm );
    mw->setIcon( appIcon );
    mw->setCaption( "QPcr1k" );
    mw->resize(418,418);
    a.setMainWidget(mw);
    mw->show();
    return a.exec();
}
