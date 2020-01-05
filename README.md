# roadcoin
## Description
Videogame inspired on old mechanical-based coin operated machines. A coin was guided though a small maze. The player rotates the maze with a handwheel.

The objective of this project is not only recreate these machines, but also make it extensible with several maps stored on files. Additionally, other forces appart from gravity will be recreated, such as springs, black holes, magnets,...

![Screenshot001](/images/screenshots/v0.1-beta.1.png)

## Technical details
The mazes are composed of a set of line segments. Normally these segments will be walls, but other segment types will be available, such as "goal" and "death" line segments.
The maze can be rotated using the keyboard, a mouse or a joystick.
This program uses the SDL2 library, and no additional libraries or engines. Everithing is implemented using simple vector operations and simple mechanics equations.

Currently compiles for GNU/Linux (using gcc), and for Windows (using MinGW).

## Current state
Work in progress. The basic game physics have been implemented. It is minimally playable, but a lot of features are still to be programmed.

## Controls
Game can be controlled with a mouse. Moving mouse left and right rotates the map. 

Additionaly, some keyboard keys can be used:

Key                 | Action
--------------------|----------------------
<kbd>ESC</kbd>      | Go back to menu, or exit game.
<kbd>&larr;</kbd>   | Rotate map left
<kbd>&larr;</kbd>   | Rotate map right
<kbd>P</kbd>        | Pause game
<kbd>F</kbd>        | Toggle fullscreen (menu only)
<kbd>C</kbd>        | Change camera mode
KP<kbd>8</kbd>        | Move camera up
KP<kbd>2</kbd>        | Move camera down
KP<kbd>4</kbd>        | Move camera left
KP<kbd>6</kbd>        | Move camera right




