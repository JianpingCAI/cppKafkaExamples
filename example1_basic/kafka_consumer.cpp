#include <iostream>
#include <string>
#include <cppkafka/cppkafka.h>

using namespace std;
using namespace cppkafka;

int main() {
    // Configuration
    Configuration config = {
        {"metadata.broker.list", "localhost:9092"},
        {"group.id", "test_group"},
        {"enable.auto.commit", false}
    };

    // Create a Kafka consumer
    Consumer consumer(config);
    consumer.subscribe({"test_topic"});

    // Process messages
    while (true) {
        Message msg = consumer.poll(std::chrono::milliseconds(100));
        if (msg) {
            if (msg.get_error()) {
                cout << "Error while consuming: " << msg.get_error() << endl;
            } else {
                cout << "Received message: " << msg.get_payload() << endl;
            }
        }
    }

    return 0;
}
