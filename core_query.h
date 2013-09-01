enum /* Query combo box selections */
{
  QRY_EST_NAME = 0,
  QRY_EST_DEVR,
  QRY_EST_MC,
  QRY_CEL_RCT_L,
  QRY_CEL_RCT_S,
  QRY_CEL_HCT_L,
  QRY_CEL_HCT_S,
  QRY_CEL_ARA_L,
  QRY_CEL_ARA_S,
  QRY_CEL_SRT_L,
  QRY_CEL_SRT_S,
  QRY_CEL_PRC_L,
  QRY_CEL_PRC_S,
  NUM_QRY_TYPES
};

static GtkTreeModel *
QRY_create_and_fill_model () /* Create and fill query tree model */
{
  gint i = 0;
  GtkListStore *store;
  GtkTreeIter iter;
  //struct estate *estateThis = estateHead;
  if (results == NULL)
  {
    return NULL;
  }
  store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

  for (i = 0; i < results->len; i++)
  {
    gtk_list_store_append (store, &iter);

    gtk_list_store_set (store, &iter,
                        QRY_RST_PATH, g_array_index (results, GSResult, i).path,
                        QRY_RST_DESC, g_array_index (results, GSResult, i).desc,
                        -1);
  }
  /* Append a row and fill in some data */
  return GTK_TREE_MODEL (store);
}

static GtkWidget *
QRY_create_view_and_model () /* Create tree view; create tree model by calling QRY_create_and_fill_model () */
{
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;
  view = gtk_tree_view_new ();
  model = QRY_create_and_fill_model();
  /* --- Column #1 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (QRY_RST_PATH));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Result Path",
                                               renderer,
                                               "text", QRY_RST_PATH,
                                               NULL);
  /* --- Column #2 --- */
  renderer = gtk_cell_renderer_text_new ();
  g_object_set_data (G_OBJECT (renderer), "column", GINT_TO_POINTER (QRY_RST_DESC));
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Result Description",
                                               renderer,
                                               "text", QRY_RST_DESC,
                                               NULL);

  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
  /* The tree view has acquired its own reference to the
  * model, so we can drop ours. That way the model will
  * be freed automatically when the tree view is destroyed */
  g_object_unref (model);
  return view;
}

static void
QRY_view_entry (GtkWidget *widget,
                gpointer   data) /* open a window that contains the selected query entry */
{
  GtkTreeIter iter;
  GtkTreeView *treeview = (GtkTreeView *)data;
  GtkTreeModel *model = gtk_tree_view_get_model (treeview);
  GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

  if (gtk_tree_selection_get_selected (selection, NULL, &iter))
    {
      gint i;
      GtkTreePath *path;
      GCell *cell_temp;

      path = gtk_tree_model_get_path (model, &iter);
      i = gtk_tree_path_get_indices (path)[0];
      if (g_array_index (results, GSResult, i).type)
      {
        if (window_cell != NULL) /* Destroy previous window if already opened */
          CEL_destroy_window (NULL, window_cell);

        /* initialize a new cell view window */
        cell_temp = CEL_head = ((GCell *) g_array_index (results, GSResult, i).entity)->parent->child;
        cells = g_array_sized_new (FALSE, FALSE, sizeof (GCell *), 1);
        for (;cell_temp != NULL; cell_temp = cell_temp -> next)
        {
          g_array_append_vals (cells, &cell_temp, 1);
        }
        window_cell = CEL_new_window(i);
        gtk_widget_show_all (window_cell);
      }
      gtk_tree_path_free (path);
    }
}

static void
QRY_destroy_result_window (GtkWidget *widget,
                           gpointer   data)
{
  GtkWidget *window = (GtkWidget *)data;
  gtk_widget_destroy(window);
  g_return_if_fail(results != NULL);
  g_array_free(results, FALSE);
}

static GtkWidget *
QRY_new_result_window () /* create a new result window */
{
  GtkWidget *window_result;
  GtkWidget *view;
  GtkWidget *vbox, *hbox1;
  GtkWidget *button_view, *button_output, *button_help, *button_close;
  GtkWidget *scrolled_window;
  gchar *window_title;

  window_result = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  window_title = g_strdup_printf("EMR: view Search Result");
  gtk_window_set_default_size (GTK_WINDOW (window_result), 600, 400);
  gtk_window_set_title (GTK_WINDOW (window_result), window_title);
  g_free(window_title);

  g_signal_connect (window_result, "delete_event", G_CALLBACK (gtk_widget_destroy), NULL);

  vbox = gtk_vbox_new(FALSE, 8);
  hbox1 = gtk_hbox_new(TRUE, 5);

  button_view = gtk_button_new_with_label ("View");
  button_output = gtk_button_new_with_label ("Output");
  button_help = gtk_button_new_with_label ("Help");
  button_close = gtk_button_new_with_label ("Close");

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrolled_window),
                                       GTK_SHADOW_ETCHED_IN);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_AUTOMATIC);

  g_signal_connect (button_close, "clicked", G_CALLBACK (QRY_destroy_result_window), window_result);

  view = QRY_create_view_and_model (results);
  gtk_widget_set_usize (scrolled_window, 580, 320);

  g_signal_connect (button_view, "clicked", G_CALLBACK (QRY_view_entry), view);

  gtk_container_add (GTK_CONTAINER (window_result), vbox);
  gtk_box_pack_start (GTK_BOX (vbox), scrolled_window, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_container_add (GTK_CONTAINER (vbox), hbox1);
  gtk_container_add (GTK_CONTAINER (hbox1), button_view);
  gtk_container_add (GTK_CONTAINER (hbox1), button_output);
  gtk_container_add (GTK_CONTAINER (hbox1), button_help);
  gtk_container_add (GTK_CONTAINER (hbox1), button_close);

  return window_result;
}

static void
QRY_open_result_window(gint         search_type,
                       const gchar *search_content)
{
  GtkWidget *window_result;
  GEstate *estateThis = EST_head;
  GBldn *bldnThis = NULL;
  GCell *cellThis = NULL;
  GSResult result_temp;
  gint found;
  gint search_content_int, search_result_int;
  gfloat search_content_float, search_result_float;
  gchar *property_name;
  results = g_array_sized_new (FALSE, FALSE, sizeof (GSResult), 1);
  for (;estateThis != NULL; estateThis = estateThis->next)
  {
    if (search_type <= QRY_EST_MC)
    {
      switch (search_type)
      {
        case QRY_EST_NAME:
          found = !g_strcmp0(search_content, estateThis->name);
          property_name = g_strdup ("Name");
          break;
        case QRY_EST_DEVR:
          found = !g_strcmp0(search_content, estateThis->propertyDevr);
          property_name = g_strdup ("Property Developer");
          break;
        case QRY_EST_MC:
          found = !g_strcmp0(search_content, estateThis->propertyMC);
          property_name = g_strdup ("Property Management Company");
          break;
      }
      if (found)
      {
        result_temp.type = 0;
        result_temp.entity = (gpointer) estateThis;
        result_temp.path = g_strdup_printf("\\%s", estateThis->name);
        result_temp.desc = g_strdup_printf("%s: %s", property_name, search_content);
        g_array_append_vals (results, &result_temp, 1);
      }
    }
    else
    {
      for (bldnThis = estateThis->child; bldnThis != NULL; bldnThis = bldnThis->next)
      {
        for (cellThis = bldnThis->child; cellThis != NULL; cellThis = cellThis->next)
        {
          if (search_type <= QRY_CEL_HCT_S)
            search_content_int = atoi(search_content);
          else
            search_content_float = atof(search_content);
          switch (search_type)
          {
            case QRY_CEL_RCT_L:
              found = cellThis->roomCount >= search_content_int;
              search_result_int = cellThis->roomCount;
              property_name = g_strdup ("Room count");
              break;
            case QRY_CEL_RCT_S:
              found = cellThis->roomCount <= search_content_int;
              search_result_int = cellThis->roomCount;
              property_name = g_strdup ("Room count");
              break;
            case QRY_CEL_HCT_L:
              found = cellThis->hallCount >= search_content_int;
              search_result_int = cellThis->hallCount;
              property_name = g_strdup ("Hall count");
              break;
            case QRY_CEL_HCT_S:
              found = cellThis->hallCount <= search_content_int;
              search_result_int = cellThis->hallCount;
              property_name = g_strdup ("Hall count");
              break;
            case QRY_CEL_ARA_L:
              found = cellThis->area >= search_content_float;
              search_result_float = cellThis->area;
              property_name = g_strdup ("Area");
              break;
            case QRY_CEL_ARA_S:
              found = cellThis->area <= search_content_float;
              search_result_float = cellThis->area;
              property_name = g_strdup ("Area");
              break;
            case QRY_CEL_SRT_L:
              found = cellThis->shareRate >= search_content_float;
              search_result_float = cellThis->shareRate;
              property_name = g_strdup ("Share rate");
              break;
            case QRY_CEL_SRT_S:
              found = cellThis->shareRate <= search_content_float;
              search_result_float = cellThis->shareRate;
              property_name = g_strdup ("Share rate");
              break;
            case QRY_CEL_PRC_L:
              found = cellThis->price >= search_content_float;
              search_result_float = cellThis->price;
              property_name = g_strdup ("Price");
              break;
            case QRY_CEL_PRC_S:
              found = cellThis->price >= search_content_float;
              search_result_float = cellThis->price;
              property_name = g_strdup ("Price");
              break;
          }
          if (found)
          {
            result_temp.type = 2;
            result_temp.entity = (gpointer) cellThis;
            result_temp.path = g_strdup_printf("\\%s >> building %d > room %d", estateThis->name, bldnThis->id, cellThis->id);
            if (search_type <= QRY_CEL_HCT_S)
              result_temp.desc = g_strdup_printf("%s: %d %s %d",
                                                 property_name,
                                                 search_result_int,
                                                 search_result_int >= search_content_int?">=":"<=",
                                                 search_content_int);
            else
              result_temp.desc = g_strdup_printf("%s: %f %s %f",
                                                 property_name,
                                                 search_result_float,
                                                 search_result_float >= search_content_float?">=":"<=",
                                                 search_content_float);
            g_array_append_vals (results, &result_temp, 1);
          }
        }
      }
    }
  }
  g_free(property_name);
  window_result = QRY_new_result_window();
  gtk_widget_show_all (window_result);
}

static void
QRY_main(GtkWidget *widget,
         gpointer   data) /* Main function of query feature */
{
  GtkWidget *window_parent = (GtkWidget *)data;
  GtkWidget *dialog, *dialog_error;
  GtkWidget *combobox;
  GtkListStore *store;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  GtkWidget *label_search;
  GtkWidget *entry_search;
  GtkWidget *hbox;
  gint dialog_response;

  label_search = gtk_label_new("Query type:");

  dialog = gtk_dialog_new_with_buttons("Query Ultility",
                                       GTK_WINDOW (window_parent),
                                       GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                       "Search",
                                       GTK_RESPONSE_YES,
                                       "Cancel",
                                       GTK_RESPONSE_CLOSE,
                                       NULL);

  dialog_error = gtk_message_dialog_new (NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_ERROR,
                                         GTK_BUTTONS_OK,
                                         "Invalid search content!");

  /* A tree model for query type selections */
  store = gtk_list_store_new (1, G_TYPE_STRING);
  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Estate Name", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Estate Developer", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Estate Management Company", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Room Count >=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Room Count <=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Hall Count >=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Hall Count <=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Size >=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Size <=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Share Rate >=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Share Rate <=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Price >=", -1);

  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, 0, "Cell Price <=", -1);

  gtk_list_store_append (store, &iter);
  combobox = gtk_combo_box_new_with_model(GTK_TREE_MODEL (store));
  g_object_unref (store);
  renderer = gtk_cell_renderer_text_new ();
  gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combobox), renderer, TRUE);
  gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combobox), renderer,
                                  "text", 0,
                                  NULL);

  entry_search = gtk_entry_new ();

  hbox = gtk_hbox_new (FALSE, 8);
  gtk_container_add (GTK_CONTAINER (hbox), label_search);
  gtk_container_add (GTK_CONTAINER (hbox), combobox);
  gtk_container_add (GTK_CONTAINER (hbox), entry_search);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show_all (dialog);

  /* prompt the user to input query information */
  dialog_response = gtk_dialog_run(GTK_DIALOG (dialog));
  if (dialog_response == GTK_RESPONSE_YES)
  {
    if (gtk_entry_get_text(GTK_ENTRY (entry_search)) != NULL)
    {
      QRY_open_result_window(gtk_combo_box_get_active(GTK_COMBO_BOX (combobox)), gtk_entry_get_text(GTK_ENTRY (entry_search)));
    }
    else
    {
      gtk_dialog_run (GTK_DIALOG (dialog_error));
      gtk_widget_destroy (dialog_error);
    }
  }
  gtk_widget_destroy (dialog);
}
