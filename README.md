# self-learning-car     
Reinforcement learning for steering control of car     

This repository contains modified version of ![TORCS](https://sourceforge.net/projects/torcs)    
TORCS is an open racing car simulator which can be programmed in C++. We can create and program car(s) in TORCS to perform different tasks. In this repository, I have created a car named "ROBO" which performs self learning task on a given track.   
We used Q-learning for training our self-learning car. With a total of 200 `states` and 7 `actions`, the car is able to predict steering values accurately. 

# Dependencies    
1. Install following dependecies:    
    `sudo apt-get install build-essential libxmu-dev libxmu6 libxi-dev libxine-dev libalut-dev freeglut3 freeglut3-dev cmake libogg-dev libvorbis-dev libxxf86dga-dev libxxf86vm-dev libxrender-dev libxrandr-dev zlib1g-dev libpng12-dev`    
2. Download ![PLIB 1.8.5](http://plib.sourceforge.net)     
   `tar xfvz /path/plib-1.8.5*`        
   `cd plib-1.8.5`    
   `./configure CFLAGS="-fPIC" CXXFLAGS="-fPIC" CPPFLAGS="-fPIC"`     
   `make`     
   `sudo make install`         
3. Download ![OpenAL](http://kcat.strangesoft.net/openal.html)    
   `tar xfvj /path/openal-soft-1.17.2*`    
   `cd openal-soft-1.17.2`    
   `cd build`    
   `cmake ..`     
   `make`     
   `sudo make install`
4. Download ![FreeGlut](http://freeglut.sourceforge.net)     
   `tar xfvz /path/freeglut-3.0.0*`     
   `cd freeglut-3.0.0`    
   `cmake .`    
   `make`     
   `sudo make install`    
5. Download/clone this repository    
   `git clone https://github.com/2vin/self-learning-car.git`    
   `cd self-learning-car`    
   `./configure`     
   `make`    
   `sudo make install`    
   `sudo make datainstall`    
6. Add following lines in ~/.bashrc file     
    `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib     
    export TORCS_BASE=path_where_repo_is_saved     
    export MAKE_DEFAULT=$TORCS_BASE/Make-default.mk`      

# Compile       
Go to `self-driving-car/src/drivers` directory and run the following command:    
`make`    
`make install`    
(Note: In order to train the robot, go to ROBO.cpp and change value of `TRAINACTIVE` & `STARTACTIVE` to `true`. For testing mode, set these values to false)    

# Usage    
In new terminal, type the following command:
`torcs`    
This will open the simulator in new window. In order to see the self-learning-car in action, go to `Race` -> `Practice` -> `Configure Race`, and see select 'ROBO' from the list. Then, start `New Race` in `Practice` to see the car running on track.

