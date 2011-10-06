#!/bin/bash
#


# Turn on tracing, we want to see what's going on
#set -x

#------------------------------------------------------------------------------

function CopyPackageBinaries {

DSTDIR=$COPYPACKAGEBINARIES_SCRIPTSTARTDIR/libs/armeabi
SRCDIR=$COPYPACKAGEBINARIES_SCRIPTSTARTDIR/../../libs/armeabi

echo " "
echo "*** Copying package binary files ***"
echo " "
echo " From $SRCDIR"
echo " "
echo " To $DSTDIR"
echo " "

cp -f $SRCDIR/libmain.so $DSTDIR
cp -f $SRCDIR/libgucefLOADER.so $DSTDIR

}

#------------------------------------------------------------------------------

function CopyPackageBinariesAsAssets {

DSTDIR=$COPYPACKAGEBINARIES_SCRIPTSTARTDIR/assets/libs/armeabi
SRCDIR=$COPYPACKAGEBINARIES_SCRIPTSTARTDIR/../../libs/armeabi

echo " "
echo "*** Copying package binary files as assets ***"
echo " "
echo " From $SRCDIR"
echo " "
echo " To $DSTDIR"
echo " "

echo "* Copy main platform modules"

cp -f $SRCDIR/libgucefMT.so $DSTDIR
cp -f $SRCDIR/libgucefCORE.so $DSTDIR
cp -f $SRCDIR/libgucefVFS.so $DSTDIR
cp -f $SRCDIR/libgucefGUI.so $DSTDIR
cp -f $SRCDIR/libgucefCOMCORE.so $DSTDIR
cp -f $SRCDIR/libgucefCOM.so $DSTDIR
cp -f $SRCDIR/libgucefMATH.so $DSTDIR
cp -f $SRCDIR/libgucefDRN.so $DSTDIR
cp -f $SRCDIR/libgucefSMDRN.so $DSTDIR
cp -f $SRCDIR/libgucefMULTIPLAY.so $DSTDIR
cp -f $SRCDIR/libgucefIMAGE.so $DSTDIR
cp -f $SRCDIR/libgucefPRODMAN.so $DSTDIR
cp -f $SRCDIR/libgucefPATCHER.so $DSTDIR
cp -f $SRCDIR/libgucefINPUT.so $DSTDIR

echo "* Copy plugins"

cp -f $SRCDIR/libdstorepluginPARSIFALXML.so $DSTDIR
cp -f $SRCDIR/libGucefLogServiceClientPlugin.so $DSTDIR
cp -f $SRCDIR/libvfspluginDVP.so $DSTDIR
cp -f $SRCDIR/libvfspluginVP.so $DSTDIR
cp -f $SRCDIR/libvfspluginZIP.so $DSTDIR

echo "* Copy plugin dependencies"

cp -f $SRCDIR/libDVPACKSYS.so $DSTDIR
cp -f $SRCDIR/libGucefLogServiceLib.so $DSTDIR
cp -f $SRCDIR/libArchiveDiffLib.so $DSTDIR

echo "* Copy 3rd party plugin dependencies"

cp -f $SRCDIR/libzziplib.so $DSTDIR
cp -f $SRCDIR/libfreetype.so $DSTDIR
cp -f $SRCDIR/liblibparsifal.so $DSTDIR
cp -f $SRCDIR/libminiupnpc.so $DSTDIR
cp -f $SRCDIR/libRocketControls.so $DSTDIR
cp -f $SRCDIR/libRocketCore.so $DSTDIR
cp -f $SRCDIR/libRocketDebugger.so $DSTDIR

}

#------------------------------------------------------------------------------

echo " "
echo "*** Setting up basic variables ***"
echo " "

# Remember the dir where we started out since we use relative paths
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
COPYPACKAGEBINARIES_SCRIPTSTARTDIR=${scriptPath%/*}
echo "COPYPACKAGEBINARIES_SCRIPTSTARTDIR = $COPYPACKAGEBINARIES_SCRIPTSTARTDIR"

# Make sure the GUCEF_HOME variable is set
CopyPackageBinaries
CopyPackageBinariesAsAssets


