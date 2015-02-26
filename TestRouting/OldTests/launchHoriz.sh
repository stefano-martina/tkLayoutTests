#!/bin/bash
if [ $# -lt 2 ]; then
	  echo "use: \"$0 fileName Radius\""
else
    root -l "testRoutingVert.cpp(\"$1\", 1, 1, new double[1]{$2},new double[1]{$3})"
fi