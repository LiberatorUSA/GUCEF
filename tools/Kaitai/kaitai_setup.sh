#!/bin/bash
#

# Provide easy access to the location of the script dir itself
DIR="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"

#  Installing Kaitai compiler
#
curl -LO https://github.com/kaitai-io/kaitai_struct_compiler/releases/download/0.10/kaitai-struct-compiler_0.10_all.deb
sudo apt-get install ./kaitai-struct-compiler_0.10_all.deb
