#!/bin/sh

rm /home/ibraheem/redfifo
rm /home/ibraheem/greenfifo
g++ -g parent.cpp -o parent
g++ -g child.cpp -o child
g++ -g draw.cpp -o draw -std=c++11 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

./parent 1