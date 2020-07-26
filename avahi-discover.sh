#!/bin/bash

gcc -Wno-format -o avahi-discover-dnssd-bin avahi-discover-dnssd.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0 avahi-client avahi-glib avahi-core` -export-dynamic
./avahi-discover-dnssd-bin

# gcc -o glib-integration-bin glib-integration.c `pkg-config --libs --cflags avahi-client avahi-glib`
# ./glib-integration-bin