
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Include path
INCLUDE_PATH = -I./include

# Linker flags
LDFLAGS = -lcppkafka -lrdkafka -lprotobuf

# Executable names
PRODUCER_EXEC = kafka_producer
CONSUMER_EXEC = kafka_consumer

all: $(PRODUCER_EXEC) $(CONSUMER_EXEC)

$(PRODUCER_EXEC): src/kafka_producer.cpp src/person.pb.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) src/kafka_producer.cpp src/person.pb.cc -o $(PRODUCER_EXEC) $(LDFLAGS)

$(CONSUMER_EXEC): src/kafka_consumer.cpp src/person.pb.cc
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATH) src/kafka_consumer.cpp src/person.pb.cc -o $(CONSUMER_EXEC) $(LDFLAGS)

clean:
	rm -f $(PRODUCER_EXEC) $(CONSUMER_EXEC)
