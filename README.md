# GAuthDaemon

**GAuthDaemon** is an open source authentication server for the *Perfect World* game ecosystem.  This project is a C++ rewrite of the original daemon and uses the Qt 5 framework together with MySQL/MariaDB for storage.

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

For Fedora-based distributions you can run the provided `install.sh` to install all required Qt packages automatically.

### Installation using Linux Packages

#### Rocky Linux 9

```bash
sudo dnf install qt5-qtbase-devel qt5-qtbase qt5-qttools qt5-qttools-devel qt5-qtbase-mysql mariadb-server mariadb-devel gcc-c++
```

#### Ubuntu 20.04

```bash
sudo apt update
sudo apt install qt5-qmake qtbase5-dev libqt5sql5-mysql build-essential mariadb-server libmariadb-dev
```

### Building from Source

Clone the repository and build it with `qmake`:

```bash
git clone https://github.com/hrace009/PWCI-AuthDaemon.git
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

## To compile the PWCI-AuthDaemon project on CentOS 7, follow these steps:

### 1. Install Required Dependencies

This project uses Qt5 (specifically Qt5Sql, Qt5Network, Qt5Core), MySQL client libraries, and GCC toolchain. Install the required packages:
```sh
sudo yum groupinstall "Development Tools"
sudo yum install qt5-qtbase-devel qt5-qtbase qt5-qtbase-gui qt5-qtbase-mysql qt5-qtdeclarative qt5-qttools qt5-qtsql-devel mysql-devel
```

You may need to enable EPEL or other repos for some packages.

### 2. Prepare the Build Environment

Ensure `qmake` (Qt's build system generator) is available:
```sh
which qmake
# If not found, try: sudo yum install qt5-qtbase-devel
```

### 3. Build the Project

In the root directory of the repository, run:
```sh
qmake gauthd.pro
make
```
- `qmake gauthd.pro` generates the `Makefile`.
- `make` compiles the project according to the generated Makefile.

### 4. (Optional) Clean Previous Builds

If you need to clean up previous build artifacts, run:
```sh
make clean
```

### 5. Troubleshooting

- **Header or Library Not Found:** If you get errors about missing Qt headers or libraries, verify your Qt installation and adjust the `INCPATH` or `LIBS` variables in the Makefile if needed.
- **MySQL Client Path:** The Makefile references a static MySQL client library. If you use dynamic linking or the library is in a different path, edit the Makefile or install the appropriate package.

### 6. Running the Application

After building, the executable (likely named `gauthd`) will be in the root or a build directory.

---

**Summary of Main Steps:**
1. `sudo yum groupinstall "Development Tools"`
2. `sudo yum install qt5-qtbase-devel qt5-qtsql-devel mysql-devel`
3. `qmake gauthd.pro`
4. `make`

## Contributing

Contributions are welcome! Feel free to submit pull requests or report issues to help improve GAuthDaemon.

## License

This project is licensed under the terms of the [GNU General Public License v3.0](LICENSE).
