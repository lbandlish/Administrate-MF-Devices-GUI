# Administrate-MF-Devices-GUI

GTK-based tool to administer IPP MF devices using IPP System Service

## Installation
gtk-3+ dev toolkit is required to build this program. 
Also need avahi-client, avahi-glib, and avahi-core libraries for DNS-SD and glib/gtk integration

#### Ubuntu
`sudo apt-get install libgtk-3-dev`
`sudo apt-get install libavahi-client-dev`
`sudo apt-get install libavahi-glib-dev`
`sudo apt-get install libavahi-core-dev`

## Current Progress

The front page of the application is complete. 

Implemented using GtkTreeView to provide functionality to sort by any column.