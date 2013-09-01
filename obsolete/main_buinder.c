#include <stdlib.h>
#include <gtk/gtk.h>
#include "core.c"
#include "config.h"

#define UI_FILE "gtk_ex.ui"
#define TOP_WINDOW "window"

/* struct treeItem_estate
{
  gint id;
  gchar *name;
  gchar *addr;
  gchar *propertyDevr; //Developer
  gchar *propertyMC;   //Management Company
  struct treeItem_bldn *children; //its children buildings
};

struct treeItem_bldn
{
    gint id;
    gint unitCount;
    gint storeyCount;
    struct treeItem_cell *child;
};

struct treeItem_cell
{
    gint id;
    gint cellStruct;
    gfloat area;
    gfloat shareRate;
    gfloat price;
}; */

static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

typedef struct _Private Private;
static struct _Private
{
	/* ANJUTA: Widgets declaration for gtk_ex.ui - DO NOT REMOVE */
};

static struct Private* priv = NULL;

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/gtk_ex.ui" */
#define TOP_WINDOW "window"

/* Signal handlers */
/* Note: These may not be declared static because signal autoconnection
 * only works with non-static methods
 */

/* Called when the window is closed */
void
destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit ();
}

static GtkWidget*
create_window (void)
{
	GtkWidget *window;
	GtkBuilder *builder;
	GError* error = NULL;

	/* Load UI from file */
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, NULL);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder, TOP_WINDOW));
        if (!window)
        {
                g_critical ("Widget \"%s\" is missing in file %s.",
				TOP_WINDOW,
				UI_FILE);
        }

	priv = g_malloc (sizeof (struct _Private));
	/* ANJUTA: Widgets initialization for gtk_ex.ui - DO NOT REMOVE */

	g_object_unref (builder);

	return window;
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    gtk_init (&argc, &argv);

	window = create_window ();
	gtk_widget_show (window);
	gtk_main ();
	g_free (priv);
  return 0;
}
