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
// #include <assert.h>
// #include <avahi-client/client.h>
// #include <avahi-client/lookup.h>
// #include <avahi-common/simple-watch.h>
// #include <avahi-common/malloc.h>

static int check_if_cups_request_error()
{

	if (cupsLastError() >= IPP_STATUS_ERROR_BAD_REQUEST)
	{
		/* request failed */
		printf("Request failed: %s\n", cupsLastErrorString());
		return 1;
	}

	else
	{
		printf("Request succeeded!\n");
		return 0;
	}
}

typedef struct add_attribute_data
{

	ipp_t *response;
	gchar *buff;
	int buff_size;

} add_attribute_data;

static void add_attribute(

	char *attr_name,
	ipp_tag_t value_tag,
	add_attribute_data data)
{

	ipp_t *response = data.response;
	gchar *buff = data.buff;
	int buff_size = data.buff_size;

	ipp_attribute_t *attr;

	if ((attr = ippFindAttribute(response, attr_name,
								 value_tag)) != NULL)
	{

		const gchar *attr_val;

		if (value_tag == IPP_TAG_ENUM)
		{
			attr_val = ippEnumString(attr_name, ippGetInteger(attr, 0));
		}

		else
		{
			attr_val = ippGetString(attr, 0, NULL);
		}

		snprintf(buff + strlen(buff), buff_size - strlen(buff),
				 "<b>\t %s:</b> = %s\n",
				 attr_name,
				 attr_val);
	}

	else
	{
		snprintf(buff + strlen(buff), buff_size - strlen(buff),
				 "<b>\t %s:</b> = unknown\n",
				 attr_name);
	}
}

int get_system_attributes(
	http_t *http,
	char *uri,
	gchar *buff,
	int buff_size)
{
	ipp_t *request = ippNewRequest(IPP_OP_GET_SYSTEM_ATTRIBUTES);
	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "system-uri", NULL, uri);
	ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());
	// ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requested-attributes", NULL, "system-dns-sd-name");
	// ippAddStrings(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD, "requested-attributes", (int)(sizeof(sysattrs) / sizeof(sysattrs[0])), NULL, sysattrs);

	ipp_t *response = cupsDoRequest(http, request, "/ipp/system");

	if (check_if_cups_request_error())
	{
		return 0;
	}

	ipp_attribute_t *attr;
	strcpy(buff, "");

	add_attribute_data data = {response, buff, buff_size};

	add_attribute("system-state", IPP_TAG_ENUM, data);
	add_attribute("system-make-and-model", IPP_TAG_TEXT, data);
	add_attribute("system-dns-sd-name", IPP_TAG_NAME, data);
	add_attribute("system-location", IPP_TAG_TEXT, data);
	add_attribute("system-geo-location", IPP_TAG_URI, data);

	return 1;

	// if ((attr = ippFindAttribute(response, "system-state-reasons",
	//                             IPP_TAG_KEYWORD)) != NULL)
	// {
	//     int i, count = ippGetCount(attr);

	//     puts("system-state-reasons=");
	//     for (i = 0; i < count; i++)
	//     printf("    %s\n", ippGetString(attr, i, NULL));
	// }

	// if_get_printer_attributes(response);
	// if_get_system_attributes(response);
	// if_get_printers(response);
}