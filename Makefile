# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Linker flags
LDFLAGS = -lcppkafka -lrdkafka

# Executable names
PRODUCER_EXEC = kafka_producer
CONSUMER_EXEC = kafka_consumer

all: $(PRODUCER_EXEC) $(CONSUMER_EXEC)

$(PRODUCER_EXEC): kafka_producer.cpp
	$(CXX) $(CXXFLAGS) kafka_producer.cpp -o $(PRODUCER_EXEC) $(LDFLAGS)

$(CONSUMER_EXEC): kafka_consumer.cpp
	$(CXX) $(CXXFLAGS) kafka_consumer.cpp -o $(CONSUMER_EXEC) $(LDFLAGS)

clean:
	rm -f $(PRODUCER_EXEC) $(CONSUMER_EXEC)
