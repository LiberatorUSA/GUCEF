*
*  CMakeListGenerator
*  Copyright (C) 2002 - 2010.  Dinand Vanvelzen
*
*  This software is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This software is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
*


About:
------------------------

The CMakeListGenerator tool is meant for automatically generating the bulk 
of the CMakeLists.txt content based on directory structure and file analysis.
This reduces maintenance time for CMake files and makes it easy to generate

CMake files for 3'rd party sources with little effort.
I, Dinand Vanvelzen, created this tool to help me build the Galaxy Unlimited
codebase using CMake without having to manually write all the CMake files for
GU libraries and their dependencies.


Features:
------------------------

- Automatically locate and list all header and source file for each module
- Ability to exclude directories from processing
- Automatically generates the include paths for each module's dependencies
- Handles most include/source directory structures
- Special processing for platform specific headers/sources is supported.



Usage: Overview
------------------------

There are 3 kinds of text files involved in the generation of the CMakeLists.txt 
files, these are:
- CMakeLists.txt:
        Module file which is automatically generated
- CMakeListsSuffix.txt: 
        Like the name says, a suffix that will be added to the automatically 
        generated CMakeLists.txt
- CMakeGenExcludeList.txt:
        A simple text file containing a sub-dir name per line that should be 
        excluded from CMakeListGenerator processing

Note that if a CMakeLists.txt file already exists the tool will overwrite it!


Usage: Command line parameters
------------------------

If no command line arguments are given then the tool will start processing 
the directory structure using the current working directory as the root.

The tool accepts the following command line parameters:
- 'rootDir=<path to processing root>'
    Example:
       'rootDir=C:\Dev\MyCodeRoot'

Like most GU applications command line parameters accepted in the form:
'<key>=<value>'


Usage: Platform (O/S) specific processing
------------------------

At present this feature is only supported for modules where the project directory
has a 'include' dir for includes and a 'src' dir for source. Each of which have 
sub-dirs for each platform.

Example:
  include\mswin\MyWindowsSpecificCode.h
  src\mswin\MyWindowsSpecificCode.cpp

For the example file structure above a special if clause will be generated where
depending on the target platform PLATFORM_HEADER_FILES and PLATFORM_SOURCE_FILES
will be set to the correct list of files.

Supported directories:
"mswin" for CMake platform "WIN32"
"linux" for CMake platform "UNIX"


History:
------------------------

- 12'th Jan 2010: First release