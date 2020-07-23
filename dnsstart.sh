#!/bin/bash

gcc -o dnssdbrowse-bin dnssdbrowse.c -lavahi-client -lavahi-common
./dnssdbrowse-bin
