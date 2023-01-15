#!/bin/sh
g++ parent.cpp -o parent
g++ -pthread line.cpp -o line
./parent