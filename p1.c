#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <sys/mman.h>

GtkWidget	*window1;
GtkWidget   *vbox;
GtkWidget   *sw;
GtkWidget	*label[100];   // Assuming limit of 100 devices
GtkWidget	*button[100];  // Assuming limit of 100 devices
GtkWidget   *hbox[100];

char tmp[1024]; 
int	row;

void on_destroy(); 
void on_row(GtkButton *);


int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv); // init Gtk

	window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window1), 700, 400);
	gtk_window_set_title(GTK_WINDOW(window1), "IPP Devices Available");
	gtk_container_set_border_width(GTK_CONTAINER(window1), 10);


	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(window1), sw);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw),
		GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw),
		GTK_SHADOW_IN);



	vbox = gtk_vbox_new(FALSE, 5);

	gtk_widget_set_valign(vbox, GTK_ALIGN_END);

	// gtk_container_add(GTK_CONTAINER(window1), vbox);
	  gtk_container_add(GTK_CONTAINER(sw), vbox);

	FILE *f1 = fopen("ipp_other_devices.txt", "r");

	if (f1 == NULL ) {
		printf("File finctions.txt not found\n");
		return EXIT_FAILURE;
		}

	row = 0;
	while (1) {
		if (fgets(tmp, 1024, f1) == NULL) {
			fclose(f1);
			break;
			}
		tmp[strlen(tmp)-1] = 0; // remove newline byte

//		A button can be freed by the function 'gtk_container_remove ()'		

		hbox[row] = gtk_hbox_new(FALSE, 3);
		label[row] = gtk_label_new(tmp);
		button[row] = gtk_button_new_with_label ("Manage");

		gtk_widget_set_size_request(label[row], 70, 30);
		gtk_widget_set_halign(label[row], GTK_ALIGN_START);
		gtk_container_add(GTK_CONTAINER(hbox[row]), label[row]);

		gtk_widget_set_halign(button[row], GTK_ALIGN_END);
		gtk_container_add(GTK_CONTAINER(hbox[row]), button[row]);

		gtk_box_pack_start(GTK_BOX(vbox), hbox[row], FALSE, FALSE, 0);
		g_signal_connect(button[row], "clicked", G_CALLBACK(on_row), NULL);
		row ++;
	}

//-----------------------------------

	g_signal_connect(window1, "destroy", G_CALLBACK(on_destroy), NULL);

	gtk_widget_show_all(window1);

	gtk_main();

	return EXIT_SUCCESS;
	}

//......................................................................................


void	on_row(GtkButton *b) {
	printf("You selected: %s\n", gtk_label_get_text (GTK_LABEL(label[10])));
	}

void	on_destroy() { 
		gtk_main_quit();
		}