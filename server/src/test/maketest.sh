#!/bin/bash

g++ -c ../udpmulticast.cpp -o udpmulticast.o
g++ -c ../test.cpp -o test.o
g++ udpmulticast.o test.o -o test
