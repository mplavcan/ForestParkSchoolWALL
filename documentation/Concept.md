Board Design
============

Board concept was created by [Travis Feldman](http://travisfeldman.org) in June 2014

The board consists of an artistic map of the northwest corner of the state of Oregon, laid out using hexagons:

![Board front showing hexes](/pictures/Board_Hexes.jpg)

On the left (ocean) side are a "battery positive" terminal and six input types:
* Rotary potentiometer (knob)
* Linear potentiometer (slider)
* Photosensors (three)
* Digital (arcade-style) joystick
* Toggle switches (three)
* Force sensors (three)

On the right (mountain) side are "battery negative" terminal and six outputs:
* LED array (white)
* Motor (blue spinner)
* LED array (red)
* Motor (red spinner)
* LED array (green)
* Transducer (speaker)

The board also has five large illuminated colored buttons (Red, Green, White, Blue, Yellow) that are associated with electroluminescent wire wrapped under the hexes.  

A LCD with a selectable color backlight occupies a hex in the upper-left corner of the board.

The circuits allow any one of the input types to be connected to one of the outputs using a series of wooden blocks to bridge across the intervening hexes.  When a full connection between the positive/input/output/negative terminals are made, the input will manipulate the output.

All components are software controllable by the Arduino, independent of each other.  While the students appear to be creating circuits, in fact the software is making the decision about what to activate.  This also prevents accidental damage from being done by "short circuits"



