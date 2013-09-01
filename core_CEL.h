static void
CEL_cell_edited (GtkCellRendererText *cell,
                 const gchar         *path_string,
                 const gchar         *new_text,
                 gpointer             data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
  GtkTreeIter iter;

  gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

  gtk_tree_model_get_iter (model, &iter, path);

  switch (column)
    {
    case CEL_ID:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (cells, GCell *, i)->id = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (cells, GCell *, i)->id, -1);
      }
      break;
    case CEL_RCT:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (cells, GCell *, i)->roomCount = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (cells, GCell *, i)->roomCount, -1);
      }
      break;
    case CEL_HCT:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (cells, GCell *, i)->hallCount = atoi (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (cells, GCell *, i)->hallCount, -1);
      }
      break;
    case CEL_ARA:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (cells, GCell *, i)->area = atof (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (cells, GCell *, i)->area, -1);
      }
      break;
    case CEL_SRT:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (cells, GCell *, i)->shareRate = atof (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (cells, GCell *, i)->shareRate, -1);
      }
      break;
    case CEL_PRC:
      {
        gint i;

        i = gtk_tree_path_get_indices (path)[0];
        g_array_index (cells, GCell *, i)->price = atof (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (cells, GCell *, i)->price, -1);
      }
      break;
    }
  gtk_tree_path_free (path);
}

static void
CEL_add_item (GtkWidget *button,
              gpointer data)
{
  GCell *cell_temp = (GCell *)malloc(sizeof(GCell));
  GtkTreeIter iter;
  GtkTreeModel *model = (GtkTreeModel *)data;

  g_return_if_fail (cells != NULL);

  cell_temp->id = g_array_index (cells, GCell *, cells->len - 1)->id + 1;
  cell_temp->roomCount = 1;
  cell_temp->hallCount = 1;
  cell_temp->area = 100.00;
  cell_temp->shareRate = 0.50;
  cell_temp->price = 10000.0;
  cell_temp->next = NULL;
  cell_temp->parent = g_array_index (cells, GCell *, 0)->parent;
  g_array_index (cells, GCell *, cells->len - 1)->next = cell_temp;
  g_array_append_vals (cells, &cell_temp, 1);

  gtk_list_store_append (GTK_LIST_STORE (model), &iter);
  gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                      CEL_ID, cell_temp->id,
                      CEL_RCT, cell_temp->roomCount,
                      CEL_HCT, cell_temp->hallCount,
                      CEL_ARA, cell_temp->area,
                      CEL_SRT, cell_temp->shareRate,
                      CEL_PRC, cell_temp->price,
                      -1);
}

static void
CEL_remove_item (GtkWidget *widget,
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

      g_array_index (cells, GCell *, i)->prev = g_array_index (cells, GCell *, i)->next;
      free (g_array_index (cells, GCell *, i));
      g_array_remove_index (cells, i);
      //CEL_array_sync ();

      gtk_tree_path_free (path);
    }
}

static GtkTreeModel *
CEL_create_and_fill_model () /* create a tree model based on GArray cells */
{
  gint i = 0;
  GtkListStore *store;
  GtkTreeIter iter;

  store = gtk_list_store_new (NUM_CEL_COLUMNS, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_FLOAT);

  for (i = 0; i < cells->len; i++)
  {
    gtk_list_store_append (store, &iter);

    gtk_list_store_set (store, &iter,
                        CEL_ID, g_array_index (cells, GCell *, i)->id,
                        CEL_RCT, g_array_index (cells, GCell *, i)->roomCount,
                        CEL_HCT, g_array_index (cells, GCell *, i)->hallCount,
                        CEL_ARA, g_array_index (cells, GCell *, i)->area,
                        CEL_SRT, g_array_index (cells, GCell *, i)->shareRate,
                        CEL_PRC, g_array_index (cells, GCell *, i)->price,
                        -1);
  }
  /* Append a row and fill in some data */
  return GTK_TREE_MODEL (store);
}

static GtkWidget *
CEL_create_view_and_model ()
{
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;
  view = gtk_tree_view_new ();
  model = CEL_create_and_fill_model();
  /* --- Column #1 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (CEL_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (CEL_ID));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "ID",
                                               renderer,
                                               "text", CEL_ID,
                                               NULL);
  /* --- Column #2 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (CEL_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (CEL_RCT));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Room Count",
                                               renderer,
                                               "text", CEL_RCT,
                                               NULL);
  /* --- Column #3 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (CEL_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (CEL_HCT));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Hall Count",
                                               renderer,
                                               "text", CEL_HCT,
                                               NULL);
  /* --- Column #4 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (CEL_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (CEL_ARA));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Area",
                                               renderer,
                                               "text", CEL_ARA,
                                               NULL);
  /* --- Column #5 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (CEL_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (CEL_SRT));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Share Rate",
                                               renderer,
                                               "text", CEL_SRT,
                                               NULL);
  /* --- Column #6 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (CEL_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (CEL_PRC));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Price",
                                               renderer,
                                               "text", CEL_PRC,
                                               NULL);

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
  /* The tree view has acquired its own reference to the
  * model, so we can drop ours. That way the model will
  * be freed automatically when the tree view is destroyed */
  g_object_unref (model);
  return view;
}

static void
CEL_destroy_window(GtkWidget *widget,
                   gpointer data)
{
  GtkWidget *window_this = (GtkWidget *)data;
  gtk_widget_destroy(window_this);
  window_cell = NULL;
  g_return_if_fail(cells != NULL);
  CEL_head->parent->child = g_array_index (cells, GCell *, 0);
  CEL_head = NULL;
  g_array_free(cells, FALSE);
}

static GtkWidget *
CEL_new_window (gint focus) /* I hope the tree view can let me set a selection, unfortunately it does not support this method */
{
  GtkWidget *window_cell;
  GtkWidget *view;
  GtkWidget *vbox, *hbox1, *hbox2;
  GtkWidget *button_add, *button_delete, *button_search_dialog, *button_help, *button_close;
  GtkWidget *scrolled_window;
  gchar *window_title;

  window_cell = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  //window_title = g_strdup_printf("EMR: view cells");
  window_title = g_strdup_printf("EMR: view cells >> %s> %d", CEL_head->parent->parent->name, CEL_head->parent->id);
  gtk_window_set_default_size (GTK_WINDOW (window_cell), 600, 400);
  gtk_window_set_title (GTK_WINDOW (window_cell), window_title);
  g_free(window_title);

  g_signal_connect (window_cell, "delete_event", G_CALLBACK (gtk_widget_destroy), NULL);

  vbox = gtk_vbox_new(FALSE, 8);
  hbox1 = gtk_hbox_new(TRUE, 5);
  hbox2 = gtk_hbox_new(TRUE, 5);

  button_add = gtk_button_new_with_label ("Add row");
  button_delete = gtk_button_new_with_label ("Remove row");
  button_search_dialog = gtk_button_new_with_label ("Search");
  button_help = gtk_button_new_with_label ("Help");
  button_close = gtk_button_new_with_label ("Close");

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
                                       GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);

  g_signal_connect (button_close, "clicked", G_CALLBACK (CEL_destroy_window), window_cell);

  view = CEL_create_view_and_model ();
  gtk_widget_set_usize (scrolled_window, 580, 320);

  gtk_container_add (GTK_CONTAINER (window_cell), vbox);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_container_add (GTK_CONTAINER (vbox), hbox1);
  gtk_container_add (GTK_CONTAINER (vbox), hbox2);
  gtk_container_add (GTK_CONTAINER (hbox1), button_add);
  gtk_container_add (GTK_CONTAINER (hbox1), button_delete);
  gtk_container_add (GTK_CONTAINER (hbox2), button_search_dialog);
  gtk_container_add (GTK_CONTAINER (hbox2), button_help);
  gtk_container_add (GTK_CONTAINER (hbox2), button_close);

  g_signal_connect (button_add, "clicked",
                    G_CALLBACK (CEL_add_item), GTK_TREE_MODEL (gtk_tree_view_get_model(GTK_TREE_VIEW (view))));
  g_signal_connect (button_delete, "clicked",
                    G_CALLBACK (CEL_remove_item), view);
  return window_cell;
}

static void
CEL_open_window (GtkWidget *widget,
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
      GCell *cell_temp = NULL;

      path = gtk_tree_model_get_path (model, &iter);
      i = gtk_tree_path_get_indices (path)[0];
      cell_temp = CEL_head = g_array_index (buildings, GBldn *, i)->child;

      cells = g_array_sized_new (FALSE, FALSE, sizeof (GCell *), 1);

      for (;cell_temp != NULL; cell_temp = cell_temp -> next)
      {
        g_array_append_vals (cells, &cell_temp, 1);
      }

      if (window_cell != NULL)
      {
        CEL_destroy_window(NULL, window_cell);
      }
      window_cell = CEL_new_window(0);
      gtk_widget_show_all (window_cell);

      gtk_tree_path_free (path);
    }
}
