# Conway's Game of life

Usage
```
Usage: life
Given an initialization file or generating its own starting values, will conduct a Conway's Game of Life simulation.

-h      Display this help message
-r      Will generate random starting cells and save off as a file to be used
-f      Starting values to be used for simulation (If none are supplied, will be randomly generated)
```

Randomly generate a simulation of Conway's Game of life
```
./life -r 
```
![]https://github.com/AlpineGrizzly/Conways_game/blob/main/gifs/life_example.gif

BUGS

TODO
- [ ] Allow input to size of the field
- [ ] Dimensions when opening a file are jenky, place the coordinate field in the middle if possible
- [ ] Wrapping when cells go off screen, toroidal 
- [ ] Simulation field will be displayed in ncurses

FUTURE ADDITIONS
- [ ] Go a step further with smart randomization (Possible trained on AI to recongnize patterns that create automatons)
