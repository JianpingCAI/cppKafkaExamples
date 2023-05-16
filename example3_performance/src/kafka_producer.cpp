#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>
#include "../protos/generated/person.pb.h"
#include "../include/RoundRobinPartitioner.h"
#include "../include/MyEventCb.h"

int main()
{
    std::string brokers = "localhost:9092";
    std::string topic = "test_topic";
    std::string errstr;

    // Create Person object
    Person person;
    person.set_name("John Doe");
    person.set_age(30);

    // Create Kafka producer configuration
    RoundRobinPartitioner rr_Partitioner;
    MyEventCb my_EventCb;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("queue.buffering.max.ms", "100", errstr);
    conf->set("batch.num.messages", "1000", errstr);
    conf->set("partitioner_cb", &rr_Partitioner, errstr);
    conf->set("event_cb", &my_EventCb, errstr);
    conf->set("statistics.interval.ms", "1000", errstr);

    // Create producer instance
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);

    // Produce message
    for (int i = 0; i < 10000; ++i)
    {
        person.set_name("John Doe " + std::to_string(i));
        // Serialize Person object
        std::string payload;
        person.SerializeToString(&payload);

        producer->produce(
            RdKafka::Topic::create(producer, topic, nullptr, errstr),
            RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            const_cast<char *>(payload.data()),
            payload.size(),
            nullptr,
            nullptr);
    }

    // Wait for message delivery
    producer->flush(5000);

    // Clean up
    delete producer;
    delete conf;

    return 0;
}
