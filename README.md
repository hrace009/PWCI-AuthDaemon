# GAuthDaemon

**GAuthDaemon** is an open source authentication server for the *Perfect World* game ecosystem.  This project is a C++ rewrite of the original daemon and uses the Qt 5 framework together with MySQL for data storage.  It aims to provide a stable and customizable auth service for private servers and developers.

## Features

- Lightweight C++ implementation built on Qt core, network and SQL modules
- MySQL database backend for user accounts and billing
- Optional login modes: username/password, token based and email login
- Anti‑brute‑force protection with configurable thresholds
- Configurable validation rules for account names and tokens
- CLI daemon suitable for Linux servers

## Getting Started

### Prerequisites

- Qt 5 development packages (QtCore, QtNetwork, QtSql)
- A MySQL or MariaDB server
- A C++17 compatible compiler

For Fedora based distributions you can run the provided `install.sh` to install all required Qt packages automatically.

### Building

Clone the repository and build it with `qmake`:

```bash
git clone https://github.com/yourname/PWCI-AuthDaemon.git
cd PWCI-AuthDaemon
qmake gauthd.pro
make
```

The resulting `gauthd` binary will be located in the project directory.

### Configuration

All runtime options are stored in `gauthd.conf`.  Below is a short excerpt of the configuration file:

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
./gauthd
```

The daemon listens for client connections on the configured host and port and logs events to the console.

## Contributing

Contributions are welcome! Feel free to submit pull requests or report issues to help improve GAuthDaemon.

## License

This project is licensed under the terms of the [GNU General Public License v3.0](LICENSE).

