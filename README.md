LowD
=====================
LowD is an infinite exploration game.

## Run
All binaries can be found in the folder *bin*.

### Linux
    ./bin/lowd

### Windows
    bin/lowd.exe

### Mac OS X
Not supported yet.


## Dependencies
- GLEW 1.6.0
- GLFW 3.0.4
- FreeType

## Compilation

### All platforms
    git clone https://github.com/j-a-c/LowD.git
    make

### Linux
    git clone https://github.com/j-a-c/LowD.git
    make linux

### Windows
- Note: you may have to tweak the Makefile for specific dependency locations.

    git clone https://github.com/j-a-c/LowD.git
    make windows

### Mac OS X
Not supported yet.


## TODO

### Features
- Player movement bobbing
- Shaders
- Terrain generation model/algorithm
- Implement Debugger class
- Implement different block colors (top/bot vs sides)
- Implement sky changing colors
- Implement clouds
- Allow config file & dynamic changing for 'constants'

### Optimizations
- Render only faces player can see (create separate display lists per face?)
- Render only chunks the player can see
- Change greedy mesh algo to ignore faces hidden by touching chunks 

### Bugfixes
- Make jumping smoother
- Move FPS to top left
- Add current chunk to debug info
- Redo the interpolation / update main loop
