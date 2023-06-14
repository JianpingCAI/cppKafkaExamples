CXX = g++
CXXFLAGS = -Wall -Wextra

ifdef DEBUG
CXXFLAGS += -g
endif

LIBS = -lcppkafka -lrdkafka -lprotobuf

# Build directory
BUILD_DIR = build/Debug

# Object files
KAFKA_PRODUCER_OBJ = $(BUILD_DIR)/kafka_producer.o
KAFKA_CONSUMER_OBJ = $(BUILD_DIR)/kafka_consumer.o

PERSON_OBJ = $(BUILD_DIR)/person.pb.o

all: $(BUILD_DIR) kafka_producer kafka_consumer

kafka_producer: $(BUILD_DIR) $(KAFKA_PRODUCER_OBJ) $(PERSON_OBJ) 
	$(CXX) $(CXXFLAGS) -o kafka_producer $(KAFKA_PRODUCER_OBJ) $(PERSON_OBJ) $(LIBS)

kafka_consumer: $(BUILD_DIR) $(KAFKA_CONSUMER_OBJ) $(PERSON_OBJ)
	$(CXX) $(CXXFLAGS) -o kafka_consumer $(KAFKA_CONSUMER_OBJ) $(PERSON_OBJ) $(LIBS)

$(KAFKA_PRODUCER_OBJ): src/kafka_producer.cpp src/person.pb.h
	$(CXX) $(CXXFLAGS) -c src/kafka_producer.cpp -o $@

$(KAFKA_CONSUMER_OBJ): src/kafka_consumer.cpp src/person.pb.h
	$(CXX) $(CXXFLAGS) -c src/kafka_consumer.cpp -o $@

$(PERSON_OBJ): src/person.pb.cc src/person.pb.h
	$(CXX) $(CXXFLAGS) -c src/person.pb.cc -o $@

src/person.pb.h src/person.pb.cc: protos/person.proto
	protoc -I=protos --cpp_out=src protos/person.proto

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f kafka_producer kafka_consumer src/person.pb.h src/person.pb.cc
