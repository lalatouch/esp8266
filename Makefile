# Target app
TARGET = app

# Sources
SRCS = $(shell find . -name '*.cpp')

# Associated Objects
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

# Use clang
CXX = clang++

# Libs to load
LDLIBS = -lm

# C++ Flags
CXXFLAGS = -std=c++14 -Wall -O3 -g

# For linker to understand it's C++
LINK.o=$(LINK.cpp)

# Associate target app with its o file 
$(TARGET): $(OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

# Debug it
debug: $(TARGET)

# Cleanup
clean:
	$(RM) *~ *.gch  $(TARGET) $(OBJS)

.PHONY: clean
