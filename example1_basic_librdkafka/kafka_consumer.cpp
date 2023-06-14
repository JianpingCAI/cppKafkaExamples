#include <iostream>
#include <string>
#include <memory>
#include <librdkafka/rdkafkacpp.h>

class ExampleEventCb : public RdKafka::EventCb
{
public:
    void event_cb(RdKafka::Event &event)
    {
        switch (event.type())
        {
        case RdKafka::Event::EVENT_LOG:
            fprintf(stderr, "LOG-%i-%s: %s\n",
                    event.severity(), event.fac().c_str(), event.str().c_str());
            break;

        case RdKafka::Event::EVENT_ERROR:
            fprintf(stderr, "ERROR (%s): %s\n",
                    RdKafka::err2str(event.err()).c_str(),
                    event.str().c_str());
            break;

        case RdKafka::Event::EVENT_STATS:
            fprintf(stderr, "\"STATS\": %s\n",
                    event.str().c_str());
            break;

        default:
            fprintf(stderr, "EVENT %d (%s): %s\n",
                    event.type(),
                    RdKafka::err2str(event.err()).c_str(),
                    event.str().c_str());
            break;
        }
    }
};

int main()
{
    std::string brokers = "localhost:9092";
    std::string errstr;
    std::string topic_str = "test";
    std::string group_id = "my_group";

    std::unique_ptr<RdKafka::Conf> conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
    // RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);

    if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    if (conf->set("group.id", group_id, errstr) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    ExampleEventCb ex_event_cb;
    if (conf->set("event_cb", &ex_event_cb, errstr) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    std::unique_ptr<RdKafka::KafkaConsumer> consumer(RdKafka::KafkaConsumer::create(conf.get(), errstr));
    if (!consumer)
    {
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        exit(1);
    }

    std::cout << "Created consumer " << consumer->name() << std::endl;

    RdKafka::ErrorCode err = consumer->subscribe({topic_str});

    if (err)
    {
        std::cerr << "Failed to subscribe to " << topic_str << " : " << RdKafka::err2str(err) << std::endl;
        exit(1);
    }

    while (true)
    {
        RdKafka::Message *msg = consumer->consume(1000);
        switch (msg->err())
        {
        case RdKafka::ERR__TIMED_OUT:
            break;

        case RdKafka::ERR_NO_ERROR:
            /* Real message */
            std::cout << "Read msg at offset " << msg->offset() << std::endl;
            std::cout << "Message: " << static_cast<const char *>(msg->payload()) << std::endl;
            break;

        case RdKafka::ERR__PARTITION_EOF:
            /* Last message */
            break;

        case RdKafka::ERR__UNKNOWN_TOPIC:
        case RdKafka::ERR__UNKNOWN_PARTITION:
            std::cerr << "Consume failed: " << msg->errstr() << std::endl;
            break;

        default:
            /* Errors */
            std::cerr << "Consume failed: " << msg->errstr() << std::endl;
        }

        delete msg;
    }

    consumer->close();
    return 0;
}
