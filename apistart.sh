#!/bin/bash

gcc -o cupsapi-bin `cups-config --cflags` cupsapi.c `cups-config --libs`
./cupsapi-bin
