# qpcr1k
ICOM c/c++ low level API, CLI and GUI for controlling the serial based PCR1000

NOTE: Additional documentation and ``whitepapers'' are located in the doc directory off of the root source directory. Also HTML documentation for the API can be located in the html directory under the doc directory.

In an effort to reduce our work we've work and blame spreaded.

The first of which is the PCR-1000 Control Suite. Based upon a newly written C++ API, the control suite consists of both a set of command line utilities and a GUI written using the QT 2.02 Widget Set. 

Keep in mind that code development, from the acquisition of the PCR-1000 itself to release, has been a little less than 5 months. Please tell us what you want to see in the next release, which will be coming hopefully in two weeks. High on the priority list is DTMF decoding and scanner functionality for the GUI. Additionally, the user will see support for multiple PCR-1000's enhanced, plus memory channel features in the GUI. Eventually we hope to have the bandscope supported in the GUI, and possibly a ncurses-based console app. 

Additionally, if you do not like the fact that the code is written using the QT widget set, get bent. If you want to write something using the GTK or Motif or whatever, feel free. The API hooks are in src/pcp/pcp.h. bin/console/pcr and bin/console/scanner provide some basic examples of how the API works. We haven't had a chance to write a document to fully detail how to code using the API yet, but hopefully we will write something of that sorts eventually. 
