#include "../include/RoundRobinPartitioner.h"

int32_t RoundRobinPartitioner::partitioner_cb(const RdKafka::Topic *topic, const std::string *key,
                                               int32_t partition_cnt, void *msg_opaque) {
    static int32_t current_partition = 0;
    current_partition = (current_partition + 1) % partition_cnt;
    return current_partition;
}
