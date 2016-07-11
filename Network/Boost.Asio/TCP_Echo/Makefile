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
TARGET = tcp_sync_client \
		tcp_sync_server \
		tcp_async_client \
		tcp_async_server

all: $(TARGET)

tcp_sync_client: tcp_sync_client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

tcp_sync_server: tcp_sync_server.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

tcp_async_client: tcp_async_client.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

tcp_async_server: tcp_async_server.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o
