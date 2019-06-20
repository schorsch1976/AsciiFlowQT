Asciiflow QT Editor
==================

As i got annoyed that [AsciiFlow Infinity](http://asciiflow.com/) or [asciiflow stable](http://stable.ascii-flow.appspot.com/#Draw) is just available when there is internet, i wrote my own QT based application.

[AsciiFlow Infinity](http://asciiflow.com/) now wants me to log in to google. Why should i log in to google to draw some ascii charts?

-----

Purpose:
========
Draw and edit any ascii art diagram. It is mostly used for programming issues to document structure in an header or source as a comment.

-----

Features:
=========
- Export and Import ascii to/from the clipboard
- Move parts of the ascii art
- draw rectangles
- draw class diagramms
- draw arrows
- draw freehand
- add text to any position
- undo and redo any change
- practicly unlimited space. The drawing area grows when you move out of it.

-----

Licence
=======
GPL-3.0

-----

Used Libraries:
===============
QT-5

-----

Building:
=========

Installing requirements:
========================
- apt get install qt5-qmake qtbase5-dev

Building:
=========
- git clone https://github.com/schorsch1976/AsciiFlowQT.git
- cd AsciiFlowQT
- mkdir build
- cd build
- qmake ../src/asciiFlow.pro
- make

Just start it by ./AsciiFlow
