# Administrate-MF-Devices-GUI

GTK-based tool to administer IPP Multi-Function devices using IPP System Service.

## Introduction

Currently, the way to manage network printers is using web interfaces, which has its
advantages in that no software needs to be installed on the system and hence there’s
operating system independence. But these interfaces are manufacturer-dependent,
and hence automation scripts need to be written separately for different printers.
Also, local print queues are managed using local printer tools, while administration
interfaces runon browsers. To eliminate these issues, PWG introduced IPP System
Service Standard which us es IPP requests to poll printer capabilities and configure the
printer, thus providing a universal solution for IPP-compliant printers.
This project involves buildinga printer configuration system with a GTK-based GUI
that can be embedded in the GNOME Control Center to bring all printer management
control in one place.


## Work Done

The program checks for running IPP System Services by using avahi based service browsers and resolvers. The information received is then used to create IPP System Object uri(s). Then using IPP Requests, attributes of this System Object and its component devices are obtained and stored in memory.

All of these objects are then displayed in the GUI in a hierarchical fashion using TreeViews. Sorting functionality is also implemented. Selecting any object displays its attributes in the sidebar. The program updates the object list in real-time by listening to service browser events.

The program has been tested with multiple System Object and Printer Object instances.

## Future Work

Support for a few more IPP Requests needs to be added to the program at this point. Also currently, the program supports IPP System Objects and Printer Objects. Support for IPP Scanner and Print Queues needs to be added.

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

## Testing and Running

There must be discoverable IPP System Services running on the network. [Pappl](https://github.com/michaelrsweet/pappl) can be used for this. 

Clone, configure and make pappl. Then run a Test System Service by executing testpappl file inside testsuite directory (See testpappl --help to see what flags can be used.):

```
pappl/testsuite/testpappl
```

Now we can run this program. Execute (inside this project's directory):
```
./system-services-show.sh
```
