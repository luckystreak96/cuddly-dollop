BUILDING AND RUNNING UNDER CLION 2019 
---
#### UBUNTU

1. Edit build configuration working directory to the path with res/
2. In Settings -> Toolchains, set CMake to 'cmake_python.py' file

#### WINDOWS (DOESN'T WORK RIGHT NOW)

Windows can't run with ninja and CLion, so we just use the default MakeFile setting.  
**Prerequisites**:
- MinGW (g++, gcc, make)
1. Edit build configuration working directory to the path with res/

DEPENDENCIES
---
#### UBUNTU

```bash
sudo apt-get install libdevil-dev libglu1-mesa-dev libglew-dev libalut-dev \
 libopenal-dev libglfw3-dev
```

#### WINDOWS

1. Install MingW
2. Might be all that's necessary?
