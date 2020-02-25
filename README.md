# roadcoin
## Description
Videogame inspired on old mechanical-based coin operated machines. A coin was guided though a small maze. The player rotates the maze with a handwheel.

The objective of this project is not only recreate these machines, but also make it extensible with several maps stored on files. Additionally, other forces appart from gravity will be recreated, such as springs, black holes, magnets,...

![Screenshot001](/images/screenshots/v0.1-beta.1.png)

## Technical details
The mazes are composed of a set of line segments. Normally these segments will be walls, but other segment types will be available, such as "goal" and "death" line segments. Additional elements have been included, such as bumpers and flippers so far.
The maze can be rotated using the keyboard, a mouse or a joystick.
This program uses the SDL2 library, and no additional libraries or engines. Everything is implemented using simple vector operations and simple mechanics equations.

The following libraries are needed:
* SDL2
* SDL2 image
* SDL2 ttf
* SDL2 mixer

Currently compiles for GNU/Linux (using gcc), and for Windows (using MinGW).
* To compile for GNU/Linux with gcc, use `make`.
* To compile for Windows with MinGW, use `mingw32-make.exe -f ./Makefile_windows_minGW`, but check "include" and "lib" paths first. Additionally, an "exe" is included (remember SDL2, SDL2image, SDL2ttf, SDL2mixer dll libraries are needed).

## Current state
Work in progress. The basic game physics have been implemented. It is minimally playable, but a lot of features are still to be programmed.

## Controls
Game can be controlled with a mouse. Moving mouse left and right rotates the map.
Pinball flippers (when available), can be controlled using left and right mouse buttons.

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


## Additional information
The music used in this project is temporary and only used for testing purposes. In order to avoid copyright issues, it is not uploaded. Feel free to use your own music.


