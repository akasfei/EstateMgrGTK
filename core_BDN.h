static void
BDN_cell_edited (GtkCellRendererText *cell,
                 const gchar         *path_string,
                 const gchar         *new_text,
                 gpointer             data) /* response function for an edited cell */
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
  GtkTreeIter iter;

  gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

  gtk_tree_model_get_iter (model, &iter, path);

  switch (column)
    {
    case BDN_ID:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (buildings, GBldn *, i)->id = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (buildings, GBldn *, i)->id, -1);
      }
      break;
    case BDN_UCNT:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (buildings, GBldn *, i)->unitCount = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (buildings, GBldn *, i)->unitCount, -1);
      }
      break;
    case BDN_SCNT:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (buildings, GBldn *, i)->storeyCount = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (buildings, GBldn *, i)->storeyCount, -1);
      }
      break;
    }
  //BDN_array_sync ();
  gtk_tree_path_free (path);
}

static void
BDN_add_item (GtkWidget *button,
              gpointer data) /* Append a new building */
{
  GBldn *bldn_temp = (GBldn *)malloc (sizeof(GBldn));
  GtkTreeIter iter;
  GtkTreeModel *model = (GtkTreeModel *)data;

  g_return_if_fail (buildings != NULL);

  bldn_temp->id = g_array_index (buildings, GBldn *, buildings->len - 1)->id + 1;
  bldn_temp->unitCount = 0;
  bldn_temp->storeyCount = 0;
  bldn_temp->prev = g_array_index (buildings, GBldn *, buildings->len - 1);
  bldn_temp->next = NULL;
  bldn_temp->parent = g_array_index (buildings, GBldn *, 0)->parent;
  bldn_temp->child = NULL;
  g_array_index (buildings, GBldn *, buildings->len - 1)->next = bldn_temp;
  g_array_append_vals (buildings, &bldn_temp, 1);

  gtk_list_store_append (GTK_LIST_STORE (model), &iter);
  gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                      BDN_ID, bldn_temp->id,
                      BDN_UCNT, bldn_temp->unitCount,
                      BDN_SCNT, bldn_temp->storeyCount,
                      -1);
}

static void
BDN_remove_item (GtkWidget *widget,
                 gpointer data)
{
  GtkTreeIter iter;
  GtkTreeView *treeview = (GtkTreeView *)data;
  GtkTreeModel *model = gtk_tree_view_get_model (treeview);
  GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

  if (gtk_tree_selection_get_selected (selection, NULL, &iter))
    {
      gint i;
      GtkTreePath *path;

      path = gtk_tree_model_get_path (model, &iter);
      i = gtk_tree_path_get_indices (path)[0];
      gtk_list_store_remove (GTK_LIST_STORE (model), &iter);

      g_array_index (buildings, GBldn *, i)->prev = g_array_index (buildings, GBldn *, i)->next;
      free (g_array_index (buildings, GBldn *, i));
      g_array_remove_index (buildings, i);
      //BDN_array_sync ();

      gtk_tree_path_free (path);
    }
}

static GtkTreeModel *
BDN_create_and_fill_model () /* create a tree model based on GArray buildings */
{
  gint i = 0;
  GtkListStore *store;
  GtkTreeIter iter;

  store = gtk_list_store_new (NUM_BDN_COLUMNS, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

  for (i = 0; i < buildings->len; i++)
  {
    gtk_list_store_append (store, &iter);

    gtk_list_store_set (store, &iter,
                        BDN_ID, g_array_index (buildings, GBldn *, i)->id,
                        BDN_UCNT, g_array_index (buildings, GBldn *, i)->unitCount,
                        BDN_SCNT, g_array_index (buildings, GBldn *, i)->storeyCount,
                        -1);
  }
  /* Append a row and fill in some data */
  return GTK_TREE_MODEL (store);
}

static GtkWidget *
BDN_create_view_and_model (/*GtkWidget *button_add, GtkWidget *button_remove*/)
{
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;
  view = gtk_tree_view_new ();
  model = BDN_create_and_fill_model();
  /* --- Column #1 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (BDN_cell_edited), model);
  g_signal_connect (renderer, "editing-started",
                    G_CALLBACK (editing_started), NULL);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (BDN_ID));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "ID",
                                               renderer,
                                               "text", BDN_ID,
                                               NULL);
  /* --- Column #2 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (BDN_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (BDN_UCNT));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Unit Count",
                                               renderer,
                                               "text", BDN_UCNT,
                                               NULL);
  /* --- Column #3 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (BDN_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (BDN_SCNT));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Stoery Count",
                                               renderer,
                                               "text", BDN_SCNT,
                                               NULL);

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
  /* The tree view has acquired its own reference to the
  * model, so we can drop ours. That way the model will
  * be freed automatically when the tree view is destroyed */

  g_object_unref (model);
  return view;
}

static void
BDN_destroy_window(GtkWidget *widget,
                   gpointer data)
{
  GtkWidget *window_this = (GtkWidget *)data;
  gtk_widget_destroy(window_this);
  window_bldn = NULL;
  g_return_if_fail (buildings != NULL);
  BDN_head->parent->child = g_array_index (buildings, GBldn *, 0);
  BDN_head = NULL;
  g_array_free(buildings, FALSE);
}

static GtkWidget *
BDN_new_window ()
{
  GtkWidget *window_bldn;
  GtkWidget *view;
  GtkWidget *vbox, *hbox1, *hbox2;
  GtkWidget *button_add, *button_delete, *button_open, *button_search_dialog, *button_help, *button_close;
  GtkWidget *scrolled_window;
  gchar *window_title;

  window_bldn = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  //window_title = g_strdup_printf("EMR: view buildings",);
  window_title = g_strdup_printf("EMR: view buildings >> %s", BDN_head->parent->name);
  gtk_window_set_default_size (GTK_WINDOW (window_bldn), 600, 400);
  gtk_window_set_title (GTK_WINDOW (window_bldn), window_title);
  g_free(window_title);

  g_signal_connect (window_bldn, "delete_event", G_CALLBACK (gtk_widget_destroy), NULL);

  vbox = gtk_vbox_new(FALSE, 8);
  hbox1 = gtk_hbox_new(TRUE, 5);
  hbox2 = gtk_hbox_new(TRUE, 5);

  /* Initialize buttons */
  button_add = gtk_button_new_with_label ("Add row");
  button_delete = gtk_button_new_with_label ("Remove row");
  button_open = gtk_button_new_with_label ("Open");
  button_search_dialog = gtk_button_new_with_label ("Search");
  button_help = gtk_button_new_with_label ("Help");
  button_close = gtk_button_new_with_label ("Close");

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
                                       GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);

  g_signal_connect (button_close, "clicked", G_CALLBACK (BDN_destroy_window), window_bldn);

  view = BDN_create_view_and_model ();
  gtk_widget_set_usize (scrolled_window, 580, 320);

  gtk_container_add (GTK_CONTAINER (window_bldn), vbox);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_container_add (GTK_CONTAINER (vbox), hbox1);
  gtk_container_add (GTK_CONTAINER (vbox), hbox2);
  gtk_container_add (GTK_CONTAINER (hbox1), button_add);
  gtk_container_add (GTK_CONTAINER (hbox1), button_delete);
  gtk_container_add (GTK_CONTAINER (hbox2), button_open);
  gtk_container_add (GTK_CONTAINER (hbox2), button_search_dialog);
  gtk_container_add (GTK_CONTAINER (hbox2), button_help);
  gtk_container_add (GTK_CONTAINER (hbox2), button_close);

  g_signal_connect (button_add, "clicked",
                    G_CALLBACK (BDN_add_item), GTK_TREE_MODEL (gtk_tree_view_get_model(GTK_TREE_VIEW (view))));
  g_signal_connect (button_delete, "clicked",
                    G_CALLBACK (BDN_remove_item), view);
  g_signal_connect (button_open, "clicked",
                    G_CALLBACK (CEL_open_window), view);
  return window_bldn;
}

static void
BDN_open_window (GtkWidget *widget,
                 gpointer data) /* Get estate list selection and open building window */
{
  GtkTreeIter iter;
  GtkTreeView *treeview = (GtkTreeView *)data;
  GtkTreeModel *model = gtk_tree_view_get_model (treeview);
  GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

  if (gtk_tree_selection_get_selected (selection, NULL, &iter))
    {
      gint i;
      GtkTreePath *path;
      GBldn *bldn_temp = NULL;

      path = gtk_tree_model_get_path (model, &iter);
      i = gtk_tree_path_get_indices (path)[0];
      bldn_temp = BDN_head = g_array_index (estates, GEstate *, i)->child;

      buildings = g_array_sized_new (FALSE, FALSE, sizeof (GBldn *), 1);

      for (;bldn_temp != NULL; bldn_temp = bldn_temp -> next)
      {
        g_array_append_vals (buildings, &bldn_temp, 1);
      }
      if (window_bldn != NULL)
      {
        BDN_destroy_window(NULL, window_bldn);
      }
      window_bldn = BDN_new_window();
      gtk_widget_show_all (window_bldn);

      gtk_tree_path_free (path);
    }
}
