.PHONY: all clean run

# Define required raylib variables
PROJECT_NAME       ?= game
RAYLIB_VERSION     ?= 4.5.0
RAYLIB_PATH        ?= ..

# Define compiler path on Windows
COMPILER_PATH      ?= C:/raylib/w64devkit/bin

# Define default options
PLATFORM           ?= PLATFORM_DESKTOP

# Locations of raylib library and associated headers
DESTDIR            ?= /usr/local
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

# Define library type for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE     ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE         ?= RELEASE

# Use external GLFW library instead of rglfw module
USE_EXTERNAL_GLFW  ?= FALSE

# Use Wayland display server protocol on Linux desktop
USE_WAYLAND_DISPLAY ?= FALSE

# Define compiler
CC = g++

# Define compiler flags
CFLAGS = -Wall -std=c++11

# Define library paths and flags
LDFLAGS = -L$(RAYLIB_PATH)/src

# Define source files
SRCS = main.cpp spaceship.cpp game.cpp laser.cpp block.cpp obstacle.cpp alien.cpp mysteryship.cpp

# Define the executable
EXEC = $(PROJECT_NAME)

# Default target entry
all: $(EXEC)

# Project target
$(EXEC): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -lraylib

# Clean everything
clean:
	rm -f $(EXEC)

# Run the game
run: $(EXEC)
	./$(EXEC)
