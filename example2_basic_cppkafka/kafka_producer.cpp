#include <iostream>
#include <string>
#include <cppkafka/cppkafka.h>

using namespace std;
using namespace cppkafka;

int main() {
    // Configuration
    Configuration config = {
        {"metadata.broker.list", "localhost:9092"}
    };

    // Create a Kafka producer
    Producer producer(config);

    string message;
    while(message != "exit")
    {
        cout << "Enter message to send: ";
        getline(cin, message);

        // Send the message
        producer.produce(MessageBuilder("test_topic").partition(0).payload(message));
        producer.flush();

        cout << "Message sent: " << message << endl;
    }

    return 0;
}
