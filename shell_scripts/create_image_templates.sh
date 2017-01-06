#!/bin/bash

# For creating directories for all the images and copying them over
#for i in `find -iname '*.gif' -o -iname '*.jpg' -o -iname '*.png' -o -iname '*.jpeg'`
#do
	#filename=`basename $i`
	#directoryname=`basename ${i%.*}`
	#echo $filename
	#echo $directoryname
	
	#mkdir $directoryname
	#mv $filename $directoryname
#done

# For copying all of the activation scripts and manifests to the directories
#for d in `find . -type d`
#do
	#echo $d/activate.chai
	#cp -n activate.chai $d/activate.chai
	#cp -n manifest.xml $d/manifest.xml
#done