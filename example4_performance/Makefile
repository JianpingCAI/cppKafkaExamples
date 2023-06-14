# Makefile

CXX=g++
CXXFLAGS=-std=c++14 -Wall -Iinclude -I$(PROTO_GENDIR)
LIBS=-lrdkafka++ -lrdkafka

SRCDIR=src
OBJDIR=obj
SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJECTS=$(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Proto files
PROTO_DIR=protos
PROTO_GENDIR=$(PROTO_DIR)/generated
PROTO_OBJDIR=obj/protos
PROTO_FILES=$(wildcard $(PROTO_DIR)/*.proto)
PROTO_GENSRC=$(patsubst $(PROTO_DIR)/%.proto,$(PROTO_GENDIR)/%.pb.cc,$(PROTO_FILES))
PROTO_OBJECTS=$(patsubst $(PROTO_DIR)/%.proto,$(PROTO_OBJDIR)/%.pb.o,$(PROTO_FILES))

TARGETS=kafka_producer kafka_consumer

all: $(TARGETS)

# all: $(OBJDIR) $(PROTO_OBJDIR) $(PROTO_GENDIR) $(TARGETS)

# $(OBJDIR):
# 	mkdir -p $(OBJDIR)

# $(PROTO_OBJDIR):
# 	mkdir -p $(PROTO_OBJDIR)

# $(PROTO_GENDIR):
# 	mkdir -p $(PROTO_GENDIR)

$(PROTO_OBJDIR)/%.pb.o: $(PROTO_DIR)/%.proto
	mkdir -p $(PROTO_OBJDIR)
	mkdir -p $(PROTO_GENDIR)
	protoc -I=$(PROTO_DIR) --cpp_out=$(PROTO_GENDIR) $<
	$(CXX) $(CXXFLAGS) -c $(patsubst $(PROTO_OBJDIR)/%.o,$(PROTO_GENDIR)/%.cc,$(PROTO_GENSRC)) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(PROTO_OBJECTS)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

kafka_producer: $(OBJDIR)/kafka_producer.o $(OBJDIR)/RoundRobinPartitioner.o $(OBJDIR)/MyEventCb.o $(PROTO_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) -lprotobuf

kafka_consumer: $(OBJDIR)/kafka_consumer.o $(OBJDIR)/MyEventCb.o $(PROTO_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS) -lprotobuf

clean:
	rm -rf $(TARGETS) $(OBJDIR) $(PROTO_GENDIR)
