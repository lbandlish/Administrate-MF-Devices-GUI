#!/bin/bash

gcc -Wno-format -o show-MF-devices-bin show-MF-devices.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0 avahi-client avahi-glib avahi-core` -export-dynamic
./show-MF-devices-bin
