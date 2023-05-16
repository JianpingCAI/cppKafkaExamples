# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra
# Linker flags
LDFLAGS = -lcppkafka -lrdkafka

OBJDIR = obj

# Executable names
PRODUCER_EXEC = kafka_producer
CONSUMER_EXEC = kafka_consumer

all: $(OBJDIR) $(PRODUCER_EXEC) $(CONSUMER_EXEC)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(PRODUCER_EXEC): $(OBJDIR)/kafka_producer.o
	$(CXX) $(CXXFLAGS) $(OBJDIR)/kafka_producer.o -o $(PRODUCER_EXEC) $(LDFLAGS)

$(CONSUMER_EXEC): $(OBJDIR)/kafka_consumer.o
	$(CXX) $(CXXFLAGS) $(OBJDIR)/kafka_consumer.o -o $(CONSUMER_EXEC) $(LDFLAGS)

clean:
	rm -f $(PRODUCER_EXEC) $(CONSUMER_EXEC)
	rm -rf $(OBJDIR)
