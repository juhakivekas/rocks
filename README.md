RUBBLE
------

How much dirty stuff do i have to mash togheter to get something awesome? How about i try with a Rumblepad, RasPI, and JACK audio.

This is a personal C++ project and a piece of media art. As a piece of interactive software meant for performance this could be considered a musical/visual instrument. I'll possibly be using this as a showcase piece in job interviews due to the diversity of functionality and several layers of abstraction handeld.

Building
--------



	make all
	
	

Running
-------

First start the JACK server:

	jackd -d alsa

Then start the main program, currently beat.out

	./beat.out

Now the JACK ports have to be connected to the system output sound. You can do this with the graphical tool QJackCtl. Now sound should be playing!


## Dependencies

The project depends on [jack audio](http://www.jackaudio.org/) for audio interfaces. These need to be installed in order to compile and run teh software.

install jack audio library development files:

	sudo apt-get install libjack-dev
	
install jack audio daemon:

	sudo apt-get install jackd1
	
During the installation of jackd you will be prompted whether to add realtime permissions to the audio group. 
You will need to log out and log back in to you user account for the permission changes to become effective.