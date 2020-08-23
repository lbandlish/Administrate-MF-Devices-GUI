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

The program checks for running IPP System Services by using avahi based service browsers and resolvers. Then using IPP Requests, attributes of System Service and its component MF devices are obtained.

All of these objects are then displayed in the GUI in a hierarchical fashion using TreeViews. Sorting functionality is also implemented. Selecting any object displays its attributes in the sidebar. The program updates the object list in real-time by listening to service browser events.

## Testing

There must be discoverable IPP System Services running on the network. [Pappl](https://github.com/michaelrsweet/pappl) can be used for this. 

Clone, configure and make pappl. Then run a Test System Service by executing testpappl file inside testsuite directory (See testpappl --help to see what flags can be used.):

```
testsuite/testpappl
```

Now we can run this program to find the system service advertised by pappl and its component printers. Execute:
```
./system-services-show.sh
```
