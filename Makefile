#
#  File: Makefile
#  Project: Assignment #1 for ENEL323 (Memory Game)
#  Brief: Makefile for memgame.
#  Author: Aydin Arik (aar52, 19473818)
#  Last Modified: 21/04/11
# 
#  Additional Notes: 
#	-Game has been tested and runs fine on Ubuntu 10.04 (32bit) and Ubuntu 10.10 (64bit)
#

# Definitions
TARGET = memgame
CC = gcc
CFLAGS = -Wall 
GLUTFLAG = -lglut
OPENGLFLAGS = -lGL -lGLU
DEL = rm

# Default target
all: $(TARGET).exe

# Compile: Create object files from C source files
memgamecontroller.o: memgamecontroller.c memgamecontroller.h memgameview.h memgamemodel.h global.h
	$(CC) -c $(CFLAGS) $< -o $@

memgamemodel.o: memgamemodel.c memgamemodel.h memgameview.h memgamecontroller.h global.h
	$(CC) -c $(CFLAGS) $< -o $@

memgameview.o: memgameview.c memgameview.h memgamemodel.h memgamecontroller.h cardchars.h global.h
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).exe: memgamecontroller.o memgamemodel.o memgameview.o 
	$(CC) $(CFLAGS) $^ -o $@ $(GLUTFLAG) $(OPENGLFLAGS) 
    

# Remove old files
.PHONY: clean
clean:
	$(DEL) -f *.o *.exe 
