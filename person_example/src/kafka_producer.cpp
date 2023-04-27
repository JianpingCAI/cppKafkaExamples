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

  // // Serialize the Person object to a string
  // string serialized_data;
  // person.SerializeToString(&serialized_data);

  // Serialize the Person object to a byte vector
  std::vector<uint8_t> serialized_data(person.ByteSizeLong());
  person.SerializeToArray(serialized_data.data(), serialized_data.size());

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

/*
Regarding the difference between using std::string and std::vector<uint8_t> for serialization, both types can store the binary data produced by Google's Protocol Buffers serialization. The main difference is that std::string is typically used for representing text, while std::vector<uint8_t> is a more appropriate data structure for representing a sequence of bytes.

Using std::vector<uint8_t> to store binary data makes the code more explicit about the type of data being stored and avoids confusion about whether the data is text or binary. It also avoids potential issues with null bytes, which can be present in binary data but might cause problems with some string-based functions that expect null-terminated strings.

The choice of data structure for serialized data depends on your requirements and whether you prioritize clarity or compatibility with existing code. In general, it is a good practice to use the most appropriate data type for the data you are working with, which would be std::vector<uint8_t> for binary data. However, if you need to interoperate with existing code that uses std::string for serialized data, you can continue to use std::string.
*/
