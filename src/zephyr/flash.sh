#!/bin/bash
if [ -d build ]; then
	cd build
#	ninja flash
	west flash
	cd ..
else
	echo build missing !!!
fi
echo -n "press any key..."
read
