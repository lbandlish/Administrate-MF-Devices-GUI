#!/bin/bash

set -e

gcc -Wno-format -o _system-services-show-bin system-services-show.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0 avahi-client avahi-glib avahi-core` -export-dynamic
# gcc -g -Wno-format -o _system-services-show-bin system-services-show.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0 avahi-client avahi-glib avahi-core` -export-dynamic
# G_DEBUG=fatal-criticals
./_system-services-show-bin
