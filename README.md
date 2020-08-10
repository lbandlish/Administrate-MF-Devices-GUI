# Administrate-MF-Devices-GUI

GTK-based tool to administer IPP MF devices using IPP System Service

## Installation
gtk-3+ dev toolkit is required to build this program. 
Also need avahi-client, avahi-glib, and avahi-core libraries for DNS-SD and glib/gtk integration

#### Ubuntu
```
sudo apt-get install libgtk-3-dev
sudo apt-get install libavahi-client-dev
sudo apt-get install libavahi-glib-dev
sudo apt-get install libavahi-core-dev
```
## Current Progress

Currently, the program checks for running IPP System Services by using avahi based service browsers and resolvers. These System Objects are then displayed in the GUI, using treeviews and sorting options.

## Testing

There must be discoverable IPP System Services running on the network. [Pappl](https://github.com/michaelrsweet/pappl) can be used for this. 

Clone, configure and make pappl. Then run a Test System Service by executing testpappl file inside testsuite directory:

```
testsuite/testpappl
```

Now we can run this program to find the system service advertised by pappl. Execute:
```
./system-services-show.sh
```
