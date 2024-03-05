#include "KafkaProducerWrapper.h"
#include <thread>
#include <chrono>

KafkaProducerWrapper::KafkaProducerWrapper()
{
    // Initialization of smart pointers is not required here
}

KafkaProducerWrapper::~KafkaProducerWrapper()
{
    if (producer)
    {
        producer->flush(10000); // Ensure all messages are produced before destruction
    }
    // Smart pointers will automatically clean up
}

bool KafkaProducerWrapper::Initialize(const std::string &brokers, const std::string &client_id)
{
    std::string errstr;

    // Creating configuration object
    conf.reset(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));

    // Set broker list
    if (conf->set("metadata.broker.list", brokers, errstr) != RdKafka::Conf::CONF_OK)
    {
        throw std::runtime_error("Failed to set broker list: " + errstr);
    }

    // Set client id
    if (conf->set("client.id", client_id, errstr) != RdKafka::Conf::CONF_OK)
    {
        throw std::runtime_error("Failed to set client id: " + errstr);
    }

    // // Set security protocol to SASL_SSL to ensure encryption
    // conf->set("security.protocol", "SASL_SSL", errstr);

    // // Set SASL mechanism to PLAIN
    // conf->set("sasl.mechanisms", "PLAIN", errstr);

    // // Set SASL credentials
    // conf->set("sasl.username", "your_username", errstr);
    // conf->set("sasl.password", "your_password", errstr);

    // // Set SSL configurations, assuming SSL is enabled
    // conf->set("ssl.ca.location", "/path/to/ca-cert", errstr);

    // Create producer instance
    producer.reset(RdKafka::Producer::create(conf.get(), errstr));
    if (!producer)
    {
        throw std::runtime_error("Failed to create producer: " + errstr);
    }

    return true;
}

bool KafkaProducerWrapper::AddTopic(const std::string &topic_name)
{
    if (topics.find(topic_name) != topics.end())
    {
        // Topic already exists
        return false;
    }

    std::string errstr;
    std::unique_ptr<RdKafka::Conf> topicConf(RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC));
    // Configure the topic if necessary

    std::unique_ptr<RdKafka::Topic> topic(RdKafka::Topic::create(producer.get(), topic_name, topicConf.get(), errstr));
    if (!topic)
    {
        throw std::runtime_error("Failed to create topic: " + errstr);
    }

    topics[topic_name] = std::move(topic);
    return true;
}

bool KafkaProducerWrapper::RemoveTopic(const std::string &topic_name)
{
    auto it = topics.find(topic_name);
    if (it == topics.end())
    {
        // Topic not found
        return false;
    }

    topics.erase(it);
    return true;
}

bool KafkaProducerWrapper::ProduceMessage(const std::string &topic_name, const std::string &message, int retry_attempts, int retry_delay_ms)
{
    auto it = topics.find(topic_name);
    if (it == topics.end())
    {
        throw std::runtime_error("Topic not found: " + topic_name);
    }

    void *payload = (void *)message.c_str();
    size_t len = message.size();

    while (retry_attempts > 0)
    {
        RdKafka::ErrorCode resp = producer->produce(
            it->second.get(), RdKafka::Topic::PARTITION_UA,
            RdKafka::Producer::RK_MSG_COPY,
            payload, len,
            nullptr, 0, // Optional key and its length
            nullptr     // Optional message opaque value
        );

        if (resp == RdKafka::ERR_NO_ERROR)
        {
            return true; // Message successfully produced
        }
        else if (resp != RdKafka::ERR__QUEUE_FULL)
        {
            throw std::runtime_error("Failed to produce message: " + RdKafka::err2str(resp));
        }

        // Handle retriable error
        retry_attempts--;
        std::this_thread::sleep_for(std::chrono::milliseconds(retry_delay_ms));

        // Poll to serve delivery report callbacks
        producer->poll(retry_delay_ms);
    }

    throw std::runtime_error("Failed to produce message after retries");
}
