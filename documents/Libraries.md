# Develop Kafka Applications

There are multiple ways to develop Kafka applications, each suitable for different use cases and requirements.

1.**Producer and Consumer APIs**: These APIs are used for directly producing messages to Kafka topics and consuming messages from Kafka topics. They provide fine-grained control over message production and consumption and are suitable for various data streaming and integration scenarios. The **Kafka Producer API** allows you to send records to one or more Kafka topics, while the **Kafka Consumer API** allows you to read records from one or more Kafka topics.

2.**Kafka Streams API**: This API is designed for building highly scalable and fault-tolerant stream processing applications that consume, process, and produce data stored in Kafka topics. The Kafka Streams API provides high-level functional programming constructs and low-level processor APIs for defining complex stream processing topologies. It is suitable for building real-time data processing applications, such as data transformation, aggregation, and analytics.

*Note*: The Apache Kafka project only directly supports the Kafka Streams API in Java. The C/C++ library, librdkafka, supports the basic **producer and consumer APIs**, but it does not support the higher-level stream processing capabilities found in the Kafka Streams API.
If you require stream processing capabilities in C++, you will likely need to implement these features yourself on top of the consumer and producer primitives provided by librdkafka, or use a third-party stream processing library.
However, there are other languages with Kafka client libraries that do support the Kafka Streams API. For instance, there is a Python library called Faust and a Node.js library called kafkajs that have similar capabilities.

3.**Kafka Connect API**: This API is used for building scalable and reliable data pipelines between Kafka and other data systems. Kafka Connect provides a framework for importing data from external systems into Kafka topics (**source** connectors) and exporting data from Kafka topics to external systems (**sink** connectors). It is suitable for building data integration solutions, such as ETL pipelines and data synchronization, and supports various data sources and sinks through built-in and custom connectors.

Apache Kafka's Kafka Connect API is a component of Apache Kafka itself and it is developed in Java. It is not a client library, but a distributed integration framework. Therefore, it is not something that would be provided by a library in the way that the producer and consumer APIs are.

Unfortunately, there is no direct C/C++ client library that supports the Kafka Connect API, as the Kafka Connect API is not designed to be used by client applications directly, but rather to run in a Kafka Connect cluster as a separate service.

However, you can interact with Kafka Connect using its **REST interface** from any language that can send HTTP requests, including C++. This is typically used for tasks such as starting, stopping, and managing connectors.

4.**Kafka Admin API**: The Admin API is a *Java API* for managing and inspecting topics, brokers, and other Kafka objects. You can use it to create, alter, delete, and describe topics, as well as to list, describe, and manage consumer groups.

Example - Creating a Kafka topic using the Admin API:

```java
import org.apache.kafka.clients.admin.AdminClient;
import org.apache.kafka.clients.admin.CreateTopicsResult;
import org.apache.kafka.clients.admin.NewTopic;

import java.util.Collections;
import java.util.Properties;
import java.util.concurrent.ExecutionException;

public class KafkaAdminExample {
    public static void main(String[] args) throws InterruptedException, ExecutionException {
        Properties config = new Properties();
        config.put("bootstrap.servers", "localhost:9092");

        AdminClient adminClient = AdminClient.create(config);

        NewTopic newTopic = new NewTopic("my_topic", 1, (short) 1);
        CreateTopicsResult result = adminClient.createTopics(Collections.singletonList(newTopic));

        result.all().get();

        adminClient.close();
    }
}
```

5.Kafka REST Proxy: The Kafka REST Proxy is a RESTful API that provides an alternative to the native Kafka protocol for producing and consuming messages. It allows you to access Kafka clusters using HTTP/JSON, which is particularly useful when you're developing applications in languages that do not have native Kafka client libraries.

Example - Producing a message using the Kafka REST Proxy:

```bash
curl -X POST -H "Content-Type: application/vnd.kafka.json.v2+json" \
    --data '{"records":[{"value":{"name": "John Doe", "age": 30}}]}' \
    http://localhost:8082/topics/my_topic
```

6.KSQL (now called ksqlDB): ksqlDB is an event streaming database that allows you to build stream processing applications using SQL-like queries. With ksqlDB, you can create, read, write, and modify streams and tables in Kafka using a familiar SQL syntax. This makes it easier to work with Kafka data, especially for users who are already familiar with SQL.

Example - Create a stream and filter messages using ksqlDB:

```sql
CREATE STREAM person_stream (name VARCHAR, age INT)
  WITH (KAFKA_TOPIC='person_topic', VALUE_FORMAT='JSON');

CREATE STREAM adult_stream AS
  SELECT * FROM person_stream WHERE age >= 18;
```

7.Spring Kafka: Spring Kafka is a library that provides high-level abstractions for building Kafka applications using the Spring framework. It simplifies the development and configuration of Kafka producers, consumers, and streams, and integrates with other Spring components such as the Spring Boot auto-configuration and the Spring Cloud Stream binder.

Example - Producing a message using Spring Kafka:

application.yml:

```yaml
spring:
  kafka:
    bootstrap-servers: localhost:9092
    producer:
      key-serializer: org.apache.kafka.common.serialization.StringSerializer
      value-serializer: org.apache.kafka.common.serialization.StringSerializer
```

KafkaProducerExample.java:

```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.core.KafkaTemplate;
import org.springframework.stereotype.Component;

@Component
public class KafkaProducerExample {
    @Autowired
    private KafkaTemplate<String, String> kafkaTemplate;

    public void sendMessage(String message) {
        kafkaTemplate.send("my_topic", message);
    }
}
```

8.Faust: Faust is a stream processing library for Python, built on top of the Kafka Streams API. It provides a functional programming style similar to Kafka Streams but with Pythonic syntax. Faust allows you to develop stream processing applications in Python and integrates with popular Python libraries such as asyncio and aiokafka.

Example - Filtering messages using Faust:

faust\_example.py:

```python
import faust

app = faust.App('faust_example', broker='kafka://localhost:9092')
source_topic = app.topic('source_topic', value_type=str)
destination_topic = app.topic('destination_topic', value_type=str)

@app.agent(source_topic)
async def process(stream):
    async for value in stream:
        if "example" in value:
            await destination_topic.send(value=value)
```

9.KafkaJS: KafkaJS is a modern Apache Kafka client library for Node.js applications. It provides a simple and robust API for producing and consuming messages with Kafka in JavaScript and TypeScript applications.

Example - Producing a message using KafkaJS:

kafka\_producer\_example.js:

```javascript
const { Kafka } = require('kafkajs');

async function main() {
  const kafka = new Kafka({ clientId: 'kafkajs-example', brokers: ['localhost:9092'] });
  const producer = kafka.producer();

  await producer.connect();
  await producer.send({
    topic: 'my_topic',
    messages: [{ value: 'Hello KafkaJS!' }],
  });

  await producer.disconnect();
}

main().catch(console.error);
```

10. Confluent Schema Registry: Confluent Schema Registry is a service that stores and retrieves Avro schema versions for Kafka producers and consumers. It allows you to evolve the schema of your Kafka messages while maintaining compatibility between different versions. The Schema Registry provides serializers and deserializers for integrating with Kafka clients.

Example - Producing a message using Avro and Schema Registry:

pom.xml dependencies:

```xml
<dependency>
    <groupId>io.confluent</groupId>
    <artifactId>kafka-avro-serializer</artifactId>
    <version>${confluent.version}</version>
</dependency>
<dependency>
    <groupId>io.confluent</groupId>
    <artifactId>kafka-schema-registry-client</artifactId>
    <version>${confluent.version}</version>
</dependency>
```

ProducerExample.java:

```java
import io.confluent.kafka.serializers.KafkaAvroSerializer;
import io.confluent.kafka.serializers.KafkaAvroSerializerConfig;
import org.apache.avro.generic.GenericRecord;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.Producer;
import org.apache.kafka.clients.producer.ProducerRecord;

import java.util.Properties;

public class ProducerExample {
    public static void main(String[] args) {
        Properties props = new Properties();
        props.put("bootstrap.servers", "localhost:9092");
        props.put("key.serializer", "org.apache.kafka.common.serialization.StringSerializer");
        props.put("value.serializer", KafkaAvroSerializer.class.getName());
        props.put(KafkaAvroSerializerConfig.SCHEMA_REGISTRY_URL_CONFIG, "http://localhost:8081");

        try (Producer<String, GenericRecord> producer = new KafkaProducer<>(props)) {
            GenericRecord record = createAvroRecord();
            ProducerRecord<String, GenericRecord> producerRecord = new ProducerRecord<>("my_topic", record);
            producer.send(producerRecord);
        }
    }
}
```

11.Alpakka Kafka: Alpakka Kafka is a library that provides a Reactive Streams API for building Kafka applications using the Akka Streams framework. It enables you to develop highly concurrent, non-blocking, and back-pressured Kafka producers, consumers, and streams in Scala and Java.

Example - Consuming messages using Alpakka Kafka:

build.sbt dependencies:

```scala
libraryDependencies += "com.typesafe.akka" %% "akka-stream-kafka" % "2.1.0"
```

ConsumerExample.scala:

```scala
import akka.actor.ActorSystem
import akka.kafka.ConsumerSettings
import akka.kafka.scaladsl.Consumer
import akka.stream.scaladsl.Sink
import org.apache.kafka.clients.consumer.ConsumerConfig
import org.apache.kafka.common.serialization.StringDeserializer

object ConsumerExample extends App {
  implicit val system: ActorSystem = ActorSystem("alpakka-kafka-example")
  import system.dispatcher

  val consumerSettings = ConsumerSettings(system, new StringDeserializer, new StringDeserializer)
    .withBootstrapServers("localhost:9092")
    .withGroupId("alpakka-kafka-example")
    .withProperty(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest")

  Consumer.plainSource(consumerSettings, Consumer.topics("my_topic"))
    .map(_.value)
    .to(Sink.foreach(println))
    .run()
}
```
