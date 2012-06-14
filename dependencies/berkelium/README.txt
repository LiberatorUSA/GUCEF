Berkelium copyright (C) 2009-2010 The Sirikata team.

Berkelium is a BSD licensed library that provides off-screen browser rendering
via Google's open source Chromium web browser. See http://berkelium.org for a
detailed overview.

==== Building:

For Windows instructions, see WINDOWS-BUILDING.txt in the win32/ folder.

For Mac or Linux, first use the automated script to build chromium:
util/build-chromium.sh
You can specify options to it, see --help for more information. The most
important option is probably --deps, which on supported platforms (e.g. Ubuntu,
Fedora), will try to install required dependencies from system packages.

For a Debug build of chromium, export CHROMIUM_DEBUG=true
    Warning: Debug requires 4GB RAM and 64-bit Linux or Mac OS X 10.6
    I highly recommend *not* building Chromium in Debug.
    Link times without debug are ~15 seconds.
     With debug, about 5 minutes with heavy disk access.

To build, you need CMake 2.4 or higher installed (2.6 recommended)

To generate a Makefile, run:
I recommend building Berkelium in Debug, no matter how Chromium was built:
cmake . -DCMAKE_BUILD_TYPE=Debug
    (If you are using debug Chromium, specify -DCHROME_MODE=Debug)

To build Chromium, run:
make

==== Release build:
Run cmake with:
cmake . -DCMAKE_BUILD_TYPE=Release

On mac, there is a script which will replace hardcoded absolute paths
with relative paths, as well as do all the packaging:
util/mac-install.sh

On Linux, just run "make" again, and copy the created executables. I recommend copying "libffmpegsumo.so" from chromium/src/out/Debug for video tag support, as well as any other plugins you may want to include.

==== Support:

Our new mailing list is at http://groups.google.com/group/berkelium

Before asking a question, search on google with "berkelium", which will also
find discussions in the old list archives as well.
If you can't find the answer in the archives, feel free to ask a question to
the mailing list.

The authors below should be used only as a last resort, or for private matters.

==== About the authors:

Main contributors:
Patrick Reiter Horn <patrick.horn@gmail.com>
Ewen Cheslack-Postava <ewencp@cs.stanford.edu>
kevin Gadd <kevin.gadd@gmail.com>
Daniel Reiter Horn <danielrh@users.sourceforge.net>
(feel free to commit your own name/organization here)
Some code borrowed from the Chromium project at http://chromium.org

Code is licensed under the New BSD License: see source code for the full text.

See VERSION.txt for the matching chromium revision by platform.
