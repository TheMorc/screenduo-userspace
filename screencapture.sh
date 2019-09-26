
#!/bin/bash
 
while true
do
  	#screencapture -x screen.png
	#/usr/local/Cellar/imagemagick/7.0.8-66/bin/convert -resize 320x240\! -strip screen.png BMP3:screen.bmp
	
  	screencapture -x -R15,-275,340,260 screen.png
	/usr/local/Cellar/imagemagick/7.0.8-66/bin/convert -resize 320x240\! -strip screen.png BMP3:screen.bmp
	./duo "\i"
done
