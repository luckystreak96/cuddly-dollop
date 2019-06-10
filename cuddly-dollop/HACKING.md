BUILDING AND RUNNING UNDER CLION 2019 
---
#### UBUNTU (CLion)

1. Edit build configuration working directory to the path with res/
2. In Settings -> Toolchains, set CMake to 'cmake_python.py' file

#### WINDOWS (Visual Studio)

- Install Visual Studio (Preferably VS2019)
- That's all the necessary config, see dependancies below

DEPENDENCIES
---
#### UBUNTU

```bash
sudo apt-get install libdevil-dev libglu1-mesa-dev libglew-dev libalut-dev \
 libopenal-dev libglfw3-dev
```

#### WINDOWS

- Download and install c++ parts of Windows SDK (or equivalent): https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk