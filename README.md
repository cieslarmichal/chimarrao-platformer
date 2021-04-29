# chimarrao

<h3>1. Requirements:</h3>
<ul>
  <li>compiler that supports C++ 20</li>
  <li>git</li>
  <li>cmake</li>
</ul>

## Setup on Windows 10 with MSVC
1. Download [SFML 2.5.1 for MSVC](https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip) and unzip in directory C:\libraries\sfml
2. Download [Boost 1.71](https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.zip) and unzip in directory C:\libraries\boost
3. Add path to SFML binaries directory to PATH and restart computer
4. Get repository:

       git clone https://github.com/walter-strazak/chimarrao-platformer.git
       
5. Inside chimarrao directory initialize third-party libraries:

       git submodule update --init --recursive


## Setup on Ubuntu 20.04
1. Install SFML:

       sudo apt install libsfml-dev

2. Install boost:

       sudo apt install libboost-all-dev   
       
3. Get repository:

       git clone https://github.com/walter-strazak/chimarrao-platformer.git
       
4. Inside chimarrao directory initialize third-party libraries:

       git submodule update --init --recursive
 
  
