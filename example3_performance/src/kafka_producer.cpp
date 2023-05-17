#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include "../protos/generated/person.pb.h"
#include "../include/RoundRobinPartitioner.h"
#include "../include/MyEventCb.h"
#include "../include/Order.h"

int main()
{
    std::string brokers = "localhost:9092";
    std::string topic_person = "topic_person";
    std::string errstr;

    // Create Person object
    Person person;
    person.set_name("John Doe");
    person.set_age(30);

    // Create Kafka producer configuration
    RoundRobinPartitioner rr_Partitioner;
    MyEventCb my_EventCb;
    std::unique_ptr<RdKafka::Conf> conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));

    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("queue.buffering.max.ms", "100", errstr);
    conf->set("batch.num.messages", "1", errstr);
    conf->set("partitioner_cb", &rr_Partitioner, errstr);
    conf->set("event_cb", &my_EventCb, errstr);
    conf->set("statistics.interval.ms", "1000", errstr);

    // Create producer instance
    std::unique_ptr<RdKafka::Producer> producer(RdKafka::Producer::create(conf.get(), errstr));
    std::unique_ptr<RdKafka::Topic> topic1(RdKafka::Topic::create(producer.get(), topic_person, nullptr, errstr));

    // Produce message
    for (int i = 0; i < 5; ++i)
    {
        person.set_name("John Doe " + std::to_string(i));

        // Serialize Person object
        std::string payload;
        person.SerializeToString(&payload);

        RdKafka::ErrorCode resp = producer->produce(
            topic1.get(),
            RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            const_cast<char *>(payload.data()),
            payload.size(),
            nullptr,
            nullptr);

        if (resp != RdKafka::ERR_NO_ERROR)
        {
            std::cerr << "Failed to send message: " << RdKafka::err2str(resp) << std::endl;
        }
    }

    // Wait for message delivery
    producer->flush(1000);

    std::unique_ptr<RdKafka::Topic> topic2(RdKafka::Topic::create(producer.get(), "orders", nullptr, errstr));

    Order order;
    order.customer_id = "customerID123";
    order.order_id = "orderID456";
    order.product_ids = {"productID1", "productID2"};
    order.total_cost = 100.0;
    std::string value = nlohmann::json(order).dump();
    RdKafka::ErrorCode resp = producer->produce(
        topic2.get(),
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char *>(value.c_str()),
        value.size(),
        nullptr,
        nullptr);

    if (resp != RdKafka::ERR_NO_ERROR)
    {
        std::cerr << "Failed to send message: " << RdKafka::err2str(resp) << std::endl;
    }

    // Wait for message delivery
    producer->flush(5000);

    return 0;
}
