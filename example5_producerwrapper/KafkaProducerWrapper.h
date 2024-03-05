#include <librdkafka/rdkafkacpp.h>
#include <string>
#include <map>
#include <memory>

class KafkaProducerWrapper
{
public:
    KafkaProducerWrapper();
    ~KafkaProducerWrapper(); // The custom destructor might be optional now

    bool Initialize(const std::string &brokers, const std::string &client_id);
    bool AddTopic(const std::string &topic_name);
    bool RemoveTopic(const std::string &topic_name);
    bool ProduceMessage(const std::string &topic_name, const std::string &message, int retry_attempts = 5, int retry_delay_ms = 100);

private:
    std::unique_ptr<RdKafka::Producer> producer;
    std::unique_ptr<RdKafka::Conf> conf;
    std::map<std::string, std::unique_ptr<RdKafka::Topic>> topics;
};
