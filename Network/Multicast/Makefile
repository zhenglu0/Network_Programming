# the compiler: g++ for C++ program, define as g++ for C++
CXX = g++
CC = gcc
RM = rm -f
CFLAGS = $(CXXFLAGS)
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS  = -Wall -Werror -D USE_IP_MULTICAST_ALL
LDFLAGS = -lboost_system -pthread
SPECIAL_FLAGS = -std=c++11

# the build target executable:
TARGET = mcreceive \
		mcsend

all: $(TARGET)

mcreceive: mcreceive.c
	$(CC) $(CFLAGS) $< -o $@

mcsend: mcsend.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) *.o
