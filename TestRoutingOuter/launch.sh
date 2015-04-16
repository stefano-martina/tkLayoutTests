#!/bin/bash
if [ $# -lt 1 ]; then
	echo "use: \"$0 testDir\""
else
	root -l "macro.cpp(\"$1\")"
fi

