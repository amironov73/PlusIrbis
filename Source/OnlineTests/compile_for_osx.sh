#!/usr/bin/env bash

cp ../irbis/cmake-build-debug/*.dylib .
g++ -std=c++14 OnlineTests.cpp -L . -lPlusIrbis -o OnlineTests
chmod +x OnlineTests