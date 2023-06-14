#include <iostream>
#include <string>
#include <memory>
#include <librdkafka/rdkafkacpp.h>

int main()
{
    std::string brokers = "localhost:9092";
    std::string errstr;
    std::string topic_str = "test";
    std::string message = "Hello, World!";

    std::unique_ptr<RdKafka::Conf> conf(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));

    if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK)
    {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    std::unique_ptr<RdKafka::Producer> producer(RdKafka::Producer::create(conf.get(), errstr));
    if (!producer)
    {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        exit(1);
    }

    std::unique_ptr<RdKafka::Topic> topic(RdKafka::Topic::create(producer.get(), topic_str, nullptr, errstr));

    RdKafka::ErrorCode resp = producer->produce(
        topic.get(),
        RdKafka::Topic::PARTITION_UA,
        RdKafka::Producer::RK_MSG_COPY,
        const_cast<char *>(message.c_str()),
        message.size(),
        nullptr,
        nullptr);

    if (resp != RdKafka::ERR_NO_ERROR)
    {
        std::cerr << "Failed to produce message: " << RdKafka::err2str(resp) << std::endl;
    }
    else
    {
        std::cout << "Message produced successfully" << std::endl;
    }

    producer->poll(0);

    while (producer->outq_len() > 0)
    {
        producer->poll(1000);
    }

    return 0;
}
