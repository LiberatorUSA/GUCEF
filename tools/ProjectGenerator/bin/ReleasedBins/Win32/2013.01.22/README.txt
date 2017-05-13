*
*  ProjectGenerator
*  Copyright (C) 2002 - 2012.  Dinand Vanvelzen
*
*  This software is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 3 of the License, or (at your option) any later version.
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

Intro:
The ProjectGenerator tool is meant for automatically generating the bulk 
of the project file content based on directory structure and file analysis.
This reduces maintenance time for module/project files and makes it easy to 
generate such files for 3'rd party sources with little effort.

History:
I, Dinand Vanvelzen, created this tool to help me build the Galaxy Unlimited
codebase using CMake without having to manually write all the CMake files for
GU libraries and their dependencies. Later I started doing native Android 
development which required Android.mk files which CMake could not provide.
As such the CMakeListGenerator tool was later renamed and its functionality 
expanded and enhanced to include support for different generator backends 
besides CMake.

Example usefullness:
One of the advantages is being able to dump new code releases of dependencies 
into the archive without having to go through the code to check which files got
added/deleted etc. Just re-run the generator and you will have up-to-date
module/project files for the build system you are using.
An additional advantage is the automatic generation of include directory paths
which can be a pain to get right, especially with a lot of modules in a complicated
directory structure. Using this tool you can just move the modules around and their
include paths will be correct again as soon as you re-run the generator.



----------------------------------------------
Version:
----------------------------------------------

This is a release build created on 22'nd of January 2013.
It is a 32-bit Win32 application for the x86 platform.
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
  This can be used as input to other tools in a toolchain.
- Extensive flexibility wrt defining modules for multiple platforms just once
  without sacrificing the ability to make platform specific customizations
- Support for legacy module definitions as dictated by the CMakeListGenerator tool
- Support for a special kind of module called a "HeaderIncludeLocation" which allows
  you to reference a location where you store headers as if it where a module.
- Support for a special kind of module called a "CodeIncludeLocation" which allows
  you to embed code from another location into your module by adding a dependence on
  such a location.
- Support for multiple generator backends:
     - CMake
     - XML
     - Android make


----------------------------------------------
Features: Supported generators
----------------------------------------------

The currently supported generator backends are compiled into the tool. 
The support generators are:

  - "cmake"
       Generates CMakeList.txt files which the CMake tool can consume
       This generator only generates the module files not the overall
       project file since in the case of CMake this involves a lot of scripting.
  - "androidmake"
       Generates Android.mk files for all the modules and creates a
       top level 'project' file which includes all the generated module
       make files.
  - "xml"
       Generates a "Project.xml" file which is simply a dump of all the gathered
       information. If you wish to integrate the ProjectGenerator with toolchains
       not supported by a generator it is expected that you achieve such integration
       via this output option.


----------------------------------------------
Inner workings: Staging
----------------------------------------------

The project generator tool works in several stages
These are as follows:
  1) Gather all processing instructions from all dirs
  2) Define each module as specified and supplement with autogenerated info
  3) Determine the build order of each module for each platform
  4) Determine the dependency inherited include paths of each module
  5) Invoke all desired generators to produce the desired files


----------------------------------------------
Usage: Overview (New system)
----------------------------------------------

Note that the method described here can be used in paralell to the "Legacy System"
The legacy system does not support all the features the new system does and you are urged
to switch to this method of defining modules as quickly as possible.

There are 4 kinds of files involved in the definition of modules, these are:
- ModuleInfo.xml:
        File which defined the module based on which project files are automatically generated.
        For more info on the schema for this file see "Usage: Defining a module using ModuleInfo.xml"
- ProjectGenExcludeList.txt:
        A simple text file containing a sub-dir/file name per line that should be 
        excluded from CMakeListGenerator processing. This is also referred to as the 
        "Simple exclude list" because it is simple to specify but lacks some flexibity.
        If you to to include as well or need to do platform specific processing use the 
        xml based instruction file described below. For more details see the section 
        on processing instructions.
- ProjectGenInstructions.xml:
        Offers a more advanced method of changing how data will be processed by the
        generator. It allows you to specify which files/directories to include/exclude
        and for what platform. For more details see the section on processing instructions.

The generator tool is meant to run as part of your build system setup before running 
followup tools such as CMake. In combination the generator plus followup tooling
will allow you to automate almost every aspect of setting up your module/project files.



----------------------------------------------
Usage: Overview (Legacy system)
----------------------------------------------

Note even though ProjectGenerator uses a different method of defining modules it still
supports the old method using a CMakeListsSuffix file. Both methods can be used at the 
same time to allow developers to migrate over time to the new system. Do note that this
old method is considered deprecated and will be removed in some future release.

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
Usage: Defining a module using ModuleInfo.xml
----------------------------------------------

A directory is considered to contain a module if it has a "ModuleInfo.xml" file in it.
This file is a minimalist definition of a module in your project. Most of the information
regarding the module will be automatically generated. You can define as much or as little
of your module as you wish in the ModuleInfo.xml. Most xml tags/sections are optional.

Minimalist ModuleInfo.xml example:

<ModuleInfoEntry>
    <Module Name="gucefMT" Type="SharedLibrary" Platform="All">
        <Dependencies>
            <Dependency Name="gucefHeaders" />
        </Dependencies>
        <Preprocessor>
            <Define String="GUCEF_MT_BUILD_MODULE" />
        </Preprocessor>
    </Module>
    <Module Platform="mswin">
        <Linker>
            <Dependency Name="Winmm" />
        </Linker>
    </Module>
    <Module Platform="linux">
        <Linker>
            <Dependency Name="pthread" />
        </Linker>
    </Module>
</ModuleInfoEntry>

The above example shows how to define a module named "gucefMT" which is to be a shared 
library (dll/so) for the platform "All". 

--- Platform "All"
The platform tag "All" is a special plaform which can be used to indicate that the code 
is not platform specific but just standard C/C++ or perhaps it has its own macros and whatnot
to deal with being platform independent. A module definition for the "All" platform is considered 
a baseline which specific platform info can supplement. You can see an example of this in the 
linux platform section where an additional linker dependency is defined. 
  "All" + "linux" = all linker dependencies. 
In this case the module has no "All" platforms linker dependency and as such its total number of linker 
dependecies for the linux platform is 1.

--- Platform "mswin"
The mswin platform is a way of specifying all ms windows desktop platforms which are
 "win32" and "win64"
The same is accomplished by specifiying "win32;win64"

--- Platform overrides
Note that besides supplementing info from "All" platforms you can also override information such as 
the build order or the module name. This is typically not though and supplemental info is the most
common usage. If you wanted to override the module's name for the mswin platform you would simply
add it to that platform's Module description:
  <Module Name="MsWinGucefMt" Platform="mswin">


-- Possible sections in a Module definition for a given platform:

<Module BuildOrder="<buildIndexForModule>" Name="<nameOfYourModule>" Type="<moduleTypeString>" Platform="<supportedPlatform>">
    <Files Type="<whichTypeOfFilesAreThese(Headers|Source)>" >
        <Dir Path="<relativePathToDir>" >
            <File Name="<filename>" />
        </Dir>
    </Files>
    <Includes Source="whereDidIncludesComeFrom(Self|Dependency)" >
        <Include Path="<relativeIncludePath>" />
    </Includes>
    <Dependencies>
        <Dependency Name="<nameOfDependencyModuleAlsoInProject>" />
    </Dependencies>
    <Preprocessor>
        <Define String="<somePreprocessorDefine>" />
    </Preprocessor>
    <Linker>
        <Dependency Name="<nameOfLibraryToLinkTo>" />
    </Linker>
</Module>

Each platform specification is noted as a seperate <Module> section all of which are combined in the
<ModuleInfoEntry> section which contains the entire definition of the module for all platforms.
The <supportedPlatform> for <Module> can contain multiple platforms which must be seperated by a ;
<supportedPlatform1;supportedPlatform2> is a valid value and the same defintion will apply to both
those platforms.

Note that the modules mentioned under "Dependencies" determine the build order and include paths
This is seperate from the dependencies named in the "Linker" section since you can be dependent on
a module building first or be dependent on its headers without wanting to link to that module.

Typically there is no need to specify <Files> or <Includes> since the ProjectGenerator automatically
discovers the files and includes for you.

The type of the module is specified as in <moduleTypeString> which must be one of the following:
 - "Executable"
 - "SharedLibrary"
 - "StaticLibrary"
 - "HeaderIncludeLocation"
 - "CodeIncludeLocation"

The "HeaderIncludeLocation" is a special module type which is only used for determination of include paths
It will not be translated into an actual module. The advantage is that you can refer to such a location without
using the path. Instead you can use the name of the HeaderIncludeLocation to refer to paths that should be added
to your includes.

The "CodeIncludeLocation" is also a special module type which is used to merge code into a module as if it were part
of a module's file structure. The main use-case for this is code which uses shared code that gets compiled into multiple
modules instead of the usual exporting/importing from a shared module. Headers and source files belonging to a module of
this type will get merged into modules which reference it as a dependency.


----------------------------------------------
Usage: Command line parameters
----------------------------------------------

If no command line arguments are given then the tool will start processing 
the directory structure using the current working directory as the root.
Like most GU applications command line parameters accepted in the form:
*<key>=<value>*

The tool accepts the following command line parameters:

- *addToolCompileTimeToOutput=<true/false>*
     Determines whether to add the compile date-time of the tool to the header of all the CMake 
     files that are generated. This is particulary usefull when you are suspecting that your CMake
     files are not being generated by the correct version of the generator tool.
     The default is for this feature to be turned off.

- *writeLogLocationToOutput=<true/false>*
     Determines whether the location of the tool logfile should be added at the end of the 
     generated CMake files. This is mainly usefull for debugging your build scripts.
     The default is for this feature to be turned off.

- *rootDir=<path to processing root>*
    Example:
       *rootDir=C:\Dev\MyCodeRoot*

     Note that you can pass multiple instances of the rootDir argument in order
     to pass multiple root directories.

    Example:
       *rootDir=C:\Dev\MyCodeRoot* *rootDir=D:\OtherDev\MyCodeRoot*

     Passing multiple root dirs will allow include dependencies between the two root dirs
     to be resolved thus allowing them to be used as if it's all part of the same project set.

     Note that if no rootDir's are passed then the default is used which is the current working
     directory.

- *projectName=<name of your project>*
    Example: 
       *projectName=GUCEF*

     Note that this setting may not affect the output depending on which generator is used.
     If you do not pass this parameter the generators will simply not be given a project name.

- *generators=<nameOfGenerator1>;<nameOfGenerator2>*
    Example:
       *generators=cmake;androidmake;xml*

     This parameter tells the tool which generators should be used to generate project/module
     files basedon the gathered information. Check the feature list for a list of which generators
     are actually supported in this release. You can use as many generators as you wish, just
     seperate them with a ;
     Note that if this parameter is not passed to the tool then only the default generator will 
     be used. The default generator is "xml".

- *dirsToIgnore=<dirNameToIgnoreGlobally1>;<dirNameToIgnoreGlobally2>*
    Example:
       *dirsToIgnore=_svn;.svn*

     This parameter tells the information gatherer in the tool to ignore the directories specified
     at all times. This is especially usefull with something like SVN sub-directories since they are
     present in every archive directory. You would not want to add processing instructions to every single
     archive dir just to exclude these. In such scenarios this param is the recommended dir exclusion method.


---------------------------------------------------------------
Usage: Generator specific command line parameters: CMake
---------------------------------------------------------------

The CMake generator supports the following additonal params:

- *cmakgen:TemplateDir=<pathToTemplateDir>
    Example:
       *cmakgen:TemplateDir=C:\Dev\MyCodeRoot\projects\MyProjectGenCMakeTemplates*

     The parameter tells the CMake generator to look for template files in the given directory
     These templates will then be used by the CMake generator for modules. Note that modules defined
     using the legacy system cannot use this feature. For more information about this featue read the
     section called "Usage: CMake generator templates".



----------------------------------------------
Usage: CMake generator templates
----------------------------------------------

CMake generator templates are text files which have CMake script code in them. This script code can 
include any of the supported tags which will be replaced by information obtained per module by the 
ProjectGenerator. Template files must always have the file extension "cmt" which stands for CMake
Template. After the tags are resolved into their values the result is merged into the CMakeLists file 
content produced by the ProjectGenerator. Due to CMake's already extensive scription abilities it is 
recommended to use this feature to simply invoke a CMake syntax function which does whatever you need 
it to do. Doing so will keep your templates simple and easy to maintain.

Supported tags:
    - $#$MODULENAME$#$
      Will be replaced by the name of the module as used by the CMakeLists generator.
      Ideal use is to invoke a function using this value:
          Example:
            my_cmake_config_function( $#$MODULENAME$#$ )


Note that at present templates are applied per module type. Depending on the module type a specific
template should be defined. At present this means you can have the following template files:
  - "Executable.cmt"
  - "SharedLibrary.cmt"
  - "StaticLibrary.cmt"

An excecutable will have the template "Executable.cmt" applied etc.


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
for CMake depending on the target platform PLATFORM_HEADER_FILES and PLATFORM_SOURCE_FILES
will be set to the correct list of files. Different generators might use different methods 
but the concept is the same. In this case the if clause will check for platform WIN32. 
which directory names map to which platform is listed below.
You must name the directories according to one of the supported directory names
in order to make use of this feature to exclude/include files based on the target
platform.

Supported platform directories:
  "mswin" for platform "WIN32"
  "win32" for platform "WIN32"
  "win64" for platform "WIN64"
  "linux" for platform "linux"
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
    tie-in plugin library which is "dstorepluginPARSIFALXML.dll" (or whatever the module's name is 
    for the platform distribution you are using).

The simple exclude list is, as you might guess, simple. As described in the overview it is simply
a list consisting of each item being placed on a different line. There is no way to specify whether
an item is a directory or a file, this will be automatically determined, nor can you specify includes
or anything platform specific. This seems very restrictive but in practice the simple exclude list 
serves most use cases. Typically you just want to exclude some test code or unrelated directories etc
for which the simple exclude list is sufficient.

If you cannot get the generator to process your module correctly using the exclude list then you
can utilize its more advanced sibling which is an XML file which allows you to specify more.
The XML file layout is shown in the example below:

<PROJECTGENERATOR>
  <EXCLUDES PLATFORM="ALL">
    <ITEM NAME="memunix.c" TYPE="FILE" />
    <ITEM NAME="Samples" TYPE="DIR" />
  </EXCLUDES>
  <INCLUDES PLATFORM="LINUX">
    <ITEM NAME="memunix.c" TYPE="FILE" />
  </INCLUDES>
</PROJECTGENERATOR>


Note that the legacy root node name "CMAKELISTGENERATOR" instead of "PROJECTGENERATOR" is also still supported.

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
- File includes are supported such that you can include files with a different path relative to the location 
  of the processing instructions file. If you set "NAME" to not just the file name but include a path then this
  path will be taken into account. This is especially useful if you wish to embed source from another module
  into your module without duplicating the source files in your archive.


Note that processing instructions can also be used to simply exclude directories that are not used 
for your builds and as such need not be processed by the generator. Excluding such directories speeds
up the generator's processing time. As such processing instructions can also be used as an optimization.




----------------------------------------------
History:
----------------------------------------------

- 22'nd January 2013 :
     - Build using latest dependencies, no other changes
- 4'th April 2012 :
     - Changes to ensure preprocessor defines are alphabetically ordered every time the scripts are generated.
       This ensures the generated scripts wont show as changed when re-running the ProjectGenerator.
- 20'th January 2012:
     - Linux only: There was a symbol error in the previous binary release. This is a new build from the new trunk code.
       No new ProjectGenerator specific features were added.
- 6'th September 2011:
     - Added support for module type "CodeIncludeLocation". This had to be implemented to support 
       thirdparty code which was using the same code in multiple modules by simply including the code from multiple
       locations. I would not recommend organizing your code this way, but if you need it its now supported.
     - Fixed a bug in the Android makefile generator which could cause some files to be left out of a module by mistake
       when merging a All platforms definition with an Android one.
     - Fixed the Android makefiles doing an if check on the variable value instead of variable existance.
- 5'th April 2011:
     - First version of the ProjectGenerator which supports everything the CMakeListGenerator used to support
       plus a lot more. This version support the legacy system and the old system and is meant to bridge between
       the two tools.
     - Basic template support was added for the CMake generator
     - Complete restructuring of the internal data structures allowing greatly increased flexibility in the way
       modules are defined.
     - This is the first version of ProjectGenerator used to generate all the modules/project files for the GUCEF
       archive itself.
- 26'th Jan 2011:
     - First version under the name of ProjectGenerator. Added Android makefile support to the already
       implemented support for CMake and just a plain XML dump of the gathered information.
     - Note that because of issues with the Linux version the character used to seperate cmd line params
       has been changed from ' to *. Please update your scripts invoking the tool accordingly.
     - This version was never publically announced since only the basic features where operational.
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
