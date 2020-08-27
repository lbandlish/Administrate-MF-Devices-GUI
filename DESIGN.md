# Architecture of this software

## Workflow

- **system-services-show.c** sets up the GUI in its main function. 
- It also creates an avahi service browser to browse services of type "_ipps-system._tcp"
- The service browser listens for events and creates separate service resolvers for all AVAHI_BROWSER_NEW and AVAHI_BROWSER_REMOVE events.
- In case of an AVAHI_BROWSER_NEW event, new IPP System Objects are created (after some checks). For every new system object, 
    - A Get-System-Attributes request is issued using *get_attributes* method in **cupsapi.c** and attributes from the response are recorded.
    - A Get-Printers request is issued using *get_printers* method in **cupsapi.c** which is used to get component printer-uris, and then for every component printer, a Get-Printer-Attributes request is issued using *get_attributes* method and attributes from the responses are recorded to create Printer Objects. These Printer Objects are stored in a list inside their parent System Object.

    All of these new IPP Objects are added to the tree store of the GUI to display them in the application.

- In case of an AVAHI_BROWSER_REMOVE event, after confirming that a System Object no longer exists, it and all of its children Objects are freed and removed from the GUI.

## Files

### system-services-show.c

    Setts up the GUI for the project.
    Detects Browsing and Resolving browser events to find system service instances.


### cupsapi.c

    Contains functions to make IPP Requests and parse IPP responses. 
    Gateway for communication between GUI and IPP Objects.


### administrate_mf_devices_gui.h
 
    Header file which includes all the libraries required to compile the code.
    Also defines structs and enums used throughout this project.

### system-services-show.sh

    Compiles and runs the program.


