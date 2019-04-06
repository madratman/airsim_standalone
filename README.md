# airsim_standalone
example standalone repo to help link with AirSim   
tested with gcc 5.4.0.

## Build
- [Build AirSim on linux](https://github.com/Microsoft/AirSim/blob/master/docs/build_linux.md)
-  Set path to AirSim root folder in [this line](https://github.com/madratman/airsim_standalone/blob/master/CMakeLists.txt#L4). 
- Build it!
  ```
  mkdir build && cd $_;
  cmake ..
  make -j`nproc`
  ```
