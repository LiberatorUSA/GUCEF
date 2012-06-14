#!/bin/bash

# build-chromium.sh
# Builds Chromium as a dependency for Berkelium.
#
# Parameters:
# --use-root - enable the use of the root account where necessary
# --deps - try to build dependencies as well (may require --use-root on some platforms)
# -j[x] - build with x parallel compiler invocations
# --build-dir x - use x as the build directory for chromium
# --install-dir x - use x as the install destination for chromium
# --app-dir x - use x to "install" to, i.e. add symlinks from Chromium that are needed
#               by the final binary created by linking to libberkelium

platform="`uname -s`"
proctype="`uname -m`"
if [ "`linux64 uname -m`" = "x86-64" -a \! "$proctype" = "x86-64" ]; then
    install_lib32=true
fi

USE_ROOT=true
USER=`whoami`
USER_SHELL="/bin/bash"
SAME_USER=true
DO_INSTALL=true
DO_DOWNLOAD=true

if [ x"${platform}" = x"Darwin" ]; then

    if [ -e /Developer/SDKs/MacOSX10.5.sdk ] ; then
        SDK_DIR="/Developer/SDKs/MacOSX10.5.sdk"
        SDK_VER=10.5
    else
        SDK_DIR="/Developer/SDKs/MacOSX10.6.sdk"
        SDK_VER=10.6
    fi

    XCODE_VERSION_LINE=`xcodebuild -version | grep "Xcode 3"`
    if [ \! x"${XCODE_VERSION_LINE}" = x"" ]; then
	XCODE_VERSION="3"
    else
	XCODE_VERSION="4"
    fi

    NUM_PROCS=-j2
    echo "Building for Mac OS X with SDK ${SDK_VER}"
    
elif [ x"${platform}" = x"Linux" ]; then
    NUM_PROCS=-j`cat /proc/cpuinfo|grep processor|wc -l`
    echo "Building for Linux `uname -m`"
fi

SCRIPTDIR="`dirname $0`"
if [ \! x"$SCRIPTDIR" = x"" ]; then
    cd "$SCRIPTDIR/.."
fi
PWD=`pwd`

WITH_DEPS=false

# The work directory for the build -- where gclient, the chromium source, etc live
# FIXME allow paramter to override this
CHROMIUM_BUILD_DIR="${PWD}/build/"
# The directory that contains patches.  How do we make sure we always have access to this?
# Currently assumes you are running from the base directory of this repository, i.e. won't
# work from outside this project
CHROMIUM_PATCHES_DIR="${PWD}/patches"
# The directory to install to.  Currently this just ends up being a sym link.
CHROMIUM_INSTALL_DIR="${PWD}/installed-chromium"
# The directory to install the app to
CHROMIUM_APP_DIR=""

FORCE_32BIT=false
VERBOSE_FLAGS=""

################################################################################################################################
# Define a function to execute a command as a different user if necessary
################################################################################################################################

function user_eval {
    if $SAME_USER ; then
        pushd "${PWD}" >/dev/null
        $USER_SHELL -c "$1"
        RET=$?
        popd >/dev/null
        return $RET
    else
        su - $USER -s /usr/bin/env -- $USER_SHELL -c "export OLDPATH='$OLDPATH'; export PATH='$PATH'; cd ${PWD}; $1"
        return $?
    fi
}

################################################################################################################################
# Define a function to make a symlink to an existing installation
################################################################################################################################

function make_symlink {
    TARGET=$1
    LINK_NAME=$2

    user_eval "cd ${PWD}; rm -f $LINK_NAME; ln -fs $TARGET $LINK_NAME && (touch -m $LINK_NAME || true)"
}

# Unlike make_symlink, this will delete the contents, if this is not a symlink.
function clean_dir {
    DIRECTORY=$1
    user_eval "cd ${PWD}; rm -f ${DIRECTORY} 2>/dev/null; rm -rf ${DIRECTORY}"
}


# Usage: careful_patch directory patchfile
function careful_patch {
    echo
    echo "Applying patch $2"
    user_eval "cd $1 && (patch --batch -R -p0 -N --dry-run < $2 2>/dev/null || patch --batch -p0 -N < $2)"
    RET=$?
    if [[ $RET -ne 0 ]]; then
        echo "FAILED TO APPLY $2 -- Important if this patch is needed on this platform."
    fi
    return $RET
}

################################################################################################################################
# Parse command-line arguments
################################################################################################################################

until [ -z "$1" ] # until all parameters used up
do
    case "$1" in
        --deps )
            WITH_DEPS=true
            ;;
        -j* )
            NUM_PROCS="$1"
            ;;
        --build-dir )
            shift
            CHROMIUM_BUILD_DIR="$1"
            ;;
        --install-dir )
            shift
            CHROMIUM_INSTALL_DIR="$1"
            ;;
        --app-dir )
            shift
            CHROMIUM_APP_DIR="$1"
            ;;
# FIXME should be able to automatically figure this out using PWD and the executable name in the arguments
        --patch-dir )
            shift
            CHROMIUM_PATCHES_DIR="$1"
            ;;
        --chromium-version )
            shift
            CHROMIUM_VERSION="$1"
            SET_CHROMIUM_VERSION=true
            ;;
        --download-only )
            DO_DOWNLOAD=true
            DO_INSTALL=false
            ;;
        --install-only )
            DO_DOWNLOAD=false
            DO_INSTALL=true
            ;;
        --verbose )
            VERBOSE_FLAGS="V=1"
            ;;
        --force )
            GCLIENT_FORCE="--force"
            ;;
        * )
            echo "Usage: [linux32] $0 [--deps] [-j123] [--verbose] [--force]" >&2
            echo "       [--build-dir foo] [--install-dir foo] [--app-dir foo] [--patch-dir foo]" >&2
            echo "       [--chromium-version 6.0.472.27] [--chromium-version 54321]" >&2
            echo >&2
            echo "If run with linux32, does a 32-bit build on a 64-bit system." >&2
            echo "-jFOO runs parallel build with FOO processors (default autodetected)'" >&2
            echo "--force forces rerunning svn in case you encountered svn errors." >&2
            echo "--verbose shows verbose bash lines and runs gcc commands with make V=1" >&2
            echo "--build-dir, --app-dir, --install-dir : read this script to understand." >&2
            echo "--patch-dir Lets you specify other patches *instead* of default 'patches' dir" >&2
            echo "--deps attempts installing fedora/debian/ubuntu packages as root." >&2
            echo "--chromium-version sets a release version or SVN revision to download." >&2
            echo >&2
            exit 1
    esac
    shift
done

if [ \! x"$VERBOSE_FLAGS" = x ]; then
    set -x
fi

# Make sure we have absolute paths
if [ -z `echo ${CHROMIUM_BUILD_DIR} | grep ^/` ]; then
    CHROMIUM_BUILD_DIR=${PWD}/${CHROMIUM_BUILD_DIR}
fi
if [ -z `echo ${CHROMIUM_PATCHES_DIR} | grep ^/` ]; then
    CHROMIUM_PATCHES_DIR=${PWD}/${CHROMIUM_PATCHES_DIR}
fi
if [ -z `echo $CHROMIUM_INSTALL_DIR} | grep ^/` ]; then
    CHROMIUM_INSTALL_DIR=${PWD}/${CHROMIUM_INSTALL_DIR}
fi


CHROMIUM_DEPOTTOOLS_DIR="${CHROMIUM_BUILD_DIR}/depot_tools"
CHROMIUM_CHECKOUT_DIR="${CHROMIUM_BUILD_DIR}/chromium"

# Chromium revision to build. Ideally we could keep these synced across all platforms.
if [ x"${SET_CHROMIUM_VERSION}" = x ]; then
    if [ x"${platform}" = x"Darwin" ]; then
        CHROMIUM_VERSION=`grep 'mac *=' VERSION.txt | cut -d= -f2`
    elif [ x"${platform}" = x"Linux" ]; then
        CHROMIUM_VERSION=`grep 'linux *=' VERSION.txt | cut -d= -f2`
    fi
    # Cleans up all spaces.
fi
CHROMIUM_VERSION=${CHROMIUM_VERSION// }
if echo ${CHROMIUM_VERSION} | grep -q '[0-9]*\.[0-9]*\.[0-9]*\.[0-9]*'; then
    CHROMIUM_REPO=http://src.chromium.org/svn/releases/${CHROMIUM_VERSION}
    CHROMIUM_REV=""
else
    CHROMIUM_REPO=http://src.chromium.org/svn/trunk/src
    CHROMIUM_REV="--revision src@${CHROMIUM_VERSION}"
fi

if [ x"${platform}" = x"Darwin" ]; then

    mkdir -p "${CHROMIUM_BUILD_DIR}"
    cd "${CHROMIUM_BUILD_DIR}"
    if [ \! -e depot_tools ]; then
        curl -o depot_tools.tar.gz http://src.chromium.org/svn/trunk/tools/depot_tools.tar.gz
        tar -zxf depot_tools.tar.gz
    fi
    PATH="${CHROMIUM_DEPOTTOOLS_DIR}:$PATH"
    echo "${CHROMIUM_CHECKOUT_DIR}"
    mkdir -p "${CHROMIUM_CHECKOUT_DIR}"
    cd "${CHROMIUM_CHECKOUT_DIR}"
    rm -f .gclient
    gclient config ${CHROMIUM_REPO}
    python -c 'execfile(".gclient");solutions[0]["custom_deps"]={"src/third_party/WebKit/LayoutTests":None,"src/webkit/data/layout_tests":None,};open(".gclient","w").write("solutions="+repr(solutions));';
    gclient sync $GCLIENT_FORCE ${CHROMIUM_REV}
    for patch in "${CHROMIUM_PATCHES_DIR}"/*.patch; do
        careful_patch "${CHROMIUM_CHECKOUT_DIR}/src" "${patch}"
    done
    cd src/chrome
    if [ x"${XCODE_VERSION}" = x"3" ]; then
	xcodebuild -project chrome.xcodeproj -configuration Release -target chrome -sdk ${SDK_DIR}
    else
	xcodebuild -project chrome.xcodeproj -configuration Release -arch i386 -target chrome -sdk ${SDK_DIR}
    fi


    # "Install" process, symlinking libraries and data to the appropriate locations
    if [ x"${CHROMIUM_APP_DIR}" != x ]; then
        # Make sure the top level build dir is there
        if [ \! -e "${CHROMIUM_APP_DIR}" ]; then
            user_eval "mkdir -p '${CHROMIUM_APP_DIR}'" || true
        fi

        ln -sf "${CHROMIUM_DATADIR}/chrome.pak" "${CHROMIUM_APP_DIR}/chrome.pak"
        ln -sf "${CHROME_ROOT}/src/xcodebuild/Release/${CHROMIUM_FRAMEWORK}/Resources/chrome.pak" "${CHROMIUM_APP_DIR}/chrome.pak"
        ln -sf "${CHROME_ROOT}/src/xcodebuild/Release/${CHROMIUM_FRAMEWORK}/Versions" "${CHROMIUM_APP_DIR}/Versions"
    fi
elif [ x"${platform}" = x"Linux" ]; then

    # Make sure the top level build dir is there
    if [ \! -e ${CHROMIUM_BUILD_DIR} ]; then
        user_eval "mkdir -p ${CHROMIUM_BUILD_DIR}" || true
    fi

    # Real dependencies
    if $WITH_DEPS ; then
        if $USE_ROOT ; then
            echo "Installing Chromium Prerequisites. Package listing is from:"
            echo "http://code.google.com/p/chromium/wiki/LinuxBuildInstructionsPrerequisites"
	    if [[ -n `which apt-get` ]] ; then
		DISTRO=debian
	    elif [[ -n `which yum` ]] ; then
		DISTRO=fedora
	    else
        # TODO: Replace with message listing packages that should be installed as root, as per Patrick's suggestion
		echo "Installing as root is not supported on your distribution."
		exit
	    fi

            case "$DISTRO" in
                debian )
                    sudo apt-get install subversion pkg-config python ruby perl g++ g++-multilib
                    sudo apt-get install bison flex gperf libnss3-dev libnspr4-dev libgtk2.0-dev libnspr4-0d libasound2-dev
                    sudo apt-get install msttcorefonts libgconf2-dev libcairo2-dev libdbus-1-dev
                    sudo apt-get install liborbit2-dev libpopt-dev orbit2 libjpeg-dev libbz2-dev
                    sudo apt-get install libnss3-dev libgnome-keyring-dev libdbus-glib-1-dev
                    sudo apt-get install libcups2-dev libxss-dev libxtst-dev libpam0g-dev
                    ;;
                fedora )
                    sudo yum install subversion pkgconfig python perl ruby gcc-c++ bison \
                        flex gperf nss-devel nspr-devel gtk2-devel.i386 glib2-devel.i386 \
                        freetype-devel.i386 atk-devel.i386 pango-devel.i386 cairo-devel.i386 \
                        fontconfig-devel.i386 GConf2-devel.i386 dbus-devel.i386 alsa-lib-devel
                    sudo rpm --force -Uvh /var/cache/yum/updates/packages/pango-devel-1.22.3-1.fc10.i386.rpm
                    ;;
            esac


            if [ x"$install_lib32" = x"true" ] ; then
                # make sure /usr/local/lib32 exists
                if ! [ -e /usr/local/lib32 ]; then
                    sudo mkdir /usr/local/lib32
                fi
                if ! [ -e /usr/lib32/libnss3.so -o -e /usr/local/lib32/libnss3.so ] ; then
                    #------------from google's script
                    sudo mkdir -p /usr/local/lib32
                    find_curl # make sure curl is available
                    for deb in nspr/libnspr4-0d_4.7.1~beta2-0ubuntu1_i386.deb \
                        nss/libnss3-1d_3.12.0~beta3-0ubuntu1_i386.deb \
                        sqlite3/libsqlite3-0_3.4.2-2_i386.deb; do
                        WORKDIR=$(mktemp -d -p /tmp)
                        pushd ${WORKDIR}
                        letter=$(echo ${deb} | sed -e "s/^\\(.\\).*$/\\1/")
                        user_eval "${CURL_BIN} -O http://mirrors.kernel.org/ubuntu/pool/main/${letter}/${deb}"
                        ar x $(basename ${deb})
                        tar xf data.tar.gz
                        for lib in usr/lib/* ; do
                            libbase=$(basename ${lib})
                            sudo mv ${lib} /usr/local/lib32/${libbase}
                            so=$(echo ${libbase} | sed -e "s/^\\(.*\\.so\\)\\..*$/\\1/")
                            if [ ${so} != ${libbase} ] ; then
                                sudo ln -s -f ${libbase} /usr/local/lib32/${so}
                            fi
                        done
                        popd
                        rm -rf ${WORKDIR}
                    done
                    #-------------------------
                fi

                #ln -s ../local/lib32/libnss3.so /usr/lib32/
                #ln -s ../local/lib32/libnspr4.so /usr/lib32/
                #ln -s ../local/lib32/libnssutil3.so /usr/lib32/
                #ln -s ../local/lib32/libplc4.so /usr/lib32/
                #ln -s ../local/lib32/libplds4.so /usr/lib32/
                #ln -s ../local/lib32/libsmime3.so /usr/lib32/
                #ln -s ../local/lib32/libsqlite3.so /usr/lib32/
                #ln -s ../local/lib32/libssl3.so /usr/lib32/
                sudo ln -s /usr/lib32/libcrypto.so.* /usr/local/lib32/libcrypto.so
                sudo ln -s /usr/lib32/libssl.so.* /usr/local/lib32/libssl.so
                sudo ln -s /usr/lib32/libgtk-x11-2.0.so.? /usr/local/lib32/libgtk-x11-2.0.so
                sudo ln -s /usr/lib32/libgdk-x11-2.0.so.? /usr/local/lib32/libgdk-x11-2.0.so
                sudo ln -s /usr/lib32/libgdk_pixbuf-2.0.so.? /usr/local/lib32/libgdk_pixbuf-2.0.so
                sudo ln -s /usr/lib32/libatk-1.0.so.? /usr/local/lib32/libatk-1.0.so
                for lib in gio-2.0.so.0 gdk-x11-2.0.so.0 atk-1.0.so.0 gdk_pixbuf-2.0.so.0 \
                    pangocairo-1.0.so.0 pango-1.0.so.0 pangoft2-1.0.so.0 gthread-2.0.so.0 glib-2.0.so.0 \
                    gobject-2.0.so.0 gmodule-2.0.so.0 glib-2.0.so.0 gtk-x11-2.0.so.0 \
                    cairo.so.2 freetype.so.6 z.so.1 fontconfig.so.1 \
                    X11.so.6 Xrender.so.1 Xext.so.6 gconf-2.so.4 asound.so.2; do
                    so=$(echo ${lib} | sed -e "s/^\\(.*\\.so\\)\\..*$/\\1/")
                    sudo ln -s /usr/lib32/lib$lib /usr/local/lib32/lib$so
                done
            fi
        fi
    fi

    # Depot tools
    if [ \! -e ${CHROMIUM_DEPOTTOOLS_DIR} ] && $DO_DOWNLOAD; then
        echo "Downloading chromium..."
        user_eval "cd ${CHROMIUM_BUILD_DIR} &&
                 svn co http://src.chromium.org/svn/trunk/tools/depot_tools/"
    fi
    
    if $DO_DOWNLOAD; then
        echo "${CHROMIUM_CHECKOUT_DIR}"
        mkdir -p ${CHROMIUM_CHECKOUT_DIR}
        user_eval "
                 mkdir -p ${CHROMIUM_CHECKOUT_DIR} &&
                 cd ${CHROMIUM_CHECKOUT_DIR} &&
                 export PATH=\"${CHROMIUM_DEPOTTOOLS_DIR}:${PATH}\" &&
                 rm -f .gclient && gclient config ${CHROMIUM_REPO} &&
                 python -c '"'execfile(".gclient");solutions[0]["custom_deps"]={"src/third_party/WebKit/LayoutTests":None,"src/webkit/data/layout_tests":None,};open(".gclient","w").write("solutions="+repr(solutions));'"' &&
                 export GYP_GENERATORS=make &&
                 gclient sync $GCLIENT_FORCE ${CHROMIUM_REV}" ||
            export FAILED="$FAILED chromium"
    fi
    
    if $DO_INSTALL; then
	    # And now for the real chromium
	    echo "Installing Chromium... "
	    echo "Without debug, it takes about 3.5 gigabytes of disk space to install and build the source tree."
	    echo "Hit Ctrl-C if you are going to run out of space, so you don't end up with truncated objects."
	    clean_dir ${CHROMIUM_INSTALL_DIR}
	    if [ x"${CHROMIUM_DEBUG}" = xtrue ]; then
	        echo "*** Building Chromium in Debug mode"
	        MAKEFLAGS=" BUILDTYPE=Debug"
	        OUTDIR=Debug
	    else
	        echo "Building Chromium in Release. export CHROMIUM_DEBUG=true for debug mode."
	        MAKEFLAGS=" BUILDTYPE=Release"
	        OUTDIR=Release
	    fi
	    if [ x"$proctype" = x"x86_64" ]; then
	        CHROME_PLATFORM=x64
	        GYP_DEFINES="${GYP_DEFINES} linux_fpic=1"
	    else
	        CHROME_PLATFORM=ia32
	    fi
	
	    GCC_VERSION=`gcc --version | grep ^gcc | cut -d " " -f 4`
	    GCC_VERSION_MAJOR=`gcc --version | grep ^gcc | cut -d " " -f 4 | cut -d "." -f 1`
	    GCC_VERSION_MINOR=`gcc --version | grep ^gcc | cut -d " " -f 4 | cut -d "." -f 2`
	    GCC_VERSION_REV=`gcc --version | grep ^gcc | cut -d " " -f 4 | cut -d "." -f 3`
	    if [ "${GCC_VERSION_MAJOR}" -gt "4" -o "${GCC_VERSION_MAJOR}" -eq "4" -a "${GCC_VERSION_MINOR}" -ge "6" ]; then
	        GCC_46_FIX_CXXFLAGS="-Wno-conversion-null -Wno-unused-but-set-variable -Wno-unused-result -Wno-int-to-pointer-cast"
	    else
	        GCC_46_FIX_CXXFLAGS=""
	    fi
	    if [ -e ${CHROMIUM_CHECKOUT_DIR} ]; then

                # Some patches need to selectively applied. They have
                # special extensions so the majority can just always
                # be applied
                VOLATILE_MALLOC=`grep volatile /usr/include/malloc.h`
	        if [ x"$VOLATILE_MALLOC" != x ]; then
	            for patch in "${CHROMIUM_PATCHES_DIR}"/*.patch.volatile-malloc; do
	                careful_patch "${CHROMIUM_CHECKOUT_DIR}/src" "${patch}"
	            done
                fi

                # The rest always get applied
	        for patch in "${CHROMIUM_PATCHES_DIR}"/*.patch; do
	            careful_patch "${CHROMIUM_CHECKOUT_DIR}/src" "${patch}"
	        done &&
	        user_eval "
	                 cd ${CHROMIUM_CHECKOUT_DIR} &&
	                 export PATH=\"${CHROMIUM_DEPOTTOOLS_DIR}:${PATH}\" &&
	                 export GYP_GENERATORS=make &&
	                 export CHROMIUM_ROOT="'"$PWD"'" &&
	                 export GYP_DEFINES='${GYP_DEFINES} disable_nacl=1 target_arch=${CHROME_PLATFORM}' &&
	                 export CXXFLAGS=\"${GCC_46_FIX_CXXFLAGS}\" &&
	                 gclient runhooks --force &&
	                 cd src &&
	                 make $VERBOSE_FLAGS -r $NUM_PROCS $MAKEFLAGS chrome" && \
                     make_symlink ${CHROMIUM_CHECKOUT_DIR}/src/out/$OUTDIR ${CHROMIUM_INSTALL_DIR} && \
                     echo ${OUTDIR} > ${CHROMIUM_BUILD_DIR}/compilemode.txt || \
                     export FAILED="$FAILED chromium"
	
	
	
	        # "Install" process, symlinking libraries and data to the appropriate locations
	        if [ x"${CHROMIUM_APP_DIR}" != x ]; then
	            # Make sure the top level build dir is there
	            if [ \! -e ${CHROMIUM_APP_DIR} ]; then
	                user_eval "mkdir -p ${CHROMIUM_APP_DIR}" || true
	            fi
	
	            user_eval "ln -sf ${CHROMIUM_DATADIR}/chrome.pak ${CHROMIUM_APP_DIR}/chrome.pak
	                       ln -sf ${CHROMIUM_DATADIR}/libffmpegsumo.so ${CHROMIUM_APP_DIR}/libffmpegsumo.so
	                       ln -sf ${CHROMIUM_DATADIR}/locales ${CHROMIUM_APP_DIR}/locales
	                       ln -sf ${CHROMIUM_DATADIR}/resources ${CHROMIUM_APP_DIR}/resources" || \
                           export FAILED="$FAILED chromium"
	
	        fi
	    else
	        export FAILED="$FAILED chromium"
	    fi
	fi
fi

if [ x"${FAILED}" != x ]; then
    echo "Failed to install:" ${FAILED}
    exit 1
fi

exit 0
