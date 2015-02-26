#!/bin/bash
if [ $# -lt 2 ]; then
	echo "use: \"$0 file1 file2\""
else
	root -l "compareOuter.cc(\"$1\",\"$2\")"
fi

