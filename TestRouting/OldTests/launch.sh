#!/bin/bash
if [ $# -lt 2 ]; then
	  echo "use: \"$0 fileName numLayer\""
else
    case "$2" in
        1) root -l "testRouting.cpp(\"$1\",12.,40.)"
            ;;

        2) root -l "testRouting.cpp(\"$1\",24.,75.)"
            ;;

        3) root -l "testRouting.cpp(\"$1\",36.,115.)"
            ;;

        4) root -l "testRouting.cpp(\"$1\",52.,170.)"
            ;;

	      *) echo "numLayer not valid."
            ;;
    esac
fi
