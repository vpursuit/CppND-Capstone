# CPPND: Capstone project of Peter Trebing

This is the Udacity C++ Nanodegree Capstone project of Peter Trebing.

This project is a simplified simulation of the atmosphere and the greenhouse effect. 
It implements a particle system which represents three athmospheric gas molecules: 

- N2, nitrogen (grey particles)
- O2, oxygen  (red particles)
- CO2, carbon dioxide (black, bold particles)

Note: The proportions are not to scale. 

The model shows how the molecules collide and exchange kinetic energy. 
In addition, friction is simulated, i.e. the molecules gradually lose speed until 
they finally come to a standstill after a long time. Over time the atmosphere 'freezes' so to speak.

![](CollisionSim.gif)

## User interaction
The simulation allows the following user inputs:

| Shortcut        | Action                        | 
| :-------------: |:-----------------------------:| 
| H               | Accelerate the CO2 molecules  |
| +               | Add more N2 and O2 molecules  |
| -               | Remove N2 and O2 molecuules   |
| CTRL-Q          | Exits the simulation          |

### Heat up the atmosphere
To heat up the atmosphere, you can accelerate the CO2 molecules 
by pressing the 'H' key. This simulates the irradiation of solar energy. 
The CO2 molecules then collide randomly with N2 and O2 molecules and transfer 
part of their kinetic energy so that the total energy and thus the temperature of the 
gas(atmosphere) rises. Because of the simulated friction between gas molecules, the energy of 
the whole system declines again. Over time the molecules become slower again.

### Add more molecules
You can increase the density of the atmosphere by pressing '+'. This adds new N2 and O2 molecules.
Be careful with this, because it increases the computing effort, 
slows down the simulation and reduces the frame rate.

### Remove molecules
Decrease the atmospheric density by pressing '-'.

### Stop the simulation
By pressing CTRL-Q you can stop the simulation.

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


###Implementation
The simulation of physics and representation is done in separate and independent threads.
