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

// struct ServiceType;
gchar systemServiceType[] = "_ipps-system._tcp";

// struct Service
// {
//     struct ServiceType *service_type;
//     gchar *service_name;
//     gchar *domain_name;

//     AvahiIfIndex interface;
//     AvahiProtocol protocol;

//     GtkTreeRowReference *tree_ref;
// };

// struct ServiceType
// {
//     gchar *service_type;
//     AvahiSServiceBrowser *browser;

//     GList *services;
//     GtkTreeRowReference *tree_ref;
// };

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
    // gchar *domain_name;

    /* CONSIDER USING ADDRLIST (http_addrlist_t) AS IT CAN BE DIRECTLY USED TO SEND CONNECTION REQUESTS
    * (READ CUPSPM)
    */
    GList *sources; // elements will be of type ObjectSources

    GtkTreeRowReference *tree_ref;

    /* txtuuid may be useful*/
    // gchar *txtuuid
};

static GtkWidget *main_window = NULL;
static GtkTreeView *tree_view = NULL;
static GtkTreeModel *sortmodel = NULL;
static GtkTreeStore *tree_store = NULL;
static GtkWidget *info_label = NULL;
static AvahiServer *server = NULL;
// static AvahiSServiceTypeBrowser *service_type_browser = NULL;
// static GHashTable *service_type_hash_table = NULL;
static GHashTable *system_map_hash_table2 = NULL;
// static AvahiSServiceResolver *service_resolver = NULL;
// static AvahiSServiceResolver *service_resolver2 = NULL;
// static struct Service *current_service = NULL;
static GtkWidget *hbox;
static GtkWidget *lvbox;
static GtkWidget *rvbox;
static GtkWidget *scrollWindow1;
static GtkWidget *scrollWindow2;

// static struct Service *get_service(const gchar *service_type, const gchar *service_name, const gchar *domain_name, AvahiIfIndex interface, AvahiProtocol protocol)
// {
//     struct ServiceType *st;
//     GList *l;

//     if (!(st = g_hash_table_lookup(service_type_hash_table, service_type)))
//         return NULL;

//     for (l = st->services; l; l = l->next)
//     {
//         struct Service *s = l->data;

//         if (s->interface == interface &&
//             s->protocol == protocol &&
//             avahi_domain_equal(s->service_name, service_name) &&
//             avahi_domain_equal(s->domain_name, domain_name))
//             return s;
//     }

//     return NULL;
// }

// static void free_service(struct Service *s)
// {
//     GtkTreeIter iter;
//     GtkTreePath *path;

//     if (current_service == s)
//     {
//         current_service = NULL;

//         if (service_resolver)
//         {
//             avahi_s_service_resolver_free(service_resolver);
//             service_resolver = NULL;
//         }

//         gtk_label_set_text(GTK_LABEL(info_label), "<i>Service removed</i>");
//     }

//     s->service_type->services = g_list_remove(s->service_type->services, s);

//     if ((path = gtk_tree_row_reference_get_path(s->tree_ref)))
//     {
//         gtk_tree_model_get_iter(GTK_TREE_MODEL(tree_store), &iter, path);
//         gtk_tree_path_free(path);
//     }

//     gtk_tree_store_remove(tree_store, &iter);

//     gtk_tree_row_reference_free(s->tree_ref);

//     g_free(s->service_name);
//     g_free(s->domain_name);
//     g_free(s);
// }

// static void service_browser_callback(
//     AVAHI_GCC_UNUSED AvahiSServiceBrowser *b,
//     AvahiIfIndex interface,
//     AvahiProtocol protocol,
//     AvahiBrowserEvent event,
//     const char *service_name,
//     const char *service_type,
//     const char *domain_name,
//     AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
//     AVAHI_GCC_UNUSED void *userdata)
// {

//     if (event == AVAHI_BROWSER_NEW)
//     {
//         struct Service *s;
//         GtkTreeIter iter, piter;
//         GtkTreePath *path, *ppath;
//         gchar iface[256];
//         char name[IF_NAMESIZE];

//         s = g_new(struct Service, 1);
//         s->service_name = g_strdup(service_name);
//         s->domain_name = g_strdup(domain_name);
//         s->interface = interface;
//         s->protocol = protocol;
//         s->service_type = g_hash_table_lookup(service_type_hash_table, service_type);
//         g_assert(s->service_type);

//         s->service_type->services = g_list_prepend(s->service_type->services, s);

//         ppath = gtk_tree_row_reference_get_path(s->service_type->tree_ref);
//         gtk_tree_model_get_iter(GTK_TREE_MODEL(tree_store), &piter, ppath);

//         snprintf(iface, sizeof(iface), "%s %s", if_indextoname(interface, name), avahi_proto_to_string(protocol));

//         gtk_tree_store_append(tree_store, &iter, &piter);
//         gtk_tree_store_set(tree_store, &iter, 0, s->service_name, 1, iface, 2, s, -1);

//         path = gtk_tree_model_get_path(GTK_TREE_MODEL(tree_store), &iter);
//         s->tree_ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(tree_store), path);
//         gtk_tree_path_free(path);

//         /* Expand tree structure on program startup */
//         gtk_tree_view_expand_row(tree_view, ppath, FALSE);
//         gtk_tree_path_free(ppath);
//     }
//     else if (event == AVAHI_BROWSER_REMOVE)
//     {
//         struct Service *s;

//         if ((s = get_service(service_type, service_name, domain_name, interface, protocol)))
//             free_service(s);
//     }
// }
struct ObjectSources2 *is_system_object_present2(
    GList *sources,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AVAHI_GCC_UNUSED const char *domain_name,
    const char *host_name,
    uint16_t port)
{

    // printf("%d %s\n\n", addr->data, addr->data);

    printf("\tis_system_object_present2: ");

    for (GList *l = sources; l; l = l->next)
    {
        struct ObjectSources2 *s = l->data;

        if (s->family == protocol &&
            s->port == port &&
            /* possible bug in next line (inputs are not domains) */
            avahi_domain_equal(s->host, host_name) &&
            avahi_domain_equal(s->domain_name, domain_name))
        {
            printf("Yes\n");
            return s;
        }
    }

    printf("No\n");
    return NULL;
}

static void remove_from_system_object2(
    struct SystemObject2 *so,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AVAHI_GCC_UNUSED const char *domain_name,
    const char *host_name,
    const AvahiAddress *addr,
    uint16_t port)
{
    struct ObjectSources2 *source = NULL;

    if (source = is_system_object_present2(so->sources, protocol, domain_name, host_name, port))
    {
        so->sources = g_list_remove(so->sources, source);
    }
}

static void add_to_system_object2(
    struct SystemObject2 *so,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AVAHI_GCC_UNUSED const char *domain_name,
    const char *host_name,
    const AvahiAddress *addr,
    uint16_t port)
{
    struct ObjectSources2 *source = NULL;

    if (source = is_system_object_present2(so->sources, protocol, domain_name, host_name, port))
    {

        /* Object already added */
        return;
    }

    else
    {
        //   char portString[10];
        //   sprintf(portString, "%d", port);
        // http_addrlist_t* addrlist = httpAddrGetList(host_name, protocol, portString);
        // int count = 0;
        // for (http_addrlist_t * listItem = addrlist; listItem = listItem->next; listItem != NULL) {
        //     count++;
        // }

        // printf ("Count is %d\n\n\n", count);

        source = g_new(struct ObjectSources2, 1);
        source->domain_name = g_strdup(domain_name);
        source->host = g_strdup(host_name);
        source->port = port;
        source->family = protocol;
        // /* DO SOMETHING TO COPY ADDR INTO SOURCE->ADDR */

        // // copyAddr(source->addr, addr);
        // source->addr = g_new(struct AvahiAddress, 1);

        so->sources = g_list_prepend(so->sources, source);
    }
}

static void service_remove_resolver_callback2(
    AvahiSServiceResolver *r,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AvahiResolverEvent event,
    AVAHI_GCC_UNUSED const char *service_name,
    AVAHI_GCC_UNUSED const char *service_type,
    AVAHI_GCC_UNUSED const char *domain_name,
    const char *host_name,
    const AvahiAddress *a,
    uint16_t port,
    AvahiStringList *txt,
    AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
    void *userdata)
{

    /* Can add code to quit when encountering AVAHI_BROWSER_FAILURE event */

    /* Need to create separate hash map for printers.
     * (all of these maps can be stored in some sort of list)
     * service_type is passed and this can be some sort of key
     * in this list and entry corresponding to this key will be map.
     */

    if (!service_name) {
        printf("Error: empty service_name passed to resolver\n");
    }

    else if (event == AVAHI_RESOLVER_FOUND)
    {
        struct SystemObject2 *so;
        printf("\t(REMOVE) Looking in ht for: %s\n", service_name);

        if (so = g_hash_table_lookup(system_map_hash_table2, service_name))
        {

            remove_from_system_object2(so, interface, protocol, domain_name, host_name, a, port);

            /* Check if system_object is empty */
            if (so->sources == NULL)
            {

                /* Add code to remove printers and other components of system */
                printf("Check 1\n");
                GtkTreeIter iter;
                GtkTreePath *path;

                if ((path = gtk_tree_row_reference_get_path(so->tree_ref)))
                {
                printf("Check 2\n");
                    gtk_tree_model_get_iter(GTK_TREE_MODEL(tree_store), &iter, path);
                printf("Check 3\n");
                    gtk_tree_path_free(path);
                printf("Check 4\n");

                    /* See avahi-discover-dnssd.c code, there this function is used outside this if block */
                    gtk_tree_store_remove(tree_store, &iter);
                printf("Check 4\n");

                }

                gtk_tree_row_reference_free(so->tree_ref);
                printf("Check 5\n");
                g_free(so->object_name);
                printf("Check 6\n");
                g_free(so);
                printf("Check 7\n");

            }
        }
    }

    else if (event == AVAHI_RESOLVER_FAILURE)
    {
        printf("Error: Failed to resolve: %s\n", avahi_strerror(avahi_server_errno(server)));
    }

    avahi_s_service_resolver_free(r);
}

static void service_new_resolver_callback2(
    AvahiSServiceResolver *r,
    AVAHI_GCC_UNUSED AvahiIfIndex interface,
    AVAHI_GCC_UNUSED AvahiProtocol protocol,
    AvahiResolverEvent event,
    AVAHI_GCC_UNUSED const char *service_name,
    AVAHI_GCC_UNUSED const char *service_type,
    AVAHI_GCC_UNUSED const char *domain_name,
    const char *host_name,
    const AvahiAddress *a,
    uint16_t port,
    AvahiStringList *txt,
    AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
    void *userdata)
{

    /* Can add code to quit when encountering AVAHI_BROWSER_FAILURE event */

    /* Need to create separate hash map for printers.
     * (all of these maps can be stored in some sort of list)
     * service_type is passed and this can be some sort of key
     * in this list and entry corresponding to this key will be map.
     */


    if (!service_name) {
        printf("Error: empty service_name passed to resolver\n");
    }


    else if (event == AVAHI_RESOLVER_FOUND)
    {
        printf("2. Resolver\n");
        struct SystemObject2 *so;
        GtkTreePath *path = NULL;
        GtkTreeIter iter;

        printf("\t(NEW) Looking in ht for: %s\n", service_name);

        if (!(so = g_hash_table_lookup(system_map_hash_table2, service_name)))
        {
            printf("3. create System Object\n");
            so = g_new(struct SystemObject2, 1);
            so->object_name = g_strdup(service_name);
            so->sources = NULL;
            so->tree_ref = NULL;

            gtk_tree_store_append(tree_store, &iter, NULL);
            gtk_tree_store_set(tree_store, &iter, 0, so->object_name, 1, avahi_proto_to_string(protocol), 2, so, -1);
            path = gtk_tree_model_get_path(GTK_TREE_MODEL(tree_store), &iter);
            so->tree_ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(tree_store), path);
            gtk_tree_path_free(path);

            printf("Inserting in ht: %s\n", so->object_name);

            g_hash_table_insert(system_map_hash_table2, so->object_name, so);

            if (g_hash_table_lookup(system_map_hash_table2, service_name))
            {
                printf("Inserted in ht.\n");
            }
            /* Add code to check and display printers and other components of system */
        }

        add_to_system_object2(so, interface, protocol, domain_name, host_name, a, port);
    }

    else if (event == AVAHI_RESOLVER_FAILURE)
    {
        printf("Error: Failed to resolve: %s\n", avahi_strerror(avahi_server_errno(server)));
    }

    avahi_s_service_resolver_free(r);
}

static void service_browser_callback2(
    AvahiSServiceBrowser *b,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiBrowserEvent event,
    const char *service_name /**< Service name, e.g. "Lennart's Files" */,
    const char *service_type /**< DNS-SD type, e.g. "_http._tcp" */,
    const char *domain_name /**< Domain of this service, e.g. "local" */,
    AvahiLookupResultFlags flags, /**< Lookup flags */
    void *userdata)
{

    printf("\n1. service browser\n");

    if (event == AVAHI_BROWSER_NEW) {
        printf ("Browser: AVAHI_BROWSER_NEW\n");
        avahi_s_service_resolver_new(server, interface, protocol, service_name, service_type, domain_name, AVAHI_PROTO_UNSPEC, 0, service_new_resolver_callback2, b);
    }

    else if (event == AVAHI_BROWSER_REMOVE) {
        printf ("Browser: AVAHI_BROWSER_REMOVE\n");
        avahi_s_service_resolver_new(server, interface, protocol, service_name, service_type, domain_name, AVAHI_PROTO_UNSPEC, 0, service_remove_resolver_callback2, b);
    }

    else {
        printf ("Browser: Non NEW/REMOVE event.\n");
    }

}

// static void service_type_browser_callback(
//     AVAHI_GCC_UNUSED AvahiSServiceTypeBrowser *b,
//     AVAHI_GCC_UNUSED AvahiIfIndex interface,
//     AVAHI_GCC_UNUSED AvahiProtocol protocol,
//     AvahiBrowserEvent event,
//     const char *service_type,
//     const char *domain,
//     AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
//     AVAHI_GCC_UNUSED void *userdata)
// {

//     struct ServiceType *st;
//     GtkTreePath *path;
//     GtkTreeIter iter;

//     if (event != AVAHI_BROWSER_NEW)
//         return;

//     if (g_hash_table_lookup(service_type_hash_table, service_type))
//         return;

//     st = g_new(struct ServiceType, 1);
//     st->service_type = g_strdup(service_type);
//     st->services = NULL;

//     gtk_tree_store_append(tree_store, &iter, NULL);
//     gtk_tree_store_set(tree_store, &iter, 0, st->service_type, 1, "", 2, NULL, -1);

//     path = gtk_tree_model_get_path(GTK_TREE_MODEL(tree_store), &iter);
//     st->tree_ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(tree_store), path);
//     gtk_tree_path_free(path);

//     g_hash_table_insert(service_type_hash_table, st->service_type, st);

//     st->browser = avahi_s_service_browser_new(server, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, st->service_type, domain, 0, service_browser_callback, NULL);
// }

// static void update_label(struct Service *s, const gchar *hostname, const AvahiAddress *a, guint16 port, AvahiStringList *txt)
static void update_label2(struct SystemObject2 *so)
{


    if (so == NULL)
    {
        printf("Update Label insuccessful - System object empty (NULL)\n");
        return;
    }
    gchar t[512];

    if (so->object_name != NULL)
    {
        snprintf(t, sizeof(t),
                 "<b>System Object Name:</b> %s\n",
                 so->object_name);
    }

    snprintf(t + strlen(t), sizeof(t) - strlen(t), "<b>Sources: </b>\n");

    for (GList *l = so->sources; l; l = l->next)
    {
        struct ObjectSources2 *s = l->data;
        snprintf(t + strlen(t), sizeof(t) - strlen(t),
                 "<b>\t Domain name:</b> %s\n"
                 "<b>\t Host:</b> %s\n"
                 "<b>\t Port:</b> %d\n"
                 "<b>\t Family(Protocol):</b> %s\n\n",
                 s->domain_name,
                 s->host,
                 s->port,
                 avahi_proto_to_string(s->family));
    }

    printf("Label Content: \n%s\n", t);
    gtk_label_set_markup(GTK_LABEL(info_label), t);
}

// static void service_resolver_callback(
//     AvahiSServiceResolver *r,
//     AVAHI_GCC_UNUSED AvahiIfIndex interface,
//     AVAHI_GCC_UNUSED AvahiProtocol protocol,
//     AvahiResolverEvent event,
//     AVAHI_GCC_UNUSED const char *name,
//     AVAHI_GCC_UNUSED const char *type,
//     AVAHI_GCC_UNUSED const char *domain,
//     const char *host_name,
//     const AvahiAddress *a,
//     uint16_t port,
//     AvahiStringList *txt,
//     AVAHI_GCC_UNUSED AvahiLookupResultFlags flags,
//     void *userdata)
// {

//     struct Service *s;
//     g_assert(r);

//     if (!(s = get_service_on_cursor()) || userdata != s)
//     {
//         g_assert(r == service_resolver);
//         avahi_s_service_resolver_free(service_resolver);
//         service_resolver = NULL;
//         return;
//     }

//     if (event == AVAHI_RESOLVER_FAILURE)
//     {
//         char t[256];
//         snprintf(t, sizeof(t), "<i>Failed to resolve: %s.</i>", avahi_strerror(avahi_server_errno(server)));
//         gtk_label_set_markup(GTK_LABEL(info_label), t);
//     }
//     else if (event == AVAHI_RESOLVER_FOUND)
//         update_label(s, host_name, a, port, txt);
// }

static struct SystemObject2 *get_system_object_on_cursor2(void)
{
    GtkTreePath *path;
    GtkTreePath *true_path;
    struct SystemObject2 *so;
    GtkTreeIter iter;

    gtk_tree_view_get_cursor(tree_view, &path, NULL);

    true_path = gtk_tree_model_sort_convert_path_to_child_path(GTK_TREE_MODEL_SORT(sortmodel),
                                                               path);

    if (!true_path)
        return NULL;

    gtk_tree_model_get_iter(GTK_TREE_MODEL(tree_store), &iter, true_path);
    gtk_tree_model_get(GTK_TREE_MODEL(tree_store), &iter, 2, &so, -1);
    gtk_tree_path_free(path);
    gtk_tree_path_free(true_path);
    return so;
}

static void tree_view_on_cursor_changed2(AVAHI_GCC_UNUSED GtkTreeView *tv, AVAHI_GCC_UNUSED gpointer userdata)
{
    struct SystemObject2 *so;

    if (!(so = get_system_object_on_cursor2()))
        return;

    printf("so contents: %s\n", so->object_name);

    update_label2(so);
}

static gboolean main_window_on_delete_event(AVAHI_GCC_UNUSED GtkWidget *widget, AVAHI_GCC_UNUSED GdkEvent *event, AVAHI_GCC_UNUSED gpointer user_data)
{
    gtk_main_quit();
    return FALSE;
}

int main(int argc, char *argv[])
{
    AvahiServerConfig config;
    GtkTreeViewColumn *col1;
    GtkTreeViewColumn *col2;
    gint error;
    AvahiGLibPoll *poll_api;
    gint window_width = 700;
    gint window_height = 400;

    gtk_init(&argc, &argv);

    avahi_set_allocator(avahi_glib_allocator());

    poll_api = avahi_glib_poll_new(NULL, G_PRIORITY_DEFAULT);

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(main_window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(main_window), window_width, window_height);
    gtk_window_set_title(GTK_WINDOW(main_window), "IPP Device Management");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    g_signal_connect(main_window, "delete-event", (GCallback)main_window_on_delete_event, NULL);

    hbox = gtk_hbox_new(TRUE, 5);
    lvbox = gtk_vbox_new(FALSE, 5);
    rvbox = gtk_vbox_new(FALSE, 5);

    scrollWindow1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWindow1),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollWindow1),
                                        GTK_SHADOW_IN);

    scrollWindow2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollWindow1),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrollWindow1),
                                        GTK_SHADOW_IN);

    info_label = gtk_label_new("Select a Device from the list");

    gtk_container_add(GTK_CONTAINER(main_window), hbox);
    gtk_box_pack_start(GTK_BOX(hbox), lvbox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), rvbox, TRUE, TRUE, 0);

    tree_view = GTK_TREE_VIEW(gtk_tree_view_new());
    g_signal_connect(GTK_WIDGET(tree_view), "cursor-changed", (GCallback)tree_view_on_cursor_changed2, NULL);

    gtk_container_add(GTK_CONTAINER(lvbox), scrollWindow1);
    gtk_container_add(GTK_CONTAINER(scrollWindow1), GTK_WIDGET(tree_view));
    gtk_container_add(GTK_CONTAINER(rvbox), scrollWindow2);
    gtk_container_add(GTK_CONTAINER(scrollWindow2), info_label);

    tree_store = gtk_tree_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_POINTER);
    sortmodel = gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(tree_store));

    gtk_tree_view_set_model(tree_view, GTK_TREE_MODEL(sortmodel));
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Name", gtk_cell_renderer_text_new(), "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(tree_view, -1, "Protocol", gtk_cell_renderer_text_new(), "text", 1, NULL);
    gtk_tree_view_column_set_sort_column_id(gtk_tree_view_get_column(tree_view, 0), 0);
    gtk_tree_view_column_set_sort_column_id(gtk_tree_view_get_column(tree_view, 1), 1);
    gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(sortmodel), 0, GTK_SORT_ASCENDING);

    /* Need to see if next formattings are needed or not */

    gtk_tree_view_column_set_resizable(col1 = gtk_tree_view_get_column(tree_view, 0), TRUE);
    gtk_tree_view_column_set_resizable(col2 = gtk_tree_view_get_column(tree_view, 1), TRUE);

    gtk_tree_view_column_set_sizing(col1, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(col1, window_width / 4);

    gtk_tree_view_column_set_sizing(col1, GTK_TREE_VIEW_COLUMN_FIXED);
    gtk_tree_view_column_set_fixed_width(col1, window_width / 4);

    gtk_tree_view_column_set_expand(col1, TRUE);
    gtk_tree_view_column_set_expand(col2, TRUE);

    /* This hashing function is for when domain_name is the key, we need separate hash function. */

    // service_type_hash_table = g_hash_table_new((GHashFunc)avahi_domain_hash, (GEqualFunc)avahi_domain_equal);
    system_map_hash_table2 = g_hash_table_new((GHashFunc)avahi_domain_hash, (GEqualFunc)avahi_domain_equal);

    /* Need to figure out use of config or get rid of it */

    avahi_server_config_init(&config);
    config.publish_hinfo = config.publish_addresses = config.publish_domain = config.publish_workstation = FALSE;
    server = avahi_server_new(avahi_glib_poll_get(poll_api), &config, NULL, NULL, &error);
    avahi_server_config_free(&config);

    g_assert(server);

    // service_type_browser = avahi_s_service_type_browser_new(server, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, argc >= 2 ? argv[1] : NULL, 0, service_type_browser_callback, NULL);
    avahi_s_service_browser_new(server, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, systemServiceType, argc >= 2 ? argv[1] : NULL, 0, service_browser_callback2, NULL);
    gtk_widget_show_all(main_window);

    gtk_main();

    avahi_server_free(server);
    avahi_glib_poll_free(poll_api);

    return 0;
}
