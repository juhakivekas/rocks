#CC=gcc
#CFLAGS=-g -Wall

CPPC=g++
JACKFLAGS= -lpthread -ljack -ldl -lrt -lm
ALSAFLAGS= -lpthread -lasound -ldl -lm
GFXFLAGS= -lX11 -lGL -l GLU
CPPFLAGS=-g -Wall -std=c++11 -lpthread #$(JACKFLAGS)

all:\
	objects\
	rubble.out\
	whiskey.out\
	KOOLLA.out\
	GLX_test.out\
	GLX_animation.out\

test:\
	Gamepad_test.out
#	./Gamepad_test.out


#--------EXECUTABLES--------

#AAVE KOOLLA visuals
KOOLLA.out:\
	bin/KOOLLA.o\
	bin/Gamepad.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(GFXFLAGS)

#GLX animation test from: https://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_Programming_Animations_with_GLX_and_Xlib
GLX_animation.out:\
	bin/GLX_animation.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(GFXFLAGS)

#GLX test from: https://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib
GLX_test.out:\
	bin/GLX_test.o
	$(CPPC) -o $@ $^ $(CPPFLAGS) $(GFXFLAGS)

#Gamepad controlled JACK bytebeats
whiskey.out:\
	bin/Beater.o\
	bin/Context.o\
	bin/Bytebeat.o\
	bin/Gamepad.o\
	bin/BpmClock.o\
	bin/whiskey.o
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
	bin/Bytebeat.o\
	bin/BpmClock.o

bin/%.o: src/%.cpp
	$(CPPC) -c -o $@ $^ $(CPPFLAGS)

bin/%_test.o: test/%_test.cpp
	$(CPPC) -c -o $@ $^ $(CPPFLAGS)

#--------CLEANUP--------
clean:
	rm -rf bin/* *.out
