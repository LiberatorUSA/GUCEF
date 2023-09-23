@echo off

arduino-cli compile --optimize-for-debug --verbose --libraries "ArduinoSTL,gucefARDUINO" --fqbn arduino:avr:uno gucef_sketch/