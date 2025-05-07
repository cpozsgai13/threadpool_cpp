#!/bin/bash

g++ -std=c++17 $1.cpp -g -lpthread -lboost_system -lboost_filesystem -o $1
