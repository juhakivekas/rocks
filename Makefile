#CC=gcc
#CFLAGS=-g -Wall

CPPC=g++
JACKFLAGS= -lpthread -ljack -ldl -lrt -lm
ALSAFLAGS= -lpthread -lasound -ldl -lm
X11FLAGS= -lX11
CPPFLAGS=-g -Wall -std=c++11 -lpthread #$(JACKFLAGS)


all: compile test
compile: objects rubble.out beat.out whiskey.out mineral.out
#slide.out

test:\
	Gamepad_test.out
#	./Gamepad_test.out


#--------EXECUTABLES--------

#X11 graphics demo
slide.out:\
	bin/simple_drawing.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(X11FLAGS)
#	bin/X11_display.o
	 

#Gamepad controlled Framebuffer graphics
mineral.out:\
	bin/Framebuffer.o\
	bin/Gamepad.o\
	bin/mineral.o
	$(CPPC) -o $@ $^ $(CPPFLAGS)

#Gamepad controlled JACK bytebeats
whiskey.out:\
	bin/Beater.o\
	bin/Context.o\
	bin/Bytebeat.o\
	bin/Gamepad.o\
	bin/whiskey.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(JACKFLAGS)

#JACK bytebeats
beat.out:\
	bin/Beater.o\
	bin/Context.o\
	bin/Gamepad.o\
	bin/Bytebeat.o\
	bin/beat.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(JACKFLAGS)

#Gamepad tester/skeleton
rubble.out:\
	bin/Gamepad.o\
	bin/rubble.o
	$(CPPC) -o $@ $^ $(CPPFLAGS)


#--------UNIT TESTS (JOKES)-------

Gamepad_test.out:\
	bin/Gamepad.o\
	bin/Gamepad_test.o	
	$(CPPC) -o $@ $^ $(CPPFLAGS)

#--------OBJECT RULES--------

objects:\
	bin/Gamepad.o\
	bin/Beater.o\
	bin/Context.o\
	bin/Bytebeat.o

bin/%.o: src/%.cpp
	$(CPPC) -c -o $@ $^ $(CPPFLAGS)

bin/%_test.o: test/%_test.cpp
	$(CPPC) -c -o $@ $^ $(CPPFLAGS)

#--------CLEANUP--------
clean:
	rm -rf bin/* *.out
