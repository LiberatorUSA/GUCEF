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

----------------------------------------------
About:
----------------------------------------------

The CMakeListGenerator tool is meant for automatically generating the bulk 
of the CMakeLists.txt content based on directory structure and file analysis.
This reduces maintenance time for CMake files and makes it easy to generate
CMake files for 3'rd party sources with little effort.
I, Dinand Vanvelzen, created this tool to help me build the Galaxy Unlimited
codebase using CMake without having to manually write all the CMake files for
GU libraries and their dependencies. One of the advantages is being able 
to dump new code releases of dependencies into the archive without having to
go through the code to check which files got added/deleted etc. Just re-run the
generator and you will have up-to-date CMakeList.txt files for those projects.
An additional advantage is the automatic generation of include_directories paths
which can be a pain to get right, especially with a lot of modules in a complicated
directory structure. Using this tool you can just move the modules around and their
include paths will be correct again as soon as you re-run the generator.



----------------------------------------------
Version:
----------------------------------------------

This is a release build created on 26'rd of June 2010.
It is a 32-bit Linux application for the x86 platform.
This is the first Linux release
if you encounter any problems let me know.


----------------------------------------------
Features:
----------------------------------------------

- Automatically locate and list all header and source file for each module
- Ability to manipulate what is excluded/included using processing instructions
- Automatically generates the include paths for each module's dependencies
- Handles most include/source directory structures
- Special processing for platform specific headers/sources is supported.
- Support for combined processing of multipe source trees
- Generate XML project file with all gathered/generated information.



----------------------------------------------
Usage: Overview
----------------------------------------------

There are 4 kinds of files involved in the generation of the CMakeLists.txt 
files, these are:
- CMakeLists.txt:
        Module (project) file which is automatically generated.
        Note that if a CMakeLists.txt file already exists the tool will overwrite it!
- CMakeListsSuffix.txt: 
        Like the name says, a suffix that will be added to the automatically 
        generated CMakeLists.txt. It is also used for dependency determination by the
        generator. Directories containing a suffix file will be considered project 
        directories. Includes in this file will be merged with includes determined by the tool.
- CMakeGenExcludeList.txt:
        A simple text file containing a sub-dir/file name per line that should be 
        excluded from CMakeListGenerator processing. This is also referred to as the 
        "Simple exclude list" because it is simple to specify but lacks some flexibity.
        If you to to include as well or need to do platform specific processing use the 
        xml based instruction file described below. For more details see the section 
        on processing instructions.
- CMakeGenInstructions.xml:
        Offers a more advanced method of changing how data will be processed by the
        generator. It allows you to specify which files/directories to include/exclude
        and for what platform. For more details see the section on processing instructions.

The combination of the latter three files produces the first, the actual CMakeLists.txt file
which CMake needs to generate your project/module files. The generator tool is meant to run as
part of your build system setup before running CMake. In combination the generator plus CMake
will allow you to automate almost every aspect of setting up your module/project files.



----------------------------------------------
Usage: Command line parameters
----------------------------------------------

If no command line arguments are given then the tool will start processing 
the directory structure using the current working directory as the root.
Like most GU applications command line parameters accepted in the form:
'<key>=<value>'

The tool accepts the following command line parameters:

- 'addToolCompileTimeToOutput=<true/false>'
     Determines whether to add the compile date-time of the tool to the header of all the CMake 
     files that are generated. This is particulary usefull when you are suspecting that your CMake
     files are not being generated by the correct version of the generator tool.
     The default is for this feature to be turned off.

- 'writeLogLocationToOutput=<true/false>'
     Determines whether the location of the tool logfile should be added at the end of the 
     generated CMake files. This is mainly usefull for debugging your build scripts.
     The default is for this feature to be turned off.

- 'rootDir=<path to processing root>'
    Example:
       'rootDir=C:\Dev\MyCodeRoot'

    Note that you can pass multiple instances of the rootDir argument in order
    to pass multiple root directories.

    Example:
       'rootDir=C:\Dev\MyCodeRoot' 'rootDir=D:\OtherDev\MyCodeRoot'

    Passing multiple root dirs will allow include dependencies between the two root dirs
    to be resolved thus allowing them to be used as if it's all part of the same project set.



----------------------------------------------
Usage: Platform (O/S) specific processing
----------------------------------------------

This feature allows you to include/exclude files automatically
by placing them in one of the supported platform directories. Include-directories for 
the module with platform specific code and every module that depends on it will have
its automatically generated include dirs altered based on the target platform, saving you
the effort of making all kinds of platform checks everywhere. Note that it is still 
recommended to surround your code with a preprocessor platform check, this way if your 
file gets included on the wrong platform it will not break the build!

Example:
  include\mswin\MyWindowsSpecificCode.h
  src\mswin\MyWindowsSpecificCode.cpp

For the example file structure above a special if clause will be generated where
depending on the target platform PLATFORM_HEADER_FILES and PLATFORM_SOURCE_FILES
will be set to the correct list of files. In this case the if clause will check
for platform WIN32. which directory names map to which platform is listed below.
You must name the directories according to one of the supported directory names
in order to make use of this feature to exclude/include files based on the target
platform.

Supported platform directories:
  "mswin" for platform "WIN32"
  "win32" for platform "WIN32"
  "win64" for platform "WIN64"
  "linux" for platform "UNIX"
  "osx" for platform "OSX"
  "mac" for platform "OSX"
  "android" for platform "ANDROID"
  "iphone" for platform "IPHONEOS"
  "glx" for platform "GLX"
  "gtK" for platform "GTK"
  "symbian" for platform "Symbian"
  "sdl" for platform "SDL"

Note that you can also influence/create platform specific processing by setting up
processing instructions for the generator. This is especially usefull for projetcs 
where platform specific files are in mixed in with portable files, making the sub-dir
usage for platforms, as described above, impossible. 



----------------------------------------------
Usage: Processing instructions
----------------------------------------------

As the generator processes directories and their files to determine what to include where
etc you might want a bit more control for deviant directory structures. If a project 
complies with the directory structure used for Galaxy Unlimited projects then no processing
instructions should ever be needed. However when using third-party code this is not always 
an option. This is where processing instructions come in.

As described in the overview there are 2 ways of specifying processing instructions:
  - Simple exclude list: this requires a file named "CMakeGenExcludeList.txt"
  - Advanced processing instructions: this requires a file named "CMakeGenInstructions.xml"
    In addition it also requires the "libparsifal.dll" XML library to be present and it's GUCEF
    tie-in plugin library which is "dstorepluginPARSIFALXML.dll"

The simple exclude list is, as you might guess, simple. As described in the overview it is simply
a list consisting of each item being placed on a different line. There is no way to specify whether
an item is a directory or a file, this will be automatically determined, nor can you specify includes
or anything platform specific. This seems very restrictive but in practice the simple exclude list 
serves most use cases. Typically you just want to exclude some test code or unrelated directories etc
for which the simple exclude list is sufficient.

If you cannot get the generator to process your module correctly using the exclude list then you
can utilize its more advanced sibling which is an XML file which allows you to specify more.
The XML file layout is shown in the example below:

<CMAKELISTGENERATOR>
  <EXCLUDES PLATFORM="ALL">
    <ITEM NAME="memunix.c" TYPE="FILE" />
    <ITEM NAME="Samples" TYPE="DIR" />
  </EXCLUDES>
  <INCLUDES PLATFORM="UNIX">
    <ITEM NAME="memunix.c" TYPE="FILE" />
  </INCLUDES>
</CMAKELISTGENERATOR>


There are 2 sections:
  - Excludes section: Here you specify what to exclude from processing
  - Include section: Here you specify what to include in processing, this is applied after the excludes
    section has been applied.


- Contrary to the simple exclude list you can optionally specify whether an item is a file or a directory.
  If this is not specified then it will be automatically determined the same as with the simple exclude list.
- Both the excludes and includes sections support an optional platform attribute which allows you to specify 
  for what platform the instructions apply. If the attibute is absent or specified as "ALL" then the section 
  will be applied for all platforms. If a platform is specified as shown in the example then this will result
  in the given file only being added for that platform via a if clause in the generated CMakeLists.txt file.
  It is merged with platform files detected using sub-dirs as specified in the "Platform specific processing"
  section.
- You can have as many inlude and exclude section as you want. Keep in mind that includes are always processed
  after all excludes have been processsed.
- Including alternate directories is not supported (...yet).
- If you wish to exclude every file/dir in the directory you can specify a wildcard exclude item as follows:
    <ITEM NAME="*" TYPE="FILE" />
  Keep in mind that if the type is not specified the wildcard will apply to both directories and files alike
  thus excluding all sub-directories and all files.
  Using this wildcard item allows you to manually specify what to include as a last resort if autodetection
  of the module setup is really not working for the given dirs/files. You of course want to use this as little
  as possible because it defeats the purpose of having a generator for the given module.


Note that processing instructions can also be used to simply exclude directories that are not used 
for your builds and as such need not be processed by the generator. Excluding such directories speeds
up the generator's processing time. As such processing instructions can also be used as an optimization.




----------------------------------------------
History:
----------------------------------------------

- 26'th June 2010:
     - First version of the native Linux port. Build on Ubuntu x86 Desktop edition.
- 11'th June 2010:
     - Added 2 command line parameters to provide more control over the generated CMake files.
       Main reason for adding these is so that the generated files can be added to version control
       without constantly being listed as modified every time the tool is run.
     - Altered header for the generated CMakeLists.txt file a little to remove the obsolete header
       info that was always added as part of every auto-generated file.
- 23'rd May 2010:
     - Added serialization of all project information into an XML project file called "Project.xml".
       The idea being that this file can be used to generate other output files then just CMake files.
       The main inspiration was generation of Android makefiles which CMake does not currently support.
- 6'th May 2010:
     - Improved relative path processing to resolve as much of the up dir segments as possible.
       This greatly reduces the number of include dirs because duplicates can now more accurately be
       detected. The vast number of includes for a top level module was actually preventing a module
       from compiling in MVC9.
     - Added check for scenario where a module has headers in both the same folder as the suffix file
       and in sub dirs. In this scenario the headers in the suffix folder would not be included via an 
       include. Now it will add a relative path back into the same dir to force CMake to add the dir.
- 16'th April 2010:
     - Greatly improved the flexibility of providing processing instructions to the generator with the
       addition of the processing instructions XML which can be used if the simple exclude list does 
       not suffice. To use this functionality you will need 2 extra modules which is an xml plugin and the
       actual xml library. See the new "Processing instructions" section in the usage notes.
     - All processing instructions are now gathered as a pre-processing step.
     - Processing instructions now support a simple wildcard for excludes. Only '*' is supported right now.
     - Added compilation timestamp in the log so you can check which version of the generator you are using
     - Added compilation timestamp in the generated CMakeLists.txt so you can check which version of the 
       generator you are using.
     - Added outputting the location of the logfile as comment in the generated CMakeLists.txt because some
       people get confused about the location of the log when specifying multiple root directories.
     - Added the creation of a ${ALL_FILES} variable so that you can now specify modules as follows:
       "add_library( exampleLib ${ALL_FILES} )" in the suffix file. This way you don't need to specify the
       4 possible variables individually.
- 18'th March 2010:
     - Drastically improved support for platform directories. Various platforms are now supported
       and any include/source directory structure can be used as long as the platform dir conforms
       to one of the supported names.
- 15'th March 2010:
     - Added include dir determination based on a dependency's dependencies.
     - Added the ability to exclude specific files using the exclude list. It can now  
       be used to exclude both sub-dirs and files. Just place each entry on a new line as before.
     - Fixed a bug with last sub-dir string logic, having a trailing \ will no longer cause 
       the last sub-dir string to be determinded as being ""
     - Changed include/source dir processing so that every sub-dir and the project dir itself will
       have all known include/source file types added to the project except for directories excluded
       using the exclude list or directories who have a suffix file and are thus treated as a project 
       defined in a sub-dir.
- 18'th Jan 2010:
     - The exclude list functionality now properly accounts for different EOL
       characters in the exclude text file if the file has multiple entries
     - In addition the exclude list can now be used to exclude specific files not
       just folders
     - Added support for multiple rootDir command line arguments in order to process
       multiple roots. This is a nice feature for combining multiple source trees into
       a single CMake project because it will automatically figure out the include directories
       etc for you.
- 12'th Jan 2010: First release on Win32