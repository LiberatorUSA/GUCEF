#!/bin/sh

CHROME_ROOT="$1"
appdir="$2"
exe="$3"
appname="`echo "$appdir" | sed -e 's/\.app//'`"
appname="`basename "$appname"`"
bundleid="$4"

eval `cat $CHROME_ROOT/src/chrome/VERSION` &&
versionnum=$MAJOR.$MINOR.$BUILD.$PATCH
majorversion=$MAJOR.$MINOR

cat > "$appdir/Contents/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>$exe</string>
	<key>CFBundleIdentifier</key>
	<string>$bundleid</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>$appname</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>$versionnum</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>CFBundleVersion</key>
	<string>$majorversion</string>
	<key>LSFileQuarantineEnabled</key>
	<true/>
	<key>LSHasLocalizedDisplayName</key>
	<string>1</string>
	<key>LSMinimumSystemVersion</key>
	<string>10.5.0</string>
	<key>LSUIElement</key>
	<string>1</string>
</dict>
</plist>
EOF

