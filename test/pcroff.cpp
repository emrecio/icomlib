/*
   Copyright (C) 1999 PolyWog and Javaman for Ghetto.Org
   This file is the command line component of the PCR1000 Control Suite.

   The PCR-1000 Control Suite is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The PCR-1000 Control Suite is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the PCR-1000 API Library; see the file LICENSE.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
 */

/* 
 * This file declars the PCR-1000 command line interface for the
 * Ghetto.org radio toolkit.  It implements as much of the functionality
 * as possible as defined in the API
 *
 */

#include <pcp.h>
#include <stdio.h>

int main (int argc, char **argv)
{
	// This code turns off the radio.  EOF
	//
	PCP *rig = new PCP (PCRDEV, BAUDRATE, "RIG"); 

	rig->PCPPowerDown();

	if(!rig->PCPQueryOn()) {
		printf("Radio powered down.\n");
	}

	return 0;
}
