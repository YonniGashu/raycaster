# raycaster

Raycaster made in C++

#### Building

##### Unix-like

`$ https://github.com/YonniGashu/raycaster.git`\
`$ make`

Be sure that you have SDL2 installed, that is the only library required:

`brew install sdl2`

If needed, replace the FLAGS in the Makefile with the correct paths on your system.
You can find these paths by running:

`sdl2-config --cflags`\
`sdl2-config --libs`

The game binary, once built with `$ make`, can be found in `./bin/`.

_Be sure_ to run with `$ ./bin/game` out of the root directory of the repository.

##### Windows

good luck 🤷‍♂️
