# NVObjTemplate

## Getting started
These steps are required before compilation (setup of Omnis API)

1. Ensure that you have the appropriate development environment installed<br>
Windows = Visual Studio 2008<br>
Mac = XCode 3.2+  (XCode 4.1 on OS X 10.6 preferred)
1. Ensure that you are using Omnis Studio 5+ (This component is Unicode only)
    1. (Mac Only) Ensure that the application path bundle name has no spaces.  e.g. ```/Applications/OmnisStudio5.app```
    1. (Windows Only) Ensure that the path to the omnis.exe does not have any spaces (So don't put in "Program Files").  e.g ```C:\TigerLogic\OS50\omnis.exe```
1. (Mac only) Ensure that omnisrc.app has been copied (or symbolically linked) into /Developer/Tools.
1. Ensure that this directory is contained in folders with no spaces. e.g <br>
Mac = ```/Externals/MyProjects```<br>
Windows = ```C:\Externals\MyProjects```
1. Ensure that you set the Installation directory for the component and the path used for omnis libraries. See below for OS specific instructions.

### Windows 
1. Right-click on My Computer
1. Click 'Advanced' tab or 'Advanced System Settings'
1. Click 'Environment Variables'
1. Under 'System Variables' click 'New'
1. Variable name = OMNIS_XCOMP_PATH
1. Variable value is the path to the xcomp directory in the Omnis directory<br>
NOTE: There can be no spaces in the path (See Step 2)
e.g ```C:\TigerLogic\OS501\xcomp```
1. Under 'System Variables' click 'New'
1. Variable name = OMNIS_LIB_PATH
1. Variable value is the path to the Omnis library dependencies. The folder specified should contain the COMPLIB, EXTLIB and ULIBS folders from the API.<br>
NOTE:   There can be no spaces in the path (See Step 2) <br>
e.g ```C:\Externals\OmnisDependencies\Windows```
1. If Visual Studio is open, close and re-open it.

### MacOs
1. Select 'Go' menu in Finder
1. Select 'Go to Folder'
1. Type ```~/.MacOSX/```<br>
    NOTE: Create the .MacOSX folder if it doesn't exist
1. Open environment.plist <br>
    NOTE: If it doesn't exist then it can be created using the Property List Editor application in ```/Developer/Applications/Utilities```
1. Click the 'Root' dictionary line
1. Click the 'Add Child' toolbar button
1. Key = OMNIS_XCOMP_PATH
1. Type = String
1. Value = Path to the xcomp directory in the Omnis application package<br>
  NOTE: There can be no spaces in the path (See Step 2)<br>
  e.g ```/Applications/OmnisStudio5.app/Contents/MacOS/xcomp```
1. Click the 'Root' dictionary line
1. Click the 'Add Child' toolbar button
1. Key = OMNIS_LIB_PATH
1. Type = String
1. Value = Path to the Omnis library dependencies.  
  The folder specified should contain the compile and frameworks folders from the API.<br>
  NOTE: There can be no spaces in the path (See Step 2)<br>
  e.g ```\Externals\OmnisDependencies\Mac```
1. Log off of OS X and log back in 


## BOOST LIBRARIES
5.a. Some projects may also require the Boost libraries, this requires downloading Boost(http://www.boost.org) extracting it to a directory and setting up the $BOOST_ROOT environment variable.  (Boost Getting Started: http://www.boost.org/doc/libs/release/more/getting_started/index.html)

Note that Boost 1.49.00 is the last compatible version with GCC 4.2 and as of this writing (Omnis Studio 6) that means it is the latest version that can be used with Omnis Externals.

### Windows
1. Right-click on My Computer
1. Click 'Advanced' tab or 'Advanced System Settings'
1. Click 'Environment Variables'
1. Under 'System Variables' click 'New'
1. Variable name = BOOST_ROOT
1. Variable value is the path to the Boost directory<br>
NOTE:  There can be no spaces in the path (See Step 2)<br>
e.g C:\Boost\boost_1_49_0  (For Version 1.49, the current version at time of writing)
1. If Visual Studio is open, close and re-open it.


### MacOs  
1. Select 'Go' menu in Finder
1. Select 'Go to Folder'
1. Type ~/.MacOSX/
NOTE: Create the .MacOSX folder if it doesn't exist
1. Open environment.plist <br>
NOTE: If it doesn't exist then it can be created using the Property List Editor application in /Developer/Applications/Utilities
1. Click the 'Root' dictionary line
1. Click the 'Add Child' toolbar button
1. Key = BOOST_ROOT
1. Type = String
1. Value = Path to the Boost directory<br>
NOTE: There can be no spaces in the path (See Step 2)<br>
e.g /Boost/boost_1_49_0  (For Version 1.49, the current version at time of writing)
1. Log off of OS X and log back in 

5.b. Some projects may also require Boost libraries to be built. To build boost libraries you must go to the boost directory and execute bjam commands or install the libraries from an installer.  The commands are to be executed from the same directory as $BOOST_ROOT from 5a.  NOTE: All projects assume this builds libraries into $BOOST_ROOT/stage/lib
     
### Windows
(Requires a special flag to comply with Omnis' build requirements on Windows /Zc-wchar_t-  -- This is the 'Treat wchar_t as built-in type = NO' option in Visual Studio 2008)
```
Complete:
bjam -a --layout=versioned --build-type=complete cxxflags=-Zc:wchar_t-

Regular:
bjam -a --layout=versioned cxxflags=-Zc:wchar_t-

Complete - Static:
bjam -a --layout=versioned --build-type=complete link=static cxxflags=-Zc:wchar_t-

Regular - Static:
bjam -a --layout=versioned link=static cxxflags=-Zc:wchar_t-
```

### MacOs
(Requires special flags for building a universal binary that supports i386, x86_64, ppc and ppc64 
```
Complete:
./bjam -a --layout=versioned --build-type=complete toolset=darwin  architecture=combined address-model=32_64 macosx-version=10.5 macosx-version-min=10.4

Regular:
./bjam -a --layout=versioned toolset=darwin architecture=combined address-model=32_64 macosx-version=10.5 macosx-version-min=10.4

Complete - Static:
./bjam -a --layout=versioned --build-type=complete toolset=darwin  link=static architecture=combined address-model=32_64 macosx-version=10.5 macosx-version-min=10.4

Regular - Static:
./bjam -a --layout=versioned toolset=darwin link=static architecture=combined address-model=32_64 macosx-version=10.5 macosx-version-min=10.4
```

## 6. Setting Build and Run
Ensure that the Omnis executable has been set so 'Build and Run' can be used

### Windows
1. Open Visual Studio 2008 project NVObjTemplate.vcprog
1. Go to 'Debug' menu
1. Select 'Start Debugging'
1. A window will appear asking to find the executable
1. Enter the executable path
1. Click OK
1. A window may appear saying that the executable does not have debug information - that is ok, dismiss the message.

### Mac
1. Open XCode project NVObjTemplate.xcodeproj
1. Locate "Omnis" under "Executables"
1. Right-Click on "Omnis" and select "Get Info"
1. Change "Path" to the path of the Omnis Studio application bundle
    e.g ```/Applications/OmnisStudio5.app```

## 7. Attempt to compile the project
     
### Windows
NOTE -- If you did step 6 then the project has already been compiled and Omnis opened, but the steps are here for completeness.

1. Open Visual Studio 2008 project NVObjTemplate.vcprog
1. Go to 'Build' menu -or- Go to 'Debug' menu
1. Select 'Build Solution' -or- Select 'Start Debugging' to compile component and open Omnis

### Mac
1. Open XCode project NVObjTemplate.xcodeproj
1. Go to 'Build' menu
1. Select 'Build Results' -or- 'Build and Debug' to compile component and open Omnis

## Copyrights
Copyright (c) 2013 David McKeone
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.