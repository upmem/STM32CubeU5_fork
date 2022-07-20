#!/bin/bash - 

builddir=`pwd`
FileToPatch=../Core/src/tx_initialize_low_level.S 

echo "Prebuild script to patch $FileToPatch to replace g_pfnVectors by __Vectors if needed"

if [ -f $FileToPatch ]; then
	NeedToPatch=`grep g_pfnVectors $FileToPatch`
	if [ ! -z "$NeedToPatch" ];  then 
		cat $FileToPatch | sed -e "s/g_pfnVectors/__Vectors/" > $FileToPatch.New
		mv $FileToPatch.New $FileToPatch
	fi
fi
   