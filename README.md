# DNS Server & Library

Implementation of a DNS packet manipulation library (`dnslib`) and a DNS server application in C++.

## Project Structure

-   **lib/dnslib/**: Core library for DNS packet manipulation.
    -   `include/`: Public headers with Doxygen documentation.
    -   `src/`: Library implementation.
    -   `tests/`: Unit tests (Google Test).
-   **src/**: Source code for the terminal application.
-   **makefile**: Simplified project management.

## Getting Started

The project uses a `makefile` to wrap CMake commands for easier usage.

### Building and Running
To build the project and run the demonstration application:
```bash
make run
```

### Running Tests
To execute the unit test suite:
```bash
make test
```

### Cleaning Up
To remove build artifacts:
```bash
make clean
```

## Library Usage (dnslib)

All classes and functions are available within the `lk::dnslib` namespace.

### Building a DNS Packet
The library provides a fluent `PacketBuilder` for easy message construction.

```cpp
#include "dns.hpp"
#include <iostream>

using namespace lk::dnslib;

int main() {
    // Construct a DNS Query packet
    DNSPacket packet = PacketBuilder(0x1337)
                        .withOpcode(OPCODE::QUERY)
                        .withFlags(F_RECURSION_DES)
                        .addQuestion("example.com", TYPE::A)
                        .build();

    // Print packet details using overloaded operator<<
    std::cout << packet << std::endl;

    // Serialize to bytes for transmission
    std::vector<uint8_t> buffer;
    packet.serialize(buffer);

    return 0;
}
```

### Parsing a DNS Packet
Use `PacketParser` to convert raw byte data back into structured objects.

```cpp
#include "dns.hpp"
#include <vector>

using namespace lk::dnslib;

void handle_incoming(const std::vector<uint8_t>& raw_bytes) {
    try {
        // Parse raw bytes
        DNSPacket packet = PacketParser::parse(raw_bytes);
        
        // Inspect the header or records
        if (packet.getHeader().isResponse()) {
            std::cout << "Received response for ID: " << packet.getHeader().getId() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << std::endl;
    }
}
```

## Documentation
Public API documentation is provided directly in the header files (`.hpp`) using the Doxygen format.