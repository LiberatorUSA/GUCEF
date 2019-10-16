#-------------------------------------------------------------------
# This file is part of the CMake build system for GUCEF
#     (Galaxy Unlimited Framework)
# For the latest info, see http://www.VanvelzenSoftware.com/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure Tools build

add_subdirectory(DuplicateFileFinder)
add_subdirectory(DVPPackTool)
add_subdirectory(FileReplacer)
add_subdirectory(HDFiller)
add_subdirectory(PatcherGUI)
add_subdirectory(PatchSetGenerator)
add_subdirectory(ServerPortExtender)
add_subdirectory(CMakeListGenerator)
add_subdirectory(CallstackLogAnalyzer)
add_subdirectory(ArchiveDiffLib)
add_subdirectory(ArchiveDiff)
add_subdirectory(SVNMagicMerge)
add_subdirectory(ReferenceUpdater)
add_subdirectory(GucefArchiver)
add_subdirectory(GucefLogServiceLib)
add_subdirectory(GucefLogServiceClientPlugin)
add_subdirectory(GucefLogServiceApp)
add_subdirectory(GucefLogService_TestApp)
add_subdirectory(ProjectGen)
add_subdirectory(ProjectGen/plugins/ProjectGenVSImporter)
add_subdirectory(ProjectGen/plugins/ProjectGenDependsFilter)
add_subdirectory(ProjectGenerator)
add_subdirectory(FileDiff)
add_subdirectory(MemoryLeakFinder)
add_subdirectory(SocketSink)
add_subdirectory(DCSBruteInstaller)
add_subdirectory(udp2kafka)
add_subdirectory(udp2redis)

