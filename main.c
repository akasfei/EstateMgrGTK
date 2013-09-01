#include <gtk/gtk.h>
#include <stdlib.h>
#include "core.h"
#include "core_io.h"
#include "core_EST.h"
#include "core_CEL.h"
#include "core_BDN.h"
#include "core_query.h"

int
main (int argc, char **argv)
{
  GtkWidget *view;
  GtkWidget *vbox, *hbox1, *hbox2;
  GtkWidget *button_add, *button_delete, *button_open, *button_save, *button_search_dialog, *button_help, *button_exit;
  GtkWidget *scrolled_window;

  /* System Initialization */
  gtk_init (&argc, &argv);
  IO_initialization();
  EST_array_link ();

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 940, 600);
  gtk_window_set_title (GTK_WINDOW (window), "Estate Manager w/ GTK+");
  g_signal_connect (window, "delete_event", gtk_main_quit, NULL); /* exit main thread when clicking red 'x' */

  /* create containers */
  vbox = gtk_vbox_new(FALSE, 8);
  hbox1 = gtk_hbox_new(TRUE, 5);
  hbox2 = gtk_hbox_new(TRUE, 5);

  /* Initialize buttons */
  button_add = gtk_button_new_with_label ("Add row");
  button_delete = gtk_button_new_with_label ("Remove row");
  button_open = gtk_button_new_with_label ("Open");
  button_search_dialog = gtk_button_new_with_label ("Search");
  button_save = gtk_button_new_with_label ("Save");
  button_help = gtk_button_new_with_label ("Help");
  button_exit = gtk_button_new_with_label ("Exit");

  g_signal_connect (button_exit, "clicked", gtk_main_quit, NULL);

  /* Create main view */
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
                                       GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);
  gtk_widget_set_usize (scrolled_window, 920, 520);
  view = EST_create_view_and_model ();

  /* Add widgets into their containers */
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_container_add (GTK_CONTAINER (vbox), hbox1);
  gtk_container_add (GTK_CONTAINER (vbox), hbox2);
  gtk_container_add (GTK_CONTAINER (hbox1), button_add);
  gtk_container_add (GTK_CONTAINER (hbox1), button_delete);
  gtk_container_add (GTK_CONTAINER (hbox2), button_open);
  gtk_container_add (GTK_CONTAINER (hbox2), button_search_dialog);
  gtk_container_add (GTK_CONTAINER (hbox2), button_save);
  gtk_container_add (GTK_CONTAINER (hbox2), button_help);
  gtk_container_add (GTK_CONTAINER (hbox2), button_exit);

  /* Connect signals */
  g_signal_connect (button_add, "clicked",
                    G_CALLBACK (EST_add_item), GTK_TREE_MODEL (gtk_tree_view_get_model(GTK_TREE_VIEW (view))));
  g_signal_connect (button_delete, "clicked",
                    G_CALLBACK (EST_remove_item), view);
  g_signal_connect (button_open, "clicked",
                    G_CALLBACK (BDN_open_window), view);
  g_signal_connect (button_save, "clicked",
                    G_CALLBACK (IO_save), NULL);
  g_signal_connect (button_search_dialog, "clicked",
                    G_CALLBACK (QRY_main), window);

  gtk_widget_show_all (window);
  gtk_main ();
  return 0;
}
