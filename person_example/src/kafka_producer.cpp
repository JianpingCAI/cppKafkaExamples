#include <iostream>
#include <cppkafka/cppkafka.h>
#include "../include/person.pb.h"

using namespace std;
using namespace cppkafka;

int main() {
  // Initialize the Google Protocol Buffers library
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Create a Person object and set its fields
  Person person;
  person.set_name("John Doe");
  person.set_age(30);

  // Serialize the Person object to a string
  string serialized_data;
  person.SerializeToString(&serialized_data);

  // Set up the Kafka producer configuration
  Configuration config = {
    {"metadata.broker.list", "localhost:9092"},
    {"message.timeout.ms", 5000} // Increase the timeout to 5 seconds
  };

  // Create the Kafka producer
  Producer producer(config);

  // Create and send the message
  MessageBuilder builder("person_topic");
  builder.payload(serialized_data);
  producer.produce(builder);

  // // Flush the producer queue
  // producer.flush();

  // Wait for all messages to be acknowledged
  while (producer.get_out_queue_length() > 0) {
    this_thread::sleep_for(chrono::milliseconds(100));
  }

  // Clean up the Google Protocol Buffers library
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
