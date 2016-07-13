# the compiler: g++ for C++ program, define as g++ for C++
CXX = g++
CC = gcc
RM = rm -f
CFLAGS = $(CXXFLAGS)
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS  = -Wall -Werror
LDFLAGS = -lboost_system -pthread -lboost_thread
SPECIAL_FLAGS = -std=c++11

# the build target executable:
TARGET = sync_client \
		sync_server \
		async_client \
		async_server

all: $(TARGET)

sync_client: sync_client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

sync_server: sync_server.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

async_client: async_client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

async_server: async_server.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o
