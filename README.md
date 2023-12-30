# Conway's Game of life

Usage
```
./life -f <FILE_OF_COORDINATES>

# When using -r flag, will create a .cells with two words from random-words.txt and generate a simulation using it
./life -r 
```

TODO
- [ ] Allow randomization of cells when no file is provided
- [ ] Go a step further with smart randomization (Possible trained on AI to recongnize patterns that create automatons)
- [ ] Dimensions when opening a file are jenky, place the coordinate field in the middle if possible
- [ ] Wrapping when cells go off screen, toroidal 
