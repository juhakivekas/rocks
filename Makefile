#CC=gcc
#CFLAGS=-g -Wall

CPPC=g++
JACKFLAGS= -lpthread -ljack -ldl -lrt -lm
ALSAFLAGS= -lpthread -lasound -ldl -lm
CPPFLAGS=-g -Wall -std=c++11 -lpthread $(JACKFLAGS)


all: compile test

test:\
	Gamepad_test.out
#	./Gamepad_test.out

compile: objects rubble.out beat.out whiskey.out

whiskey.out:\
	bin/Jack_beater.o\
	bin/Bytebeat.o\
	bin/Gamepad.o\
	bin/whiskey.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(JACKFLAGS)

beat.out:\
	bin/Jack_beater.o\
	bin/Bytebeat.o\
	bin/beat.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(JACKFLAGS)

rubble.out:\
	bin/Gamepad.o\
	bin/rubble.o
	$(CPPC) -o $@ $^ $(CPPFLAGS)

objects:\
	bin/Gamepad.o\
	bin/Jack_beater.o\
	bin/Bytebeat.o

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
