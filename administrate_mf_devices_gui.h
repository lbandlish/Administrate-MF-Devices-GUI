#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/if.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/mman.h>

#include <gtk/gtk.h>
#include <gtk/gtkx.h>

#include <avahi-core/core.h>
#include <avahi-core/lookup.h>
#include <avahi-common/strlst.h>
#include <avahi-common/domain.h>
#include <avahi-common/error.h>
#include <avahi-glib/glib-watch.h>
#include <avahi-glib/glib-malloc.h>

#include <cups/cups.h>

typedef enum obj_type
{
    SYSTEM_OBJECT,
    PRINTER_OBJECT,
    SCANNER_OBJECT,
    PRINTER_QUEUE
    
} obj_type;

gchar* obj_type_string(int object_type);

struct ObjectSources2
{
    /* Most suited to use with httpConnect2
     * One httpConnect2 request per ObjectSources2 object 
     * (until connection/request is successful)
     */

    gchar *domain_name;
    // AvahiIfIndex interface;
    gchar *host;
    int port;
    int family;
    // AvahiAddress *addr;
};

struct SystemObject2
{
    gchar *object_name;
    obj_type object_type;

    // gchar *domain_name;
    GtkTreeRowReference *tree_ref;

    gchar *uri;
    gchar *sysattr;

    GList *children; /* elements will be printers, queues, scanners under system object. NULL for all except SYSTEM_OBJECT */
    GList *sources;  /* elements will be of type ObjectSources, NULL for all except SYSTEM_OBJECT */

    /* txtuuid may be useful*/
    // gchar *txtuuid
};
