#!/bin/bash

# Installing Bazel...
sudo apt-get install pkg-config zip g++ zlib1g-dev unzip python
export PATH="$PATH:$HOME/bin"

wget https://github.com/bazelbuild/bazel/releases/download/0.14.1/bazel-0.14.1-installer-linux-x86_64.sh
chmod +x bazel-0.14.1-installer-linux-x86_64.sh
./bazel-0.14.1-installer-linux-x86_64.sh --user
rm bazel-0.14.1-installer-linux-x86_64.sh
