# NVObjTemplate

This project was started so that a build tree and example library could be created that showed how to setup cross-platform Omnis externals with support for a version control system (such as Subversion, Git or Mercurial) and an automated build system.

If this example is used on a Mac, then a project can be simultaneously used by XCode and Visual Studio 2008 (in Parallels or VMWare) against the same set of source files (Important so development can occur in a single development environment, but builds can be made on all platforms).  Only additional source files need to be added to each project when changes occur.  Linux support is not available at this time.

It is required to download the SDK from TigerLogic in order to setup the build environment variables required for this project: 
https://www.omnis.net/products/components/buildyourown.jsp

# Description of files

| Files | Description |
| ----- | ----------- |
| ../include/NVObjTemplate.he<br> ../src/NVObjTemplate.cpp | Main entry point for Omnis external.  These files control the dispatch of messages to appropriate non-visual objects and the definition of non-visual objects. |
| ../include/NVObjTemplate.he<br> ../src/NVObjTemplate.cpp | Main entry point for Omnis external. These files control the dispatch of messages to appropriate non-visual objects and the definition of non-visual objects. |
| ../include/NVObjBase.he<br> ../src/NVObjBase.cpp | These files hold the base class, NVObjBase, to be used for all non-visual objects.  It provides basic methods for defining a non-visual object. |
| ../include/OmnisTools.he<br> ../src/OmnisTools.cpp | These files hold a series of helper methods that I've found useful while creating components. |
| ../include/Logging.he | Simple std::cout console logger |
| ../include/ThreadTimer.he<br> ../src/ThreadTimer.cpp | Pub/Sub Timer that allows workers to interrupt Omnis Studio |
| ../include/Worker.h<br> ../src/Worker.cpp | Worker implementation for background thread processing |
| ../include/Simple.he<br> ../src/Simple.cpp | These files hold an example implementation of a non-visual object, Simple.  Simple has 1 property and 2 methods.  It also implements the full worker model for $initialize, $start, $run, and $cancel with a simple worker delegate. |
| ../include/Static.he<br> ../src/Static.cpp | These files hold an example implementation of static methods.  There is 1 static method, $staticEmpty(). |
| ../src/NVObjTemplate.rc | This file holds the "resources" -- string table for method, property, and constant descriptions -- that Omnis uses for the component.  (Resource 32000 is the name of the entry point method in NVObjTemplate.cpp) |
| ../src/NVObjTemplate.def | This file holds the entry point name for Windows components (Should match resource 32000 in NVObjTemplate.rc) |

# Usage and Installation
## Build tree

Windows and Mac projects are configured to look at directories within the MyProjects" folder, so no additional work should be required for compiling.  It's better to avoid spaces in paths as the Omnis build system doesn't properly recognize them.

## Omnis External Dependencies:
See "Read Before Compiling.txt" for instructions on setting up the OMNIS_LIB_PATH environment variablem

| Variable | Description |
| - | - |
| $OMNIS_LIB_PATH/Mac | Omnis external component dependencies for Mac (complib and frameworks folders) |
| $(OMNIS_LIB_PATH)/Windows | Omnis external component dependencies for Windows (COMPLIB, EXTLIB, and ULIBS folders) |

### Source Code Locations

| Folder | Description |
| - | - |
| include | Header fields for component NVObjTemplate |
| src | Source files for component NVObjTemplate |
| proj | Platform dependant project files |


### Project Locations:
| Folder | Description |
| - | - |
| proj | Platform dependant project files |

## Build output
Debug / Development output puts the component in the Omnis Studio xcomp folder (See Omnis directory). Release output puts the component in the deploy folder.

| Folder | Description |
| - | - |
| deploy/Mac/ | Mac component |
| deploy\Windows\ | Windows component |

## Omnis directory

The Omnis directory must be setup prior to compiling any debug versions of the external.  It is read from an Environment Variable, OMNIS_XCOMP_PATH.

For Windows this is like: C:\TigerLogic\OS50\xcomp
- Set under System Properties (Access by Right-Clicking My Computer and then selecting Properties)
- Requires close and re-open of Visual Studio

For Mac this is like: HD:Applications:OmnisStudio:Contents:MacOS:xcomp
- Set in the environment.plist file in the user home directory ~/.MacOSX/environment.plist 
- Requires log off of OS X

## Customization 
(How to change name from NVObjTemplate)

Obviously the purpose of any template is to get you started with creating your own components.  While 
renaming the classes can be a delicate process these are the things that I've found to be a good starting place.

### All
- Since many files are being renamed they may need to be re-added to the project.  Especially if you rename on Mac and then open Windows project or vice-versa.

### MacOs
1. Rename the NVObjTemplate.xcodeproj project file (while the project is closed)
1. Rename NVObjTemplate.he and NVObjTemplate.cpp
1. Perform a global find/replace for NVObjTemplate
1. Perform a global find/replace for NV_OBJ_TEMPLATE_HE
1. Rename NVObjTemplate.rc
1. Rename NVObjTemplate.def

### Windows
1. In Visual Studio, right-click on Solution and rename
1. In Visual Studio, right-click on Project and rename
1. Rename NVObjTemplate.he and NVObjTemplate.cpp
1. Perform a global find/replace for NVObjTemplate
1. Perform a global find/replace for NV_OBJ_TEMPLATE_HE
1. Rename NVObjTemplate.rc 
1. Rename NVObjTemplate.def

# Copyrights 
Copyright (c) 2010 David McKeone
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
1. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
1. The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission.

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