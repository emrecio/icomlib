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

#include <stdlib.h>
#include <unistd.h>

#if defined(SunOS) || defined(Irix)
#include <strings.h>
#else
#include <string.h>
#endif


#include "pcp.h"
#include "pfileio.h"

PFileIO :: PFileIO (const char *name)
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PFileIO: Creating...\n");
    #endif // DEBUG_VER_

    // let's start by getting the home directory, current dir
    // and making a temporary file for ourselves.

    // first we will get the home directory, then cat a
    // '/' to the end of that. This will be our `basename'
    // for the _file_
    bzero(home_dir, sizeof(home_dir));
    strncpy(home_dir, getenv("HOME"), sizeof(home_dir)-1);
    strncat(home_dir, "/", ((sizeof(home_dir))-(strlen(home_dir))-1));

    bzero(curr_dir, sizeof(curr_dir));
    getcwd(curr_dir, sizeof(curr_dir)-1);

    bzero(tmp_filename, sizeof(tmp_filename));
    strncpy(tmp_filename, tmpnam(NULL), sizeof(tmp_filename)-1);

    tmp_file = fopen(tmp_filename, "w");
    // now we unlink the file, so we dont have to worry about it later
    // the kernel automagically deletes it when the process is finished
    if (unlink(tmp_filename)<0) {
        perror("unlinking temporary file");
    }

    // do nothing with our internal name for now
    name=0;

    #ifdef DEBUG_VER_
    fprintf(stderr, "PFileIO: Created!\n");
    #endif // DEBUG_VER_
}

PFileIO :: ~PFileIO()
{
    #ifdef DEBUG_VER_
    fprintf(stderr, "PFileIO: Destroyed!\n");
    #endif // DEBUG_VER_
}

bool PFileIO :: PFOpen(const char *fname, const char *mode, bool force)
//
// open the file _fname_ for reading and writing. If no filename
// is given, then the default .pcprc file is opened. It is highly
// suggested that you write your applications such that this rc
// file created is called ``.your_application_namerc'' This way
// there will be no conflicts between this file and any other
// applications the user may install using this library.
//
// _force_ means that the filename will NOT have the user's home
// directory prefixed in the filename. In other words, if _force_ is true
// then you are forcing the file name fed in here to be used.
//
{
#ifdef DEBUG_VER_
    fprintf(stderr, "PFileIO: PFOpen(\"%s\");\n", fname);
#endif // DEBUG_VER_

    bzero(filename, sizeof(filename));

	if (fname==NULL){
#ifdef DEBUG_VER_
		fprintf(stderr, "PFILEIO: NULL Rc File using defaults");
#endif // DEBUG_VER_
        	strncpy(filename, STDRCFILE, sizeof(filename)-1);
	} else {
		strncpy(filename, fname, sizeof(filename)-1);
	}
	bzero(rc_filepath, sizeof(rc_filepath));
	if ( !force ) {
		strncpy(rc_filepath, home_dir, sizeof(rc_filepath)-1);
		strncat(rc_filepath, filename, ((sizeof(rc_filepath))-(strlen(rc_filepath))-1));
	} else {
		strncpy(rc_filepath, filename, sizeof(rc_filepath)-1);
	}
#ifdef DEBUG_VER_
    fprintf(stderr, "PFileIO: PFOpen(\"%s\"): rc_filepath = %s\n", filename, rc_filepath);
#endif // DEBUG_VER_

    if ( (rc_file=fopen(rc_filepath,mode)) == NULL ) {
        return false;
    }

    return true;

}

bool PFileIO :: PFClose()
// closes open files.
{
    fclose(rc_file);
    fclose(tmp_file);
    return true;
}

bool PFileIO :: PFRead(char *line)
// reads the file and returns the contents of one line
{
    fgets(line, sizeof(line)-1, rc_file);
    return true;
}

bool PFileIO :: PFWrite(const char *line)
// writes to the file
{
    char *templine = new char [strlen(line)+5];
    strcpy(templine, line);
    strcat(templine, "\n");

    uint __put_ctr=0;
    __put_ctr = fputs(templine, rc_file);

    if ( __put_ctr != (strlen(templine)+1) ) {
        return false;
    }

    return true;
}

bool PFileIO :: PFWrite(void *ptr, size_t ptr_size)
// writes contents of *ptr to file
{
    int membw = fwrite(ptr, ptr_size, 1, rc_file);
    if (membw != 1) {
        return false;
    }

    return true;
}

bool PFileIO :: PFRead(void *ptr, size_t ptr_size)
// reads contents of file into *ptr
{
    if (ptr==NULL) {
        ptr = malloc(ptr_size);
    }

    int membr = fread(ptr, ptr_size, 1, rc_file);
    if (membr != 1) {
        return false;
    }

    return true;
}
