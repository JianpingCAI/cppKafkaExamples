#include <librdkafka/rdkafkacpp.h>
#include <vector>
#include "../include/MyEventCb.h"

int main()
{
    std::string brokers = "localhost:9092";
    std::string topic = "test_topic";
    std::string group_id = "test_group";
    std::string errstr;

    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    // RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    conf->set("bootstrap.servers", brokers, errstr);
    conf->set("group.id", group_id, errstr);
    conf->set("max.poll.interval.ms", "300000", errstr); // Set maximum time between poll() invocations
    conf->set("enable.auto.commit", "true", errstr);     // Enable auto-commit for consumer

    // Create consumer instance
    RdKafka::KafkaConsumer *consumer = RdKafka::KafkaConsumer::create(conf, errstr);

    // Set the event callback and statistics interval for monitoring and tuning
    MyEventCb myEventCb;
    conf->set("event_cb", &myEventCb, errstr);
    conf->set("statistics.interval.ms", "1000", errstr); // Set statistics interval

    // Subscribe to the topic
    std::vector<std::string> topics = {topic};
    RdKafka::ErrorCode err = consumer->subscribe(topics);
    if (err)
    {
        std::cerr << "Failed to subscribe to topic: " << RdKafka::err2str(err) << std::endl;
        exit(1);
    }

    // Consume messages
    while (true)
    {
        RdKafka::Message *msg = consumer->consume(1000); // Set timeout to 1000ms
        switch (msg->err())
        {
        case RdKafka::ERR_NO_ERROR:
            std::cout << "Received message: " << static_cast<char *>(msg->payload()) << std::endl;
            break;
        case RdKafka::ERR__TIMED_OUT:
            // Handle timeouts
            break;
        default:
            std::cerr << "Consume error: " << msg->errstr() << std::endl;
        }
        delete msg;
    }

    // Close consumer
    consumer->close();
    delete consumer;
    
    return 0;
}
