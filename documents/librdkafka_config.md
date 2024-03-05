# Kafka Producer

- internal event queue
- event processing
- event types
  - delivery reports
  - error events
  - other events
- poll() method
- flush() method
- error handling of `produce` method
- producer Conf
  - "queue.buffering.max.messages"
  - "queue.buffering.max.kbytes"
  - "security.protocol"

## `security.protocol` configuration `SASL_SSL`

In Apache Kafka, the `security.protocol` configuration setting is used to specify the protocol for encrypting and authenticating connections between Kafka clients and brokers. One of the options for this setting is `SASL_SSL`, which stands for Simple Authentication and Security Layer over SSL (Secure Sockets Layer).

### Understanding `SASL_SSL`

#### SASL (Simple Authentication and Security Layer)

- **Purpose:** SASL is a framework that provides authentication mechanisms in protocol libraries. It decouples authentication procedures from application protocols, allowing for various authentication methods (like GSSAPI, PLAIN, SCRAM, etc.) without changing the protocol.
  
- **In Kafka:** Kafka uses SASL to authenticate clients to the Kafka cluster. The specific type of SASL mechanism (e.g., SASL/PLAIN, SASL/SCRAM-SHA-256) determines how authentication is performed.

#### SSL (Secure Sockets Layer)

- **Encryption:** SSL provides transport-level security by encrypting the data transmitted over the network. This ensures that data exchanged between Kafka clients and brokers is secure from eavesdropping.

- **Trust and Identity Verification:** SSL uses certificates to verify the identity of clients and brokers, ensuring that clients are communicating with the correct Kafka brokers.

### Configuring `SASL_SSL` in Kafka Clients

To configure a Kafka client to use `SASL_SSL`, you need to set several properties in the client's configuration.

#### Step 1: Set `security.protocol` to `SASL_SSL`

This tells the client to use SASL for authentication over an SSL-encrypted channel.

```cpp
conf->set("security.protocol", "SASL_SSL", errstr);
```

#### Step 2: Configure SASL Mechanism

Choose and set the SASL mechanism (e.g., PLAIN, SCRAM, GSSAPI). This choice dictates how authentication will be handled.

```cpp
conf->set("sasl.mechanisms", "PLAIN", errstr);  // Example using SASL/PLAIN
```

#### Step 3: Provide Authentication Credentials

Depending on the SASL mechanism, provide the necessary credentials. For example, with SASL/PLAIN, you need to provide a username and password.

```cpp
conf->set("sasl.username", "your_username", errstr);
conf->set("sasl.password", "your_password", errstr);
```

#### Step 4: Configure SSL

- **SSL Certificate Files:** Specify the path to the SSL certificate files. These are used to verify the identity of the brokers.

```cpp
conf->set("ssl.ca.location", "/path/to/ca-cert", errstr);  // CA certificate
```

- Optionally, you might also need to set `ssl.certificate.location` and `ssl.key.location` if client authentication is required by the broker.

#### Step 5: Create the Kafka Client

With the configuration set up, create the Kafka client (producer or consumer) as usual.

```cpp
RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
```

### Conclusion

Using `SASL_SSL` in Kafka provides both secure authentication (via SASL) and encrypted data transmission (via SSL). This combination is often used in production environments to ensure secure communication between Kafka clients and the cluster. Remember that the exact configuration steps can vary depending on the SASL mechanism and your specific Kafka setup, so always refer to the documentation specific to your Kafka environment and `librdkafka` version.

## Understanding `sasl.mechanisms` in Apache Kafka

The `sasl.mechanisms` configuration in Apache Kafka specifies the SASL mechanism for authentication. It's a critical setting for securing Kafka clients and brokers communication.

### 1: PLAIN

#### Mechanism Details

- **Mechanism:** `PLAIN`
- **Description:** A simple username and password-based mechanism. It transmits credentials in plain text.
- **Usage:** Common in scenarios demanding simplicity and ease of use. Must be used with SSL/TLS to ensure security.

#### Code Example for PLAIN

Configure a Kafka producer to use SASL/PLAIN for authentication:

```cpp
#include <librdkafka/rdkafkacpp.h>
#include <string>

RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

// Set security protocol to SASL_SSL to ensure encryption
conf->set("security.protocol", "SASL_SSL", errstr);

// Set SASL mechanism to PLAIN
conf->set("sasl.mechanisms", "PLAIN", errstr);

// Set SASL credentials
conf->set("sasl.username", "your_username", errstr);
conf->set("sasl.password", "your_password", errstr);

// Set SSL configurations, assuming SSL is enabled
conf->set("ssl.ca.location", "/path/to/ca-cert", errstr);

// Create the producer
RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
// ... Error handling and usage of producer
```

### 2: GSSAPI (Kerberos)

#### Mechanism Details

- **Mechanism:** `GSSAPI`
- **Description:** Utilizes Kerberos for authentication. More secure than PLAIN.
- **Usage:** Suited for larger enterprises or environments with existing Kerberos infrastructure.

#### Code Example for GSSAPI

Configure a Kafka client to use SASL/GSSAPI (Kerberos):

```cpp
// Set security protocol and SASL mechanism
conf->set("security.protocol", "SASL_SSL", errstr);
conf->set("sasl.mechanisms", "GSSAPI", errstr);

// Kerberos configuration
conf->set("sasl.kerberos.service.name", "kafka", errstr);
conf->set("sasl.kerberos.principal", "kafkaclient@YOUR.REALM.COM", errstr);
conf->set("sasl.kerberos.keytab", "/path/to/your.keytab", errstr);

// SSL configurations
// ...

// Create the Kafka client (producer or consumer)
// ...
```

### 3: SCRAM

#### Mechanism Details

- **Mechanism:** `SCRAM-SHA-256` and `SCRAM-SHA-512`
- **Description:** SCRAM (Salted Challenge Response Authentication Mechanism) is more secure than PLAIN, as it prevents password eavesdropping and replay attacks.
- **Usage:** Ideal for systems requiring a stronger level of security without the complexity of Kerberos.

#### Code Example for SCRAM

Configure a Kafka client for SASL/SCRAM:

```cpp
// Set security protocol and SASL mechanism to SCRAM
conf->set("security.protocol", "SASL_SSL", errstr);
conf->set("sasl.mechanisms", "SCRAM-SHA-512", errstr);  // or SCRAM-SHA-256

// Set SCRAM username and password
conf->set("sasl.username", "your_username", errstr);
conf->set("sasl.password", "your_password", errstr);

// SSL configurations
// ...

// Create the Kafka client
// ...
```

### 4: OAUTHBEARER

#### Mechanism Details

- **Mechanism:** `OAUTHBEARER`
- **Description:** Enables OAuth 2.0 bearer tokens for client authentication.
- **Usage:** Suitable for cloud-native environments or integrations with OAuth 2.0 compliant authorization servers.

#### Code Example for OAUTHBEARER

Configuring OAUTHBEARER is more involved as it requires token generation and handling:

```cpp
// Set security protocol and SASL mechanism to OAUTHBEARER
conf->set("security.protocol", "SASL_SSL", errstr);
conf->set("sasl.mechanisms", "OAUTHBEARER", errstr);

// OAuth bearer token refresh callback
conf->set("oauthbearer_token_refresh_cb", your_token_refresh_callback, errstr);

// SSL configurations
// ...

// Create the Kafka client
// ...
```

### 5: AWS_MSK_IAM and EXTERNAL

#### Mechanism Details

- **AWS_MSK_IAM:** Specific to AWS Managed Streaming for Kafka (MSK) for IAM-based authentication.
- **EXTERNAL:** Uses client's SSL certificate for authentication in mutual TLS setups.

#### Code Examples

AWS_MSK_IAM and EXTERNAL are more specialized and depend heavily on the specific environment (AWS MSK for AWS_MSK_IAM and mutual TLS environments for EXTERNAL). Their configuration would follow similar patterns to the above but tailored to those specific contexts.

### Conclusion

Each `sasl.mechanisms` option caters to different security and infrastructure needs. Choosing the right mechanism depends on your security requirements, existing infrastructure, and Kafka environment. The provided code examples offer a starting point for configuring Kafka clients with different SASL mechanisms.

## Understanding `security.protocol` in librdkafka

In `librdkafka`, a C/C++ client library for Apache Kafka, the `security.protocol` configuration setting is crucial for determining how data is transmitted between Kafka clients (producers/consumers) and Kafka brokers. It defines the protocol used to secure communication. Let's explore its options, usages, and provide code examples for each. This exploration will be divided into several parts for clarity.

### 1: PLAINTEXT

#### Option Details

- **Option:** `PLAINTEXT`
- **Description:** This is the default mode, where data is sent in clear text without any encryption.
- **Usage:** Suitable for non-sensitive data or environments where security is not a concern, like within a secured network.

#### Code Example for PLAINTEXT

Here's a basic example of configuring a Kafka producer for PLAINTEXT:

```cpp
#include <librdkafka/rdkafkacpp.h>
#include <string>

RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

// Set security protocol to PLAINTEXT
conf->set("security.protocol", "PLAINTEXT", errstr);

// Create the producer
RdKafka::Producer* producer = RdKafka::Producer::create(conf, errstr);
// ... Error handling and usage of producer
```

### 2: SSL

#### Option Details

- **Option:** `SSL`
- **Description:** Enables full transport encryption using SSL/TLS.
- **Usage:** Ideal for securing data in transit, particularly over untrusted networks.

#### Code Example for SSL

Configure a Kafka producer to use SSL/TLS for encryption:

```cpp
// Set security protocol to SSL
conf->set("security.protocol", "SSL", errstr);

// Configure SSL - paths to SSL certificate files
conf->set("ssl.ca.location", "/path/to/ca-cert", errstr);  // CA certificate
conf->set("ssl.certificate.location", "/path/to/client-cert.pem", errstr);  // Client certificate
conf->set("ssl.key.location", "/path/to/client-key.pem", errstr);  // Client key
conf->set("ssl.key.password", "yourkeypassword", errstr);  // Key password, if any

// Create the producer
// ...
```

### 3: SASL_PLAINTEXT

#### Option Details

- **Option:** `SASL_PLAINTEXT`
- **Description:** Utilizes SASL for authentication without encrypting the data transmission.
- **Usage:** Suitable for environments where authentication is required, but encryption is not necessary, like within a secure internal network.

#### Code Example for SASL_PLAINTEXT

Configure a Kafka client for SASL_PLAINTEXT:

```cpp
// Set security protocol to SASL_PLAINTEXT
conf->set("security.protocol", "SASL_PLAINTEXT", errstr);

// Set the SASL mechanism, e.g., PLAIN
conf->set("sasl.mechanisms", "PLAIN", errstr);
conf->set("sasl.username", "your_username", errstr);
conf->set("sasl.password", "your_password", errstr);

// Create the Kafka client
// ...
```

### 4: SASL_SSL

#### Option Details

- **Option:** `SASL_SSL`
- **Description:** Combines SASL for authentication with SSL/TLS for encryption.
- **Usage:** Best for environments requiring both secure authentication and encrypted data transmission.

#### Code Example for SASL_SSL

Here's how to configure a Kafka client for SASL_SSL:

```cpp
// Set security protocol to SASL_SSL
conf->set("security.protocol", "SASL_SSL", errstr);

// Configure SASL mechanism and credentials
conf->set("sasl.mechanisms", "PLAIN", errstr);  // Example with SASL/PLAIN
conf->set("sasl.username", "your_username", errstr);
conf->set("sasl.password", "your_password", errstr);

// Configure SSL
conf->set("ssl.ca.location", "/path/to/ca-cert", errstr);

// Create the Kafka client
// ...
```

### Conclusion

The `security.protocol` setting in librdkafka is critical for determining the security level of your Kafka client's communication with brokers. Each option serves different security needs ranging from no security (PLAINTEXT) to full encryption and authentication (SASL_SSL). When choosing a security protocol, consider the sensitivity of your data, network environment, and compliance requirements. The provided examples offer a basic guideline on how to configure these settings in your Kafka client application.
