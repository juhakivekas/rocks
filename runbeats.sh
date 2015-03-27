#!/bin/bash

jackd -d alsa &
./whiskey.out &
jack_connect beater:1 system:playback_1
jack_connect beater:1 system:playback_2

exit
