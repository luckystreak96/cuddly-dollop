## specify the out file
#LINK_TARGET = cuddly_dollop

# source file locations
#VPATH src/
#vpath %.h res/include/%/


# clean macro
#clean : 
#	rm -f $(REBUILDABLES)
#	echo Clean done

# build all macro
#all : $(LINK_TARGET)
#	echo All done


INCLUDE_DIR := /usr/include 
SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS := -I$(INCLUDE_DIR) -L/usr/lib/x86_64-linux-gnu/ -L/usr/lib64 -I/usr/local/include -L/usr/local/lib -L/usr/X11R6/lib -I/usr/X11R6/include -pthread -ldl -Wl,--no-as-needed -lstdc++fs -lopenal -lm -lIL -lILU -lILUT -lglfw -lGLEW -lGL -lGLU
CPPFLAGS := -g -D_DEBUG -L/usr/lib64 -I/usr/local/include -L/usr/local/lib -L/usr/X11R6/lib -I/usr/X11R6/include -lstdc++fs -pthread -lGLEW -lopenal -lGLU -lGL
CXXFLAGS := -I$(INCLUDE_DIR) -L/usr/lib64 -I/usr/local/include -L/usr/local/lib -L/usr/X11R6/lib -I/usr/X11R6/include -lstdc++fs -pthread  -lopenal -std=c++17 -lGLEW -lGLU -lGL

cuddly_dollop : $(OBJ_FILES)
	g++ -Wall $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
