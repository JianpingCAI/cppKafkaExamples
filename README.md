# Build C/C++ Kafka Producer and Consumer Applications

Building a simple Kafka application in C/C++ involves using the librdkafka library, which is a C library implementation of the Apache Kafka protocol. You'll need to install librdkafka and its C++ wrapper, cppkafka, to build Kafka clients in C/C++. We'll create a simple producer and consumer example to demonstrate basic data communication.

## **step 0: Prerequisites**

***Install OpenSSL libraries (if you haven't already), which are required for building cppkafka.***

For Debian/Ubuntu-based systems, run the following command to install the OpenSSL development package:

```bash
sudo apt-get install libssl-dev
```

For CentOS/RHEL-based systems, run the following command:

```bash
sudo yum install openssl-devel
```

For Fedora-based systems, run the following command:

```bash
sudo dnf install openssl-devel
```

If not installed, you will encounter error below while running `make` for install cppkafka,
```
/usr/bin/ld: cannot find -lssl
/usr/bin/ld: cannot find -lcrypto
```

***Install JDK (if you haven't already), which is required for running the ZooKeeper server.***

1. **Install OpenJDK 11**: Run the following commands to install OpenJDK 11:

```bash
sudo apt update
sudo apt install openjdk-11-jdk
```

2. **Set the `JAVA_HOME` environment variable**: After installing OpenJDK, set the `JAVA_HOME` environment variable to point to the JDK installation directory:

```bash
echo "export JAVA_HOME=$(dirname $(dirname $(readlink $(readlink $(which javac)))))" >> ~/.bashrc
source ~/.bashrc
```

## **Step 1: Install and Run Kafka ZooKeeper and Broker**

1. **Download and install Kafka**: Download the latest Kafka release from the official website (<https://kafka.apache.org/downloads>) and extract the archive:

```bash
wget https://downloads.apache.org/kafka/3.4.0/kafka_2.13-3.4.0.tgz
tar -xzf kafka_2.XX-YY.ZZ.tgz
cd kafka_2.XX-YY.ZZ
```

Replace `kafka_2.XX-YY.ZZ.tgz` with the actual downloaded file.

2. **Start the ZooKeeper server**: Kafka uses ZooKeeper to manage its distributed nature, so you need to start the ZooKeeper server before starting Kafka. Run the following command:

```bash
bin/zookeeper-server-start.sh config/zookeeper.properties
```

Keep this terminal open or run the command in the background.

3. **Start the Kafka server**: Open a new terminal and navigate to the Kafka directory, then run the following command:

```bash
bin/kafka-server-start.sh config/server.properties
```

Keep this terminal open or run the command in the background.

Once the Kafka server is up and running, you should be able to run the Kafka producer and consumer built in the next step.

## **Step 2: Install C++ Dependencies: librdkafka and cppkafka**

Install librdkafka:

```bash
git clone https://github.com/edenhill/librdkafka.git
cd librdkafka
./configure --prefix=/usr
make
sudo make install
```

Install cppkafka:

```bash
git clone https://github.com/mfontanini/cppkafka.git
cd cppkafka
mkdir build
cd build
cmake .. -DRDKAFKA_ROOT_DIR=/usr
make
sudo make install
```

## **Step 3: Create a Kafka producer**

Create a file named `kafka_producer.cpp`:

```cpp
#include <iostream>
#include <string>
#include <cppkafka/cppkafka.h>

using namespace std;
using namespace cppkafka;

int main() {
    // Configuration
    Configuration config = {
        {"metadata.broker.list", "localhost:9092"}
    };

    // Create a Kafka producer
    Producer producer(config);

    string message;
    while(message != "exit")
    {
        cout << "Enter message to send: ";
        getline(cin, message);

        // Send the message
        producer.produce(MessageBuilder("test_topic").partition(0).payload(message));
        producer.flush();

        cout << "Message sent: " << message << endl;
    }

    return 0;
}
```

## **Step 4: Create a Kafka consumer**

Create a file named `kafka_consumer.cpp`:

```cpp
#include <iostream>
#include <string>
#include <cppkafka/cppkafka.h>

using namespace std;
using namespace cppkafka;

int main() {
    // Configuration
    Configuration config = {
        {"metadata.broker.list", "localhost:9092"},
        {"group.id", "test_group"},
        {"enable.auto.commit", false}
    };

    // Create a Kafka consumer
    Consumer consumer(config);
    consumer.subscribe({"test_topic"});

    // Process messages
    while (true) {
        Message msg = consumer.poll(100);
        if (msg) {
            if (msg.get_error()) {
                cout << "Error while consuming: " << msg.get_error() << endl;
            } else {
                cout << "Received message: " << msg.get_payload() << endl;
            }
        }
    }

    return 0;
}
```

## **Step 5: Compile the producer and consumer**

Compile the producer:

```bash
g++ -std=c++11 -o kafka_producer kafka_producer.cpp -lcppkafka -lrdkafka
```

Compile the consumer:

```bash
g++ -std=c++11 -o kafka_consumer kafka_consumer.cpp -lcppkafka -lrdkafka
```

Or use a `Makefile` to build them,

```makefile
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Linker flags
LDFLAGS = -lcppkafka -lrdkafka

# Executable names
PRODUCER_EXEC = kafka_producer
CONSUMER_EXEC = kafka_consumer

all: $(PRODUCER_EXEC) $(CONSUMER_EXEC)

$(PRODUCER_EXEC): kafka_producer.cpp
 $(CXX) $(CXXFLAGS) kafka_producer.cpp -o $(PRODUCER_EXEC) $(LDFLAGS)

$(CONSUMER_EXEC): kafka_consumer.cpp
 $(CXX) $(CXXFLAGS) kafka_consumer.cpp -o $(CONSUMER_EXEC) $(LDFLAGS)

clean:
 rm -f $(PRODUCER_EXEC) $(CONSUMER_EXEC)
```

To build both executables, simply run `make` in the terminal:

```bash
make
```

This will produce two executables, `kafka_producer` and `kafka_consumer`. To clean up the executables, you can run `make clean`.

## **Step 6: Run the producer and consumer**

Start the Kafka producer:

```bash
./kafka_producer
```

Start the Kafka consumer:

```bash
./kafka_consumer
```

Enter a message in the producer's terminal, and you should see the message being received by the consumer.

#### Trouble Shooting

./kafka_producer: error while loading shared libraries: libcppkafka.so.0.4.0: cannot open shared object file: No such file or directory

#### Solution

This can happen if the library is not installed in a standard system library path or if the linker is not aware of the library's location.

To resolve this issue, you can:

**Set the `LD_LIBRARY_PATH` environment variable**: This variable tells the linker where to search for shared libraries. Add the path to the `libcppkafka.so.0.4.0` file to `LD_LIBRARY_PATH`:

```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/cppkafka/lib
```