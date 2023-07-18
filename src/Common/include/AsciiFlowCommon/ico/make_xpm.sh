#!/bin/bash

rm *.xpm

convert About.png		xpm:About.png.xpm
convert Arrow.png		xpm:Arrow.png.xpm
convert Class.png		xpm:Class.png.xpm
convert Erase.png		xpm:Erase.png.xpm
convert Export.png		xpm:Export.png.xpm
convert Freehand.png	xpm:Freehand.png.xpm
convert Import.png		xpm:Import.png.xpm
convert Line.png		xpm:Line.png.xpm
convert Move.png 		xpm:Move.png.xpm
convert New.png 		xpm:New.png.xpm
convert Prefs.png 		xpm:Prefs.png.xpm
convert Rectangle.png	xpm:Rectangle.png.xpm
convert Redo.png 		xpm:Redo.png.xpm
convert Resize.png		xpm:Resize.png.xpm
convert Text.png 		xpm:Text.png.xpm
convert Undo.png 		xpm:Undo.png.xpm

find . -type f -name "*.xpm" -exec sed -i "s/static char/const char/g" {} \;
find . -type f -name "*.xpm" -exec sed -i "s/_png/_xpm/g" {} \;
