BUILDING AND RUNNING UNDER CLION 2019 
---
#### UBUNTU

1. Edit build configuration working directory to the path with res/
2. In Settings -> Toolchains, set CMake to 'cmake_python.py' file

#### WINDOWS

1. Edit build configuration working directory to the path with res/
2. In Settings -> Toolchains, set CMake to 'cmake_python.py' file
3. Make sure 'cmake_python.py' file has the correct path to ninja executable

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
