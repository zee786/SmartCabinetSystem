========================================================================
    CONSOLE APPLICATION : simplecab Project Overview
========================================================================

AppWizard has created this simplecab application for you.

This file contains a summary of what you will find in each of the files that
make up your simplecab application.


simplecab.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

simplecab.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

simplecab.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named simplecab.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
RFIDTag.cpp

Nishanth logic works only with empty cabinets
If I put items Antenna doesn't read door tags
It is difficult to check the door stauts
Now I am trying with full ID (Antenna ID + UID)

05/07/2014
Tags with items blocking each other. I changed the position of the antenaas then door tags were read , but still all
the tags unable to read. 
08/07/2014
got the solution, now I can read all the tags.I am reading data now one by one. 1 tag=19 bytes and I am reading 1 tag at a time. 
10/07/2014
I can read all the tags, able to show image one shelf at a time. I did not check the highlight option but it should work easily.
Current problem is when user pickup the item and put it on the table then projection should be stop on that shelf. 
11/07/2014
ALL 4 antennas are working but only projection feature is working. 
2 more features are left
1. Highlight
2. When user picks an item from a particular shelf and put on the table stop projection there 
15/07/2014
1. Still unable to find the solution about my user picks the itme and put on the table
----
About RFID Device
--If run more than 2 Antenna make change in EEPROMConf 4th Option
--Increase the number
TODO:
I will change the locations of the antennas because RFID company(TR3 Manager) software read all the tags.