# chimarrao

## Setup on Windows with MinGW
1. install MinGW 7.3.0
2. download SFML 2.5.1 for MinGW 7.3.0 and unzip in directory C:\libraries\sfml
3. download Boost 1.71 and unzip in directory C:\libraries\boost
4. add path to sfml binaries directory to PATH and restart computer


## Setup on Ubuntu 20.04
1. Install SFML:

       sudo apt install libsfml-dev

2. Install boost:

       sudo apt install libboost-all-dev   
       
3. Get repository:

       git clone https://github.com/walter-strazak/chimarrao-platformer.git
       
4. Inside chimarrao directory initialize third-party libraries:

       git submodule update --init --recursive
 
  
