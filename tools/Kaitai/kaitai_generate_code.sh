#!/bin/bash
#
# Turn on tracing, we want to see what's going on
#set -x

# Provide easy access to the location of the script dir itself
scriptPath="$(cd "${0%/*}" 2>/dev/null; echo "$PWD"/"${0##*/}")"
scriptDir=${scriptPath%/*}

#  Invocation of the pre-installed Kaitai compiler
#

kaitai-struct-compiler --version
kaitaiCodeOutDir=${scriptDir}/code/
mkdir ${kaitaiCodeOutDir}

kaitai-struct-compiler ./gucef_core_dynamic.ksy ./gucef_core_variant.ksy --target all --outdir ${kaitaiCodeOutDir} --dotnet-namespace GUCEF
kaitai-struct-compiler ./gucef_pubsub_msgcontainer_header.ksy ./gucef_pubsub_msgcontainer_footer.ksy ./gucef_pubsub_msgcontainer.ksy --target all --outdir ${kaitaiCodeOutDir} --dotnet-namespace GUCEF

