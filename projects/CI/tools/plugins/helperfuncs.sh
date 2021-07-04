#!/bin/bash

set -e

# Functions

if ! type -t readarray >/dev/null; then
  # Very minimal readarray implementation using read. Does NOT work with lines that contain double-quotes due to eval()
  readarray() {
    local cmd opt t v=MAPFILE
    while [ -n "$1" ]; do
      case "$1" in
      -h|--help) echo "minimal substitute readarray for older bash"; exit; ;;
      -r) shift; opt="$opt -r"; ;;
      -t) shift; t=1; ;;
      -u) 
          shift; 
          if [ -n "$1" ]; then
            opt="$opt -u $1"; 
            shift
          fi
          ;;
      *)
          if [[ "$1" =~ ^[A-Za-z_]+$ ]]; then
            v="$1"
            shift
          else
            echo -en "${C_BOLD}${C_RED}Error: ${C_RESET}Unknown option: '$1'\n" 1>&2
            exit
          fi
          ;;
      esac
    done
    cmd="read $opt"
    eval "$v=()"
    while IFS= eval "$cmd line"; do      
      line=$(echo "$line" | sed -e "s#\([\"\`]\)#\\\\\1#g" )
      eval "${v}+=(\"$line\")"
    done
  }
fi


##
# Loads lines from a text file and stores them in an array
#
# Input:
#    PATHTOTEXTFILE - path to the file you wish to read as an array of lines
#	 __resultvar    - output variable to hold the array data
#
function load_text_file_lines_as_array {
    local PATHTOTEXTFILE=$1
	local target="$2"
	local -a array
	readarray -t array < $PATHTOTEXTFILE # Exclude newline with -t 
	read -a "${target}" < <( echo "${array[@]}" )
}
