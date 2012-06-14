#!/bin/bash

outdir="$1"
[ x"$CHROME_ROOT" = x ] && CHROME_ROOT="build/chromium"

if [ x = x"$outdir" ]; then
    echo "Usage: $0 output-directory"
    echo
    echo "Copies just the necessary files for mac into a single directory."
    echo "Will also compress these files into a tarball with today's date."
    echo
    echo "Warnings:"
    echo "- Run from the berkelium dir. Assumes Release chromium build in ./$CHROME_ROOT."
    echo "  (Change the location of berkelium by exporting CHROME_ROOT)"
    echo "- Do not run as root."
    echo "- Requires that the directory you compiled in is at least 23 characters long:"
    echo "  (e.g. /Users/alfred/berkelium but not /Users/al/berkelium)"
    echo "  This is because we replace strings in the executable files."
    exit 1
fi

chromiumappdir="$CHROME_ROOT/src/xcodebuild/Release/Chromium.app"
if [ \! -d "$chromiumappdir" ]; then
    echo "You need to build chromium first!"
    echo "If done, $chromiumappdir should exist."
    exit 1
fi
rm CMakeCache.txt
cmake . -DCMAKE_BUILD_TYPE=Release || exit 1
make || exit 1

echo "Installing into $outdir"
mkdir "$outdir" || echo "Note: installing into unclean directory! You may end up with unused files."
mkdir -p "$outdir/bin"
mkdir -p "$outdir/include/berkelium"
mkdir -p "$outdir/lib/Chromium Framework.framework"
for versionsrc in "$CHROME_ROOT/src/xcodebuild/Release/Chromium.app/Contents/Versions"/*; do
    versionnum=$(basename "$versionsrc")
done
mkdir -p "$outdir/lib/Chromium Framework.framework/Libraries"
cp "$versionsrc/Chromium Framework.framework/Libraries/libffmpegsumo.dylib" "$outdir/lib/Chromium Framework.framework/Libraries/"
mkdir -p "$outdir/lib/Chromium Framework.framework/Resources/en.lproj"
for copyfile in chrome.pak common.sb nacl_loader.sb renderer.sb utility.sb worker.sb en.lproj/locale.pak; do
    cp "$versionsrc/Chromium Framework.framework/Resources/$copyfile" "$outdir/lib/Chromium Framework.framework/Resources/$copyfile"
done
cp build/chromium/src/third_party/WebKit/WebCore/Resources/*Cursor.png "$outdir/lib/Chromium Framework.framework/Resources/"
for header in "include/berkelium"/*; do
    cp "$header" "$outdir/$header"
done
for exe in berkelium ppmrender glut_demo glut_input libplugin_carbon_interpose.dylib; do
    echo "Installing executable $exe"
    ./bin_replace "$PWD/" '@executable_path/../lib/' < "$exe" > "$outdir/bin/$exe"
    chmod +x "$outdir/bin/$exe"
done
lib="liblibberkelium.dylib"
echo "Installing shared library $lib ..."
./bin_replace "$PWD/$lib" '@executable_path/../lib/'"$lib" < "$lib" | \
    ./bin_replace "$PWD/" 'Berkelium/' > "$outdir/lib/$lib"
chmod +x "$outdir/lib/$lib"

echo "Installing GLUT Sample.app"
mkdir -p "$outdir/GLUT Sample.app/Contents/MacOS"
ln -fs '../../lib' "$outdir/GLUT Sample.app/Contents/lib"
for exe in berkelium glut_input libplugin_carbon_interpose.dylib; do
    cp "$outdir/bin/$exe" "$outdir/GLUT Sample.app/Contents/MacOS/$exe"
done
sh util/make-info-plist.sh "$CHROME_ROOT" "$outdir/GLUT Sample.app" glut_input org.berkelium.GLUTSample &&
chmod -R a+rX "$outdir"
archivename="${outdir}-$(date +%Y-%m-%d).tgz"
echo "Tarring into $archivename ..."
tar -zcf "$archivename" "$outdir" >/dev/null
echo "Done!"
