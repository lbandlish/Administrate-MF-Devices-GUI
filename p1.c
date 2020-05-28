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
GtkWidget	*fixed1;
GtkWidget	*grid1; 
GtkWidget	*label[100];   // Assuming limit of 100 devices
GtkWidget	*button[100];  // Assuming limit of 100 devices
GtkWidget	*view1;
GtkBuilder	*builder; 

void on_destroy(); 
void on_row(GtkButton *);

char tmp[1024]; 
int	row;

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv); // init Gtk

//---------------------------------------------------------------------
// establish contact with xml code used to adjust widget settings
//---------------------------------------------------------------------
 
	builder = gtk_builder_new_from_file ("gladefile.glade");
	window1 = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));

	g_signal_connect(window1, "destroy", G_CALLBACK(on_destroy), NULL);
    gtk_builder_connect_signals(builder, NULL);

	fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
	view1 = GTK_WIDGET(gtk_builder_get_object(builder, "view1"));
	grid1 = GTK_WIDGET(gtk_builder_get_object(builder, "grid1"));

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
		gtk_grid_insert_row (GTK_GRID(grid1), row);

//		The following code will populate the grid with non-clicable labels

//		label[row] = gtk_label_new (tmp);
//		gtk_label_set_justify (GTK_LABEL(label[row]), GTK_JUSTIFY_LEFT);
//		gtk_label_set_xalign (GTK_LABEL(label[row]), 0.0);
//		gtk_grid_attach (GTK_GRID(grid1), label[row], 1, row, 1, 1);

//		The following code will populate the grid with clickable buttons.

//		A button can be freed by the function 'gtk_container_remove ()'

        label[row] = gtk_label_new(tmp);
		button[row] = gtk_button_new_with_label (tmp);
		gtk_button_set_alignment (GTK_BUTTON(button[row]), 0.5, 0.5); 
		gtk_grid_attach (GTK_GRID(grid1), label[row], 1, row, 1, 1);
        gtk_grid_attach (GTK_GRID(grid1), button[row], 2, row, 1, 1);
		g_signal_connect(button[row], "clicked", G_CALLBACK(on_row), NULL);
		row ++;
		}

//-----------------------------------

	gtk_widget_show_all(window1);

	gtk_main();

	return EXIT_SUCCESS;
	}

//......................................................................................


void	on_row(GtkButton *b) {
	printf("You selected: %s\n", gtk_button_get_label (b));
	}

void	on_destroy() { 
		gtk_main_quit();
		}