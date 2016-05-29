# Makefile for the connect four bot

all: main.cpp connectfour.cpp
		g++ -std=c++1y -O2 -static -Isrc main.cpp connectfour.cpp -lm
