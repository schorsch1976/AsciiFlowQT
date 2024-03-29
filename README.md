Asciiflow QT/Wx Editor
===================

Purpose:
========
Draw and edit any ascii art diagram.
It is mostly used for programming issues to document structure in an header or source as a comment.

Why an offline Ascii Flow Editor?
================================

As i got annoyed that [AsciiFlow Infinity](http://asciiflow.com/) or [asciiflow stable](http://stable.ascii-flow.appspot.com/#Draw) is just available when there is internet, i wrote my own QT/Wx based application.

[AsciiFlow Infinity](http://asciiflow.com/) now wants me to log in to google. Why should i log in to google to draw some ascii charts?

Features:
=========
- export and import ascii to/from the clipboard
- export and import ascii to/from a file
- move parts of the ascii art
- draw rectangles
- resize rectangles
- draw class diagramms
- draw arrows
- draw lines
- draw freehand
- add text to any position
- undo and redo any change
- practically unlimited space. The drawing area grows when you move out of it.

License
=======
GPL-3.0

Used Libraries:
===============
QT-5 and/or wxWidgets

Building:
=========

Installing requirements:
========================
- apt get install qtbase5-dev libwxgtk3.2-dev cmake g++

Building:
=========
- git clone https://github.com/schorsch1976/AsciiFlowQT.git
- cd AsciiFlowQT
- mkdir build
- cd build
- cmake ..
- make

Just start it by ./src/qt/AsciiFlow or ./src/wx/AsciiFlow
