int main (int argc, char *argv[])
{
    GtkWidget *window;
    /*
  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL, *vbox_editArea;
  GtkWidget *hbox0 = NULL, *hbox1 = NULL, *hbox_buttons;
  GtkWidget *entry_estateName = NULL, *entry_estateAddr = NULL, *entry_estateDevr = NULL, *entry_estateMC = NULL;
  GtkWidget *label_estateId, *label_estateName, *label_estateAddr, *label_estateDevr, *label_estateMC;
  GtkEntryBuffer *entry_estateName_buffer, *entry_estateAddr_buffer, *entry_estateDevr_buffer, *entry_estateMC_buffer;
  GtkWidget *frame_editArea;
  GtkWidget *scrolledWindow; /*
  // Initialize GTK+
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  // initialize GTK builder

  GtkBuilder *uiBuilder = gtk_builder_new();
  GError *builderError = NULL;
  gtk_builder_add_from_file(uiBuilder, "EstateMGR_windowform.glade", &builderError);

  /* Create the main window
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "E.M.S. v0.1");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);



  Create a vertical box
  vbox = gtk_vbox_new (FALSE, 4);
  gtk_container_add (GTK_CONTAINER (win), vbox);
  */
  /* Creating entry textfields

  frame_editArea = gtk_frame_new ("Editing Area");
  gtk_container_add (GTK_CONTAINER (vbox), frame_editArea);

  vbox_editArea = gtk_vbox_new (TRUE, 2);
  gtk_container_add (GTK_CONTAINER (frame_editArea), vbox_editArea);

  hbox0 = gtk_hbox_new (TRUE, 12);
  gtk_container_add (GTK_CONTAINER (vbox_editArea), hbox0);

  label_estateName = gtk_label_new ("Name:");
  gtk_box_pack_start (GTK_BOX (hbox0), label_estateName, FALSE, FALSE, 0);
  label_estateAddr = gtk_label_new ("Address: ");
  gtk_box_pack_start (GTK_BOX (hbox0), label_estateAddr, FALSE, FALSE, 0);
  label_estateDevr = gtk_label_new ("Developer:");
  gtk_box_pack_start (GTK_BOX (hbox0), label_estateDevr, FALSE, FALSE, 0);
  label_estateMC  = gtk_label_new ("Management company:");
  gtk_box_pack_start (GTK_BOX (hbox0), label_estateMC, FALSE, FALSE, 0);

  hbox1 = gtk_hbox_new (TRUE, 12);
  gtk_container_add (GTK_CONTAINER (vbox_editArea), hbox1);

  entry_estateName_buffer = gtk_entry_buffer_new (NULL, 0);
  entry_estateName = gtk_entry_new_with_buffer (entry_estateName_buffer);
  gtk_box_pack_start (GTK_BOX (hbox1), entry_estateName, FALSE, FALSE, 0);
  entry_estateAddr_buffer = gtk_entry_buffer_new (NULL, 0);
  entry_estateAddr = gtk_entry_new_with_buffer (entry_estateAddr_buffer);
  gtk_box_pack_start (GTK_BOX (hbox1), entry_estateAddr, FALSE, FALSE, 0);
  entry_estateDevr_buffer = gtk_entry_buffer_new (NULL, 0);
  entry_estateDevr = gtk_entry_new_with_buffer (entry_estateDevr_buffer);
  gtk_box_pack_start (GTK_BOX (hbox1), entry_estateDevr, FALSE, FALSE, 0);
  entry_estateMC_buffer = gtk_entry_buffer_new (NULL, 0);
  entry_estateMC = gtk_entry_new_with_buffer (entry_estateMC_buffer);
  gtk_box_pack_start (GTK_BOX (hbox1), entry_estateMC, FALSE, FALSE, 0);

  hbox_buttons = gtk_hbox_new (TRUE, 12);
  gtk_container_add (GTK_CONTAINER (vbox), hbox_buttons);

  button = gtk_button_new_from_stock (GTK_STOCK_DIALOG_INFO);
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (helloWorld), (gpointer) win);
  gtk_box_pack_start (GTK_BOX (hbox_buttons), button, TRUE, TRUE, 0);

  button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
  gtk_box_pack_start (GTK_BOX (hbox_buttons), button, TRUE, TRUE, 0);*/



  /* Enter the main loop */
  //gtk_widget_show_all (win);
  //gtk_main ();
    gtk_init (&argc, &argv);

	window = create_window ();
	gtk_widget_show (window);
  return 0;
}