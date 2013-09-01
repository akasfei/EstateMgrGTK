static void
EST_array_link () /* Link struct GEstate to a GArray for viewing and editing */
{
  GEstate *estate_temp = EST_head;
  estates = g_array_sized_new (FALSE, FALSE, sizeof (GEstate *), 1);
  g_return_if_fail (estates != NULL);

  for (;estate_temp != NULL; estate_temp = estate_temp -> next)
  {
    g_array_append_vals (estates, &estate_temp, 1);
  }
}

static void
EST_cell_edited (GtkCellRendererText *cell,
                 const gchar         *path_string,
                 const gchar         *new_text,
                 gpointer             data) /* response function for an edited cell */
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
  GtkTreeIter iter;

  gint column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));

  gtk_tree_model_get_iter (model, &iter, path);

  switch (column) /* different actions depending on the column which was edited */
    {
    case EST_ID:
      break;
    case EST_NAME:
      {
        gint i;
        gchar *old_text;

        gtk_tree_model_get (model, &iter, column, &old_text, -1);
        g_free (old_text);

        i = gtk_tree_path_get_indices (path)[0];
        g_free (g_array_index (estates, GEstate *, i)->name);
        g_array_index (estates, GEstate *, i)->name = g_strdup (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (estates, GEstate *, i)->name, -1);
      }
      break;
    case EST_ADDR:
      {
        gint i;
        gchar *old_text;

        gtk_tree_model_get (model, &iter, column, &old_text, -1);
        g_free (old_text);

        i = gtk_tree_path_get_indices (path)[0];
        g_free (g_array_index (estates, GEstate *, i)->addr);
        g_array_index (estates, GEstate *, i)->addr = g_strdup (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (estates, GEstate *, i)->addr, -1);
      }
      break;
    case EST_DEVR:
      {
        gint i;
        gchar *old_text;

        gtk_tree_model_get (model, &iter, column, &old_text, -1);
        g_free (old_text);

        i = gtk_tree_path_get_indices (path)[0];
        g_free (g_array_index (estates, GEstate *, i)->propertyDevr);
        g_array_index (estates, GEstate *, i)->propertyDevr = g_strdup (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (estates, GEstate *, i)->propertyDevr, -1);
      }
      break;
    case EST_MC:
      {
        gint i;
        gchar *old_text;

        gtk_tree_model_get (model, &iter, column, &old_text, -1);
        g_free (old_text);

        i = gtk_tree_path_get_indices (path)[0];
        g_free (g_array_index (estates, GEstate *, i)->propertyMC);
        g_array_index (estates, GEstate *, i)->propertyMC = g_strdup (new_text);

        gtk_list_store_set (GTK_LIST_STORE (model), &iter, column,
                            g_array_index (estates, GEstate *, i)->propertyMC, -1);
      }
      break;
    }
  gtk_tree_path_free (path);
}

static void
EST_add_item (GtkWidget *button,
              gpointer data) /* Append a new estate */
{
  GEstate *estate_temp = (GEstate *)malloc(sizeof(GEstate));
  GtkTreeIter iter;
  GtkTreeModel *model = (GtkTreeModel *)data;

  g_return_if_fail (estates != NULL);

  estate_temp->id = g_array_index (estates, GEstate *, estates->len - 1)->id + 1;
  estate_temp->name = g_strdup ("Enter name here");
  estate_temp->addr = g_strdup ("Enter address here");
  estate_temp->propertyDevr = g_strdup ("Enter Developer here");
  estate_temp->propertyMC = g_strdup ("Enter Management Company here");
  estate_temp->prev = g_array_index (estates, GEstate *, estates->len - 1);
  estate_temp->next = NULL;
  estate_temp->child = NULL;
  g_array_index (estates, GEstate *, estates->len - 1)->next = estate_temp;
  g_array_append_vals (estates, &estate_temp, 1);

  gtk_list_store_append (GTK_LIST_STORE (model), &iter);
  gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                      EST_ID, estate_temp->id,
                      EST_NAME, estate_temp->name,
                      EST_ADDR, estate_temp->addr,
                      EST_DEVR, estate_temp->propertyDevr,
                      EST_MC, estate_temp->propertyMC,
                      -1);
}

static void
EST_remove_item (GtkWidget *widget,
                 gpointer data) /* remove selected estate */
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

      g_array_index (estates, GEstate *, i)->prev = g_array_index (estates, GEstate *, i)->next;
      free (g_array_index (estates, GEstate *, i));
      g_array_remove_index (estates, i);

      gtk_tree_path_free (path);
    }
}


static GtkTreeModel *
EST_create_and_fill_model () /* create a tree model based on GArray estates */
{
  gint i = 0;
  GtkListStore *store;
  GtkTreeIter iter;

  store = gtk_list_store_new (NUM_EST_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  for (i = 0; i < estates->len; i++)
  {
    gtk_list_store_append (store, &iter);

    gtk_list_store_set (store, &iter,
                        EST_ID, g_array_index (estates, GEstate *, i)->id,
                        EST_NAME, g_array_index (estates, GEstate *, i)->name,
                        EST_ADDR, g_array_index (estates, GEstate *, i)->addr,
                        EST_DEVR, g_array_index (estates, GEstate *, i)->propertyDevr,
                        EST_MC, g_array_index (estates, GEstate *, i)->propertyMC,
                        -1);
  }
  /* Append a row and fill in some data */
  return GTK_TREE_MODEL (store);
}

static GtkWidget *
EST_create_view_and_model () /* Create tree view; create tree model by calling 'EST_create_and_fill_model' */
{
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;
  view = gtk_tree_view_new ();
  model = EST_create_and_fill_model();
  /* --- Column #1 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", FALSE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (EST_cell_edited), model);
  g_signal_connect (renderer, "editing-started",
                    G_CALLBACK (editing_started), NULL);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (EST_ID));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "ID",
                                               renderer,
                                               "text", EST_ID,
                                               NULL);
  /* --- Column #2 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (EST_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (EST_NAME));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Name",
                                               renderer,
                                               "text", EST_NAME,
                                               NULL);
  /* --- Column #3 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (EST_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (EST_ADDR));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Address",
                                               renderer,
                                               "text", EST_ADDR,
                                               NULL);
  /* --- Column #4 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (EST_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (EST_DEVR));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Developer",
                                               renderer,
                                               "text", EST_DEVR,
                                               NULL);
  /* --- Column #5 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set (renderer,
                "editable", TRUE,
                NULL);
  g_signal_connect (renderer, "edited",
                    G_CALLBACK (EST_cell_edited), model);
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (EST_MC));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Management Company",
                                               renderer,
                                               "text", EST_MC,
                                               NULL);

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
  /* The tree view has acquired its own reference to the
  * model, so we can drop ours. That way the model will
  * be freed automatically when the tree view is destroyed */

  g_object_unref (model);
  return view;
}
