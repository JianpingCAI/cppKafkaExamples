#include <iostream>
#include <cppkafka/cppkafka.h>
#include "../include/person.pb.h"

using namespace std;
using namespace cppkafka;

int main() {
  // Initialize the Google Protocol Buffers library
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Set up the Kafka consumer configuration
  Configuration config = {
    {"metadata.broker.list", "localhost:9092"},
    {"group.id", "person_consumer_group"}
  };

  // Create the Kafka consumer
  Consumer consumer(config);

  // Subscribe to the topic
  consumer.subscribe({"person_topic"});

  // Poll for messages
  while (true) {
    Message msg = consumer.poll(std::chrono::milliseconds(1000));
    if (msg) {
      if (msg.get_error()) {
        // Handle errors
        cerr << "Error while consuming message: " << msg.get_error() << endl;
      } else {
        // Deserialize the message payload into a Person object
        Person person;
        person.ParseFromString(msg.get_payload());

        // Display the received Person object
        cout << "Received person: name = " << person.name() << ", age = " << person.age() << endl;
      }
    }
  }

  // Clean up the Google Protocol Buffers library
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}