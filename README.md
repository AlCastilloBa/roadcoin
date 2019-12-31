# roadcoin
## Description
Videogame inspired on old mechanical-based coin operated machines. A coin was guided though a small maze. The player rotates the maze with a handwheel.

The objective of this project is not only recreate these machines, but also make it extensible with several maps stored on files. Additionally, other forces appart from gravity will be recreated, such as springs, black holes, magnets,...

![Screenshot001](/images/screenshots/Screenshot004.png)

## Technical details
The mazes are composed of a set of line segments. Normally these segments will be walls, but other segment types will be available, such as "goal" and "death" line segments.
The maze can be rotated using the keyboard, a mouse or a joystick.
This program uses the SDL2 library, and no additional libraries or engines. Everithing is implemented using simple vector operations and simple mechanics equations.

Currently compiles for GNU/Linux (using gcc), and for Windows (using MinGW).

## Current state
Work in progress. The basic game physics have been implemented. It is minimally playable, but a lot of features are still to be programmed.
