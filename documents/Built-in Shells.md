Apache Kafka comes with a number of shell scripts that are useful for managing and interacting with a Kafka cluster. These scripts are located in the `bin/` directory of the Kafka distribution. Here are some of the most commonly used ones:

1. **kafka-topics.sh**: This script is used to create, delete, describe, or change a topic.

   Example usage:
   - Create a topic named "test" with a single partition and three replica:

     ```bash
     bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --replication-factor 3 --partitions 1 --topic test
     ```

   - List all topics:

     ```bash
     bin/kafka-topics.sh --list --bootstrap-server localhost:9092
     ```

   - Describe the "test" topic

     ```bash
     bin/kafka-topics.sh --describe --bootstrap-server localhost:9092 --topic test
     ```

2. **kafka-console-producer.sh**: This script is used to send messages to a Kafka topic from the command line.

   Example usage:
   - Send messages to the "test" topic:

     ```bash
     bin/kafka-console-producer.sh --broker-list localhost:9092 --topic test
     ```

3. **kafka-console-consumer.sh**: This script is used to consume messages from a Kafka topic from the command line.

   Example usage:
   - Consume messages from the "test" topic:

     ```bash
     bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic test --from-beginning
     ```

4. **kafka-consumer-groups.sh**: This script is used to list, describe, delete, or reset consumer group offsets.

   Example usage:
   - List all consumer groups:

     ```bash
     kafka-consumer-groups.sh --list --bootstrap-server localhost:9092
     ```

5. **kafka-configs.sh**: This script is used to alter or describe the configuration of topics, brokers, or client quotas.

   Example usage:
   - Describe the configuration of the "test" topic:

     ```bash
     kafka-configs.sh --describe --bootstrap-server localhost:9092 --entity-type topics --entity-name test
     ```

6. **zookeeper-shell.sh**: This script is used to interact with ZooKeeper, which Kafka uses for maintaining cluster state.

   Example usage:
   - List all znodes:

     ```bash
     zookeeper-shell.sh localhost:2181 ls /
     ```

1. **kafka-run-class.sh**: This script is used to run a class in the Kafka codebase. It's more of a utility script used by other scripts, but can be used directly to run Kafka classes.

2. **kafka-server-start.sh and kafka-server-stop.sh**: These scripts are used to start and stop a Kafka broker.

   Example usage:
   - Start a Kafka broker:

     ```
     kafka-server-start.sh config/server.properties
     ```

   - Stop a Kafka broker:

     ```
     kafka-server-stop.sh
     ```

3. **zookeeper-server-start.sh and zookeeper-server-stop.sh**: These scripts are used to start and stop a ZooKeeper server. ZooKeeper is a service that Kafka uses for maintaining configuration information and providing distributed synchronization.

   Example usage:
   - Start a ZooKeeper server:

     ```
     zookeeper-server-start.sh config/zookeeper.properties
     ```

   - Stop a ZooKeeper server:

     ```
     zookeeper-server-stop.sh
     ```

4. **kafka-producer-perf-test.sh**: This script is used to load test the Kafka cluster. It produces a specified number of messages of a specified size to a specified topic.

   Example usage:
   - Produce 10000 messages of size 100 to the "test" topic:

     ```
     kafka-producer-perf-test.sh --topic test --num-records 10000 --record-size 100 --throughput -1 --producer-props bootstrap.servers=localhost:9092
     ```

5. **kafka-replica-verification.sh**: This script is used to test the consistency of replicas in a Kafka cluster. It compares the set of messages in two replicas to ensure they are the same.

   Example usage:
   - Verify the consistency of replicas for the "test" topic:

     ```
     kafka-replica-verification.sh --broker-list localhost:9092 --topic-white-list test
     ```

6. **kafka-delegation-tokens.sh**: This script is used to manage delegation tokens in a Kafka cluster. Delegation tokens are used for authentication in a Kafka cluster.

   Example usage:
   - Create a delegation token:

     ```
     kafka-delegation-tokens.sh --bootstrap-server localhost:9092 --create --max-life-time-period -1
     ```

These are just a few examples of the scripts that come with Kafka and how they can be used. There are many other scripts and options available, so it's a good idea to familiarize yourself with the Kafka documentation and experiment with these scripts in a test environment.
