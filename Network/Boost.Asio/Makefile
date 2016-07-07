# the compiler: g++ for C++ program, define as g++ for C++
CXX = g++
CC = gcc
RM = rm -f
CFLAGS = $(CXXFLAGS)
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS  = -Wall -Werror
LDFLAGS = -lboost_system -pthread
SPECIAL_FLAGS = -std=c++11

# the build target executable:
TARGET = helloworld

all: $(TARGET)

helloworld: helloworld.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o
