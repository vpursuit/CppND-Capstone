# CPPND: Capstone project of Peter Trebing

This is the Udacity C++ Nanodegree Capstone project of Peter Trebing.

This project is a simplified simulation of the atmosphere and the greenhouse effect. 
It implements a particle system which represents three athmospheric gases molecules N2, O2 and CO2. 

![](CollisionSim.gif)

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./CollisionSim`.

## USAGE

This simulation allows the following user inputs:

| Shortcut        | Action                        | 
| :-------------: |:-----------------------------:| 
| H               | Accelerate the CO2 molecules  |
| +               | Add more N2 and O2 molecules  |
| -               | Remove N2 and O2 molecuules   |
| CTRL-Q          | Exits the simulation          |

###Implementation
The simulation of physics and representation is done in separate and independent threads.
