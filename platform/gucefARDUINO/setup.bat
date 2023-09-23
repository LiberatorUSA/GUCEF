@echo off
SET GUCEF_THEBATCHDIR=%~dp0

arduino-cli version
arduino-cli config init
arduino-cli core update-index

echo --- Installing toolchains for boards
echo ---
arduino-cli core install arduino:avr
arduino-cli core install arduino:samd
echo ---

echo --- Installing libaries needed for GUCEF
echo ---
arduino-cli lib install "Arduino_BuiltIn"
arduino-cli lib install "ArduinoSTL"
echo ---

call "%GUCEF_THEBATCHDIR%setup_symlinks.bat"

echo --- Listing all installed libaries
echo ---
arduino-cli lib list --all
echo ---

echo --- CLI supported boards:
echo ---
arduino-cli board listall
echo ---

echo --- Connected boards:
echo ---
arduino-cli board list
echo ---