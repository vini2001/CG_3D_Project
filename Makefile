#Files that will be compiled
OBJS = main.cpp Libraries/glad.cpp src/game/*.cpp src/graphics/*.cpp src/game/objects/

#Used compiler
CC = g++

#Path to aditional files needed to compile
INCLUDE_PATHS = -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/local/include/freetype2/ -I headers/ -I headers/graphics/ -I headers/game/ -I headers/game/objects -I Libraries/lib/ -I Libraries/include/

#Path to libraries needed to compile
LIBRARY_PATHS = -LLibraries/lib/ -lglfw3 -lfreetype -lbz2 -lpng -lpng16 -lz

#Aditional compile options
COMPILER_FLAGS = 

#Specifies which libraries we're linking
LINKER_FLAGS = -I/usr/local/lib -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL

#Executable name
EXE_NAME = demo

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -std=c++17 -o $(EXE_NAME)