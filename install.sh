#!/bin/bash

INSTALL_DIR="/usr/local/bin"

if [ "$(id -u)" != "0" ]; then
  echo "Error: This script must be run as root or with sudo."
  exit 1
fi

mkdir -p build && cd build

cmake -G "Unix Makefiles" ..

make

if [ $? -eq 0 ]; then
  echo "Build successful. Installing the application..."

  cp examples/checksum-gen "$INSTALL_DIR/"

  if [ $? -eq 0 ]; then
    echo "Installation complete. The application is now available in $INSTALL_DIR."
    exit 0
  else
    echo "Error: Failed to copy the executable to $INSTALL_DIR."
    exit 1
  fi
else
  echo "Error: Build failed. Installation aborted."
  exit 1
fi
