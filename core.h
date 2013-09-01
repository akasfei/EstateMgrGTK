enum /* columns of estate sheet */
{
  EST_ID = 0,
  EST_NAME,
  EST_ADDR,
  EST_DEVR,
  EST_MC,
  NUM_EST_COLUMNS
};

enum /* columns of building sheet */
{
  BDN_ID = 0,
  BDN_UCNT,
  BDN_SCNT,
  NUM_BDN_COLUMNS
};

enum /* columns of cell sheet */
{
  CEL_ID = 0,
  CEL_RCT,
  CEL_HCT,
  CEL_ARA,
  CEL_SRT,
  CEL_PRC,
  NUM_CEL_COLUMNS
};

enum /* columns of query result sheet */
{
  QRY_RST_PATH = 0,
  QRY_RST_DESC,
  NUM_QRY_RST_COLUMNS
};

static GArray *estates = NULL;
static GArray *buildings = NULL;
static GArray *cells = NULL;
static GArray *results = NULL;

typedef struct GEstate
{
    gint id;
    gchar *name;
    gchar *addr;
    gchar *propertyDevr; //Developer
    gchar *propertyMC;   //Management Company
    struct GEstate *next;
    struct GEstate *prev;
    struct GBldn *child; //its children buildings
}GEstate;

typedef struct GBldn
{
    gint id;
    gint unitCount;
    gint storeyCount;
    struct GEstate *parent;
    struct GBldn *next;
    struct GBldn *prev;
    struct GCell *child;
}GBldn;

typedef struct GCell
{
    gint id;
    gint roomCount;
    gint hallCount;
    gfloat area;
    gfloat shareRate;
    gfloat price;
    struct GCell *next;
    struct GCell *prev;
    struct GBldn *parent;
}GCell;

typedef struct GSResult
{
    gint type;
    gchar *path;
    gchar *desc;
    gpointer entity;
    //struct GSResult *next;
}GSResult;

static GEstate *EST_head = NULL;
static GBldn *BDN_head = NULL;
static GCell *CEL_head = NULL;

static GtkWidget *window = NULL;
static GtkWidget *window_bldn = NULL;
static GtkWidget *window_cell = NULL;

static gboolean
separator_row (GtkTreeModel *model,
               GtkTreeIter  *iter,
               gpointer      data)
{
  GtkTreePath *path;
  gint idx;

  path = gtk_tree_model_get_path (model, iter);
  idx = gtk_tree_path_get_indices (path)[0];

  gtk_tree_path_free (path);

  return idx == 5;
}

static void
editing_started (GtkCellRenderer *cell,
                 GtkCellEditable *editable,
                 const gchar     *path,
                 gpointer         data)
{
  gtk_combo_box_set_row_separator_func (GTK_COMBO_BOX (editable),
                                        separator_row, NULL, NULL);
}
