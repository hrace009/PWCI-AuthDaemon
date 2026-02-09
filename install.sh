#!/bin/bash

# Define Log File Name
LOGFILE="install.log"

log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> $LOGFILE
}

echo "Start of installation - $(date)" > $LOGFILE

log "Updating existing packages..."
echo "Updating existing packages..."
if ! sudo dnf update -y | tee -a $LOGFILE; then
    log "Error updating packages."
    exit 1
fi

log "Reinstalling gcc, gcc-c++, and glibc-devel packages..."
echo "Reinstalling gcc, gcc-c++, and glibc-devel packages..."
if ! sudo dnf reinstall -y gcc gcc-c++ glibc-devel | tee -a $LOGFILE; then
    log "Error reinstalling gcc, gcc-c++, and glibc-devel packages."
    exit 1
fi

log "Installing Development Tools..."
echo "Installing Development Tools..."
if ! sudo dnf groupinstall "Development Tools" -y | tee -a $LOGFILE; then
    log "Error installing Development Tools."
    exit 1
fi

log "Installing MariaDB connector and server packages..."
echo "Installing MariaDB connector and server packages..."
if ! sudo dnf install -y mariadb-server mariadb-connector-c mariadb-connector-c-devel | tee -a $LOGFILE; then
    log "Error installing MariaDB packages."
    exit 1
fi

log "Removing unnecessary packages..."
echo "Removing unnecessary packages..."
if ! sudo dnf autoremove -y | tee -a $LOGFILE; then
    log "Error removing unnecessary packages."
    exit 1
fi

log "Installation completed - $(date)"
echo "Installation completed - $(date)" >> $LOGFILE
