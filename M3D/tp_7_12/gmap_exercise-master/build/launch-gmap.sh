#!/bin/sh
bindir=$(pwd)
cd /home/e20210007011/Documents/M1/M3D/tp_7_12/gmap_exercise-master/src/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20210007011/Documents/M1/M3D/tp_7_12/gmap_exercise-master/build/gmap 
	else
		"/home/e20210007011/Documents/M1/M3D/tp_7_12/gmap_exercise-master/build/gmap"  
	fi
else
	"/home/e20210007011/Documents/M1/M3D/tp_7_12/gmap_exercise-master/build/gmap"  
fi
