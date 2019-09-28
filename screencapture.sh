#!/bin/bash 
while true
do
  	#screencapture -x screen.png
	#/usr/local/Cellar/imagemagick/7.0.8-66/bin/convert -resize 320x240\! -strip screen.png BMP3:screen.bmp
	
  	#screencapture -x -R170,-275,320,240 screen.png
	#echo "frame"
	#screencapture -x -R0,-480,640,480 screen.png
	screencapture -x -R0,-240,320,240 screen.png
	/usr/local/Cellar/imagemagick/7.0.8-66/bin/convert -resize 320x240\! -strip screen.png BMP3:screen.bmp
	#./duo "\i"
done
