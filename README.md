# GAuthDaemon

**GAuthDaemon** is an open source authentication server for the *Perfect World* game ecosystem. This version is a full C++17 implementation without Qt and uses the MariaDB connector library for storage.

## Features

- Lightweight C++ implementation built on POSIX sockets and standard threading
- MariaDB/MySQL backend for user accounts and billing
- Configurable validation and anti‑brute‑force thresholds
- CLI daemon suitable for Rocky Linux 9 servers

## Getting Started

### Prerequisites

- A C++17 compatible compiler (GCC 11+ on Rocky Linux 9)
- MariaDB Connector/C development headers and client library
- A MariaDB server (or compatible MySQL server)

#### Rocky Linux 9

```bash
sudo dnf install gcc gcc-c++ make mariadb-server mariadb-connector-c mariadb-connector-c-devel
```

For MariaDB 10.11, enable the official MariaDB repository that matches your Rocky Linux release and install the `MariaDB-server` and `MariaDB-client` packages from that repository before building. The daemon links against the system MariaDB connector, so keeping the connector packages in sync with the server version prevents runtime driver errors.

### Building from Source

Clone the repository and build it with `make`:

```bash
git clone https://github.com/hrace009/PWCI-AuthDaemon.git
cd PWCI-AuthDaemon
make
```

The resulting `gauthd` binary will be located in the project directory.

### Configuration

All runtime options are stored in `gauthd.conf`. Below is a short excerpt of the configuration file:

```ini
[GAuthServer]
host = 127.0.0.1
port = 29200
allow_auth_type = login

gold_timeout = 5
hash_type = 2

[Mysql]
host = 127.0.0.1
port = 3306
user = pw
pass = pw
db = pw

[Validator]
is_enabled = true
regex = [^a-zA-Z0-9_-$#@.]

[Antibrut]
is_enabled = true
count = 3
interval = 10
block_time = 60
```

Edit these values to match your environment before starting the daemon.

### Running

After configuring, simply launch the server:

```bash
./gauthd gauthd.conf
```

The daemon listens for client connections on the configured host and port and logs events to the console.

## Contributing

Contributions are welcome! Feel free to submit pull requests or report issues to help improve GAuthDaemon.

## License

This project is licensed under the terms of the [GNU General Public License v3.0](LICENSE).
