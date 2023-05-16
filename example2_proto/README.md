# Kafka Producer and Consumer Example

This project demonstrates a simple Apache Kafka producer and consumer in C++ using the `cppkafka` library and Google's Protocol Buffers for object serialization.

## Project Structure

```
kafka\_example/
│
├── src/
│   ├── kafka\_consumer.cpp
│   ├── kafka\_producer.cpp
│   ├── person.pb.h
│   └── person.pb.cc
│
├── protos/
│   └── person.proto
│
└── Makefile
```

## Dependencies

- [librdkafka](https://github.com/edenhill/librdkafka)
- [cppkafka](https://github.com/mfontanini/cppkafka)
- [Google Protocol Buffers (protobuf)](https://developers.google.com/protocol-buffers)

## Build Instructions

1. Install the required dependencies: protobuf library and compiler.

   ```bash
   sudo apt-get install libprotobuf-dev protobuf-compiler
   ````

2. Generate the C++ code for the `Person` object using the protobuf compiler.

   ```bash
   protoc -I=protos --cpp_out=src protos/person.proto
   ```

3. Build the Kafka producer and consumer using the provided `Makefile`.

   ```bash
   make
   ```

## Usage

1. Start the Kafka and Zookeeper

  **Start the ZooKeeper server**: Kafka uses ZooKeeper to manage its distributed nature, so you need to start the ZooKeeper server before starting Kafka. Run the following command:

   ```bash
   bin/zookeeper-server-start.sh config/zookeeper.properties
   ```

   Keep this terminal open or run the command in the background.

   **Start the Kafka server**: Open a new terminal and navigate to the Kafka directory, then run the following command:

   ```bash
   bin/kafka-server-start.sh config/server.properties
   ```

   Keep this terminal open or run the command in the background.

   Once the Kafka server is up and running, you should be able to run the Kafka producer and consumer built in the next step.

2. Run the Kafka producer in a separate terminal.

   ```bash
   ./kafka_producer
   ```

3. Run the Kafka consumer in another terminal.

   ```bash
   ./kafka_consumer
   ```

The consumer should receive and display the serialized `Person` object sent by the producer.

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
