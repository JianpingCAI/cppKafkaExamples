#include <iostream>
#include <string>
#include <librdkafka/rdkafkacpp.h>
#include <memory>
#include "../protos/generated/person.pb.h"
#include "../include/RoundRobinPartitioner.h"
#include "../include/MyEventCb.h"
#include "../include/Order.h"

class DeliveryReportCb : public RdKafka::DeliveryReportCb
{
public:
    void dr_cb(RdKafka::Message &message)
    {
        if (message.err() != RdKafka::ERR_NO_ERROR)
        {
            std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
        }
        else
        {
            std::cout << "Message delivered to partition " << message.partition() << " at offset " << message.offset() << std::endl;
        }
    }
};

int main()
{
    try
    {
        std::string brokers = "localhost:9092,localhost:9093";
        std::string topic_person = "topic_person";
        std::string errstr;

        RoundRobinPartitioner rr_Partitioner;
        MyEventCb my_EventCb;

        // Create Kafka producer configuration

        std::unique_ptr<RdKafka::Conf> conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
        if (!conf)
        {
            throw std::runtime_error("Failed to create global configuration");
        }

        // conf->set("bootstrap.servers", brokers, errstr);
        if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK)
        {
            throw std::runtime_error("Failed to set bootstrap.servers: " + errstr);
        }

        DeliveryReportCb dr_cb;
        if (conf->set("dr_cb", &dr_cb, errstr) != RdKafka::Conf::CONF_OK)
        {
            throw std::runtime_error("Failed to set delivery report callback: " + errstr);
        }

        conf->set("queue.buffering.max.ms", "100", errstr);
        conf->set("batch.num.messages", "1", errstr);
        conf->set("partitioner_cb", &rr_Partitioner, errstr);
        conf->set("event_cb", &my_EventCb, errstr);
        conf->set("statistics.interval.ms", "1000", errstr);

        // Create producer instance
        std::unique_ptr<RdKafka::Producer> producer(RdKafka::Producer::create(conf.get(), errstr));
        if (!producer)
        {
            throw std::runtime_error("Failed to create producer: " + errstr);
        }

        std::unique_ptr<RdKafka::Topic> topic1(RdKafka::Topic::create(producer.get(), topic_person, nullptr, errstr));
        if (!topic1)
        {
            throw std::runtime_error("Failed to create topic: " + errstr);
        }

        // Create Person object
        Person person;
        person.set_name("John Doe");
        person.set_age(30);

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
                std::cerr << "Failed to produce message: " << RdKafka::err2str(resp) << std::endl;
            }
            else
            {
                std::cout << "Message produced, waiting for delivery report" << std::endl;
            }

            // Wait for delivery reports
            while (producer->outq_len() > 0)
            {
                producer->poll(1000);
            }
        }

        // // Wait for message delivery
        // producer->flush(5000);

        // std::unique_ptr<RdKafka::Topic> topic2(RdKafka::Topic::create(producer.get(), "orders", nullptr, errstr));

        // Order order;
        // order.customer_id = "customerID123";
        // order.order_id = "orderID456";
        // order.product_ids = {"productID1", "productID2"};
        // order.total_cost = 100.0;

        // // Specify the key as the customer ID
        // std::string key = order.customer_id;

        // std::string value = nlohmann::json(order).dump();
        // RdKafka::ErrorCode resp = producer->produce(
        //     topic2.get(),
        //     RdKafka::Topic::PARTITION_UA,
        //     RdKafka::Producer::RK_MSG_COPY,
        //     const_cast<char *>(value.c_str()), value.size(),
        //     const_cast<char *>(key.c_str()), key.size(),
        //     nullptr);

        // if (resp != RdKafka::ERR_NO_ERROR)
        // {
        //     std::cerr << "Failed to send message: " << RdKafka::err2str(resp) << std::endl;
        // }

        // Ensure all messages are delivered before exiting
        std::cout << "Waiting for all messages to be delivered" << std::endl;
        producer->flush(5000);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
