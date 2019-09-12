###############################################################################
#                                                                             #
# MAKEFILE for vbuild                                                         #
#                                                                             #
# (c) Guy Wilson 2019                                                         #
#                                                                             #
###############################################################################

# Source directory
SOURCE=src

# Build output directory
BUILD=build

# What is our target
TARGET=vbuild

# C compiler
CPP=g++

# Linker
LINKER=g++

# C compiler flags (Release)
CPPFLAGS=-c -Wall -std=c++11

# Object files 
OBJFILES=$(BUILD)/vbuild.o $(BUILD)/currenttime.o

# Target
all: $(TARGET)

# Compile C source files
#
$(BUILD)/currenttime.o: $(SOURCE)/currenttime.cpp $(SOURCE)/currenttime.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/currenttime.o $(SOURCE)/currenttime.cpp

$(BUILD)/vbuild.o: $(SOURCE)/vbuild.cpp $(SOURCE)/currenttime.h
	$(CPP) $(CPPFLAGS) -o $(BUILD)/vbuild.o $(SOURCE)/vbuild.cpp

$(TARGET): $(OBJFILES)
	$(LINKER) -lstdc++ -o $(TARGET) $(OBJFILES)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin
