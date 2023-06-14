#ifndef ROUND_ROBIN_PARTITIONER_H
#define ROUND_ROBIN_PARTITIONER_H

#include <librdkafka/rdkafkacpp.h>

class RoundRobinPartitioner : public RdKafka::PartitionerCb
{
public:
    int32_t partitioner_cb(const RdKafka::Topic *topic, const std::string *key,
                           int32_t partition_cnt, void *msg_opaque);
};

#endif // ROUND_ROBIN_PARTITIONER_H
