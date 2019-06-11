BUILDING AND RUNNING CUDDLY-DOLLOP
----------------------------------


#### UBUNTU (CLion)

1. Edit build configuration working directory to the path with res/
2. In Settings -> Toolchains, set CMake to 'cmake_python.py' file


#### WINDOWS (Visual Studio)

- Install Visual Studio (Preferably VS2019)
- Open the .sln and enjoy.

##### IF THE VS PROJECT FILES ARE NOT UP TO DATE OR DON'T WORK...

- Open git bash and cd to the project dir (the one with src/ in it):

```bash
mkdir vs_build
cd vs_build
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -G "Visual Studio 16 2019" -A Win32 ../
```

Where "Visual Studio 16 2019" is the VS generator, -A Win32 says "build 32-bit", the ../ means "CMakeLists.txt is here" and
the first part is where to find CMake. This part is probably easier to do in linux.

- That command creates a .sln file to start working with Visual Studio
- Build once - then copy the 32-bit dll files to .../Debug/
- Right click project -> properties -> debugging -> working directory -> ${Project}/..
  - This makes it so res/ and shaders/ are found without having to copy-paste them around


##### So what's up with BUILD_ALL and ZERO_CHECK projects?

According to https://cmake.org/pipermail/cmake/2008-November/025448.html :

```
ZERO_CHECK will rerun cmake. You can/should execute this after changing something on your CMake files.
ALL_BUILD is simply a target which builds all and everything project in the active solution, I guess one can compare it to "make all".
```



DEPENDENCIES
------------


#### UBUNTU

```bash
sudo apt-get install libdevil-dev libglu1-mesa-dev libglew-dev libalut-dev \
 libopenal-dev libglfw3-dev
```


#### WINDOWS

- Download and install Windows SDK:
	- **Comes bundled with Visual Studio**, or...
    	- ...get this or equivalent SDK: https://developer.microsoft.com/en-US/windows/downloads/windows-10-sdk
