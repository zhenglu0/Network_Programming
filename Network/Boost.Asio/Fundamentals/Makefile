# the compiler: g++ for C++ program, define as g++ for C++
CXX = g++
CC = gcc
RM = rm -f
CFLAGS = $(CXXFLAGS)
# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS  = -g -Wall -Werror
LDFLAGS = -lboost_system -pthread -lboost_thread
SPECIAL_FLAGS = -std=c++11

# the build target executable:
TARGET = end_points \
		async_work \
		async_strand \
		post_dispatch \
		async_wrap \
		keep_alive

all: $(TARGET)

end_points: end_points.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

async_work: async_work.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

async_strand: async_strand.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

post_dispatch: post_dispatch.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

async_wrap: async_wrap.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

keep_alive: keep_alive.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o
