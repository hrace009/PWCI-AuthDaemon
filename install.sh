#!/bin/bash

# Define Log File Name
LOGFILE="install_qt.log"

# Function to add messages to the log
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" >> $LOGFILE
}

# Initialize the log file
echo "Start of installation - $(date)" > $LOGFILE

# Update existing packages and display progress in the terminal and in the log
log "Updating existing packages..."
echo "Updating existing packages..."
if ! sudo dnf update -y | tee -a $LOGFILE; then
    log "Error updating packages."
    exit 1
fi

# Reinstall gcc, gcc-c++, and glibc-devel packages
log "Reinstalling gcc, gcc-c++, and glibc-devel packages..."
echo "Reinstalling gcc, gcc-c++, and glibc-devel packages..."
if ! sudo dnf reinstall -y gcc gcc-c++ glibc-devel | tee -a $LOGFILE; then
    log "Error reinstalling gcc, gcc-c++, and glibc-devel packages."
    exit 1
fi

# Install the Development Tools group and display progress in the terminal and in the log
log "Installing Development Tools..."
echo "Installing Development Tools..."
if ! sudo dnf groupinstall "Development Tools" -y | tee -a $LOGFILE; then
    log "Error installing Development Tools."
    exit 1
fi

# Install required qt5 packages and display progress in the terminal and in the log
log "Installing qt5 packages..."
echo "Installing qt5 packages..."
if ! sudo dnf install -y qt5-qtbase-devel qt5-qtx11extras qt5-qttools | tee -a $LOGFILE; then
    log "Error installing qt5 packages."
    exit 1
fi

# Remove unnecessary packages and display progress in the terminal and in the log
log "Removing unnecessary packages..."
echo "Removing unnecessary packages..."
if ! sudo dnf autoremove -y | tee -a $LOGFILE; then
    log "Error removing unnecessary packages."
    exit 1
fi

# Install additional libraries and display progress in the terminal and in the log
log "Installing additional libraries..."
echo "Installing additional libraries..."
if ! sudo dnf install -y qt5-qttools-devel | tee -a $LOGFILE; then
    log "Error installing additional libraries."
    exit 1
fi

# Search for qt5-related packages and display progress in the terminal and in the log
log "Searching for qt5-related packages..."
echo "Searching for qt5-related packages..."
if ! sudo dnf search qt5 | tee -a $LOGFILE; then
    log "Error searching for qt5-related packages."
    exit 1
fi

# Finalize the log file
log "Installation completed - $(date)"
echo "Installation completed - $(date)" >> $LOGFILE
