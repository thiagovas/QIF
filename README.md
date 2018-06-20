# QIF (Quantitative Information Flow)
[![Build Status](https://api.travis-ci.com/vaz985/QIF.svg?token=xv5Q8ZwDSsDycZ4aao1n&branch=master)](https://travis-ci.com/vaz985/QIF)

In this repository, you can find some QIF concepts implemented.<br/>


## Playing with the lib
Run the *setup.sh* script before playing with the lib.<br/>
This script installs every package required.


## On Compiling
On this repo, we use [Bazel](https://bazel.build) for automation of building and testing.<br/>
Each building rule has the exact same name of their correspondent file. e.g.: For building channel/channel.cpp, just run *bazel build channel/channel*.


## On the tests
The unit tests are built with the aid of [gtest](https://github.com/google/googletest), and they are kept on the tests/ folder.
