#include "KafkaProducerWrapper.h"

int main() {
    KafkaProducerWrapper producerWrapper;

    // Initialize the producer with broker list and client ID
    if (!producerWrapper.Initialize("localhost:9092", "client1")) {
        return 1; // Initialization failed
    }

    // Add topics
    producerWrapper.AddTopic("topic1");
    producerWrapper.AddTopic("topic2");

    // Produce a message to topic1
    if (!producerWrapper.ProduceMessage("topic1", "Hello, Kafka!")) {
        return 1; // Failed to produce message
    }

    // Optional: Remove a topic
    producerWrapper.RemoveTopic("topic2");

    return 0;
}
