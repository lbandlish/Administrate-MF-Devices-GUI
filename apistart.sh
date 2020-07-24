#!/bin/bash

gcc -o cupsapi-bin `cups-config --cflags` cupsapi.c dnssdbrowse.c `cups-config --libs` -lavahi-client -lavahi-common
./cupsapi-bin
