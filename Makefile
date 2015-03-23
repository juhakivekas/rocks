#CC=gcc
#CFLAGS=-g -Wall

CPPC=g++
CPPFLAGS=-g -Wall -std=c++11 -lpthread


all: compile test

test:\
	Gamepad_test.out
	./Gamepad_test.out

compile: objects rubble.out

rubble.out:\
	bin/Gamepad.o\
	bin/rubble.o
	$(CPPC) -o $@ $^ $(CPPFLAGS)

objects:\
	bin/Gamepad.o

Gamepad_test.out:\
	bin/Gamepad.o\
	bin/Gamepad_test.o	
	$(CPPC) -o $@ $^ $(CPPFLAGS)

# object file compilation rules

bin/%.o: src/%.cpp
	$(CPPC) -c -o $@ $^ $(CPPFLAGS)

bin/%_test.o: test/%_test.cpp
	$(CPPC) -c -o $@ $^ $(CPPFLAGS)

# cleanup
clean:
	rm -rf bin/*
