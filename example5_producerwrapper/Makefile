# Makefile for KafkaProducerWrapper

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Include path for librdkafka
INCLUDE_PATH = -I/path/to/librdkafka/include

# Library path for librdkafka
LIBRARY_PATH = -L/path/to/librdkafka/lib

# Libraries to link
LDLIBS = -lrdkafka++ -lrdkafka

# Source files
SOURCES = KafkaProducerWrapper.cpp main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = KafkaApp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBRARY_PATH) $(LDLIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
