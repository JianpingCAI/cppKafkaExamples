#include <librdkafka/rdkafkacpp.h>
#include <vector>
#include <memory>
#include "../include/MyEventCb.h"

int main()
{
    std::string brokers = "localhost:9092";
    std::string topic1 = "topic_person";
    std::string topic2 = "orders";
    std::string group_id = "test_group";
    std::string errstr;

    // RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    std::unique_ptr<RdKafka::Conf> conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", group_id, errstr);
    conf->set("max.poll.interval.ms", "300000", errstr); // Set maximum time between poll() invocations
    conf->set("enable.auto.commit", "true", errstr);     // Enable auto-commit for consumer

    // Create consumer instance
    std::unique_ptr<RdKafka::KafkaConsumer> consumer1(RdKafka::KafkaConsumer::create(conf.get(), errstr));

    // Set the event callback and statistics interval for monitoring and tuning
    MyEventCb myEventCb;
    conf->set("event_cb", &myEventCb, errstr);
    conf->set("statistics.interval.ms", "1000", errstr); // Set statistics interval

    // Subscribe to the topic
    std::vector<std::string> topics = {topic1, topic2};
    RdKafka::ErrorCode err = consumer1->subscribe(topics);
    if (err)
    {
        std::cerr << "Failed to subscribe to topic: " << RdKafka::err2str(err) << std::endl;
        exit(1);
    }

    // Consume messages
    while (true)
    {
        std::unique_ptr<RdKafka::Message> msg(consumer1->consume(1000)); // Set timeout to 1000ms
        switch (msg->err())
        {
        case RdKafka::ERR_NO_ERROR:
            std::cout << "Received message: " << static_cast<char *>(msg->payload()) << std::endl;
            break;
        case RdKafka::ERR__TIMED_OUT:
            std::cout << "Time out! No message received in 1000ms" << std::endl;

            // Handle timeouts
            break;
        default:
            std::cerr << "Consume error: " << msg->errstr() << std::endl;
        }
    }

    // Close consumer
    consumer1->close();

    return 0;
}
