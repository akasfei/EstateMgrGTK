#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GEstate *
IO_initialization() /* Read file and initialize all link sheets */
{
  FILE *estateDB = fopen("db\\database_estate.db","r");
  FILE *bldnDB = NULL;
  FILE *cellDB = NULL;
  GEstate *estateHead, *estateThis = NULL, *estatePrev = NULL;
  GBldn *bldnHead, *bldnThis, *bldnPrev = NULL;
  GCell *cellHead, *cellThis, *cellPrev = NULL;
  char fileBuffer[255], propertyTemp[100], bldnFileName[40], cellFileName[40];
  gchar *estate_properties[5], *bldn_properties[3], *cell_properties[6];
  int i, j, k;
  estateHead = (GEstate *)malloc(sizeof(GEstate));
  estateThis = estateHead;
  fgets(fileBuffer, 255, estateDB); // ignore line 1
  do {
    fgets(fileBuffer, 255, estateDB); // estate level
    if (estateThis == NULL)
    {
      estateThis = (GEstate *)malloc(sizeof(GEstate));
      estatePrev->next = estateThis;
    }

    for (i=0, k=0; fileBuffer[i] != '\n' && k < NUM_EST_COLUMNS; i++)
    {
      if (fileBuffer[i]=='<')
      {
        for (j=0,i++;fileBuffer[i]!='>';i++,j++)
        {
          propertyTemp[j] = fileBuffer[i];
        }
        propertyTemp[j] = '\0';
        estate_properties[k] = g_strdup (propertyTemp);
        k++;
      }
    }

    estateThis->id = atoi(estate_properties[0]);
    estateThis->name = g_strdup (estate_properties[1]);
    estateThis->addr = g_strdup (estate_properties[2]);
    estateThis->propertyDevr = g_strdup (estate_properties[3]);
    estateThis->propertyMC = g_strdup (estate_properties[4]);
    strcpy(bldnFileName, "db\\database_bldn_");
    strcat(bldnFileName, estate_properties[0]);
    strcat(bldnFileName, ".db");
    bldnDB = fopen(bldnFileName,"r"); //open building database
    if (bldnDB != NULL)
    {
      bldnHead = (GBldn *)malloc(sizeof(GBldn));
      bldnThis = bldnHead;
      bldnPrev = NULL;
      fgets(fileBuffer, 255, bldnDB); // ignore line 1
      do {
        fgets(fileBuffer, 255, bldnDB); // building level
        if (bldnThis == NULL)
        {
          bldnThis = (GBldn *)malloc(sizeof(GBldn));
          bldnPrev->next = bldnThis;
        }

        for (i=0, k=0; fileBuffer[i] != '\n' && k < NUM_BDN_COLUMNS; i++)
        {
          if (fileBuffer[i] == '<')
          {
            for (j=0, i++; fileBuffer[i] != '>'; i++, j++)
            {
              propertyTemp[j] = fileBuffer[i];
            }
            propertyTemp[j] = '\0';
            bldn_properties[k] = g_strdup (propertyTemp);
            k++;
          }
        }

        bldnThis->id = atoi(bldn_properties[0]);
        bldnThis->unitCount = atoi(bldn_properties[1]);
        bldnThis->storeyCount = atoi(bldn_properties[2]);
        bldnThis->parent = estateThis;
        strcpy(cellFileName , "db\\database_cell_");
        strcat(cellFileName, estate_properties[0]);
        strcat(cellFileName, "_");
        strcat(cellFileName, bldn_properties[0]);
        strcat(cellFileName, ".db");
        cellDB = fopen(cellFileName, "r"); //open cell database
        if (cellDB != NULL)
        {
          cellHead = (GCell *)malloc(sizeof(GCell));
          cellThis = cellHead;
          cellPrev = NULL;
          fgets(fileBuffer, 200, cellDB);
          do {
            fgets(fileBuffer, 255, cellDB); // cell level
            if (cellThis == NULL)
            {
              cellThis = (GCell*)malloc(sizeof(GCell));
              cellPrev->next = cellThis;
            }

            for (i=0, k=0; fileBuffer[i] != '\n' && k < NUM_CEL_COLUMNS; i++)
            {
              if (fileBuffer[i] == '<')
              {
                for (j=0, i++; fileBuffer[i] != '>'; i++, j++)
                {
                  propertyTemp[j] = fileBuffer[i];
                }
                propertyTemp[j] = '\0';
                cell_properties[k] = g_strdup (propertyTemp);
                k++;
              }
            }

            cellThis->id = atoi(cell_properties[0]);
            cellThis->roomCount = atoi(cell_properties[1]);
            cellThis->hallCount = atoi(cell_properties[2]);
            cellThis->area = atof(cell_properties[3]);
            cellThis->shareRate = atof(cell_properties[4]);
            cellThis->price = atof(cell_properties[5]);
            cellThis->parent = bldnThis;
            cellThis->next = NULL;
            cellThis->prev = cellPrev;
            cellPrev = cellThis;
            cellThis = NULL;
          } while (!feof(cellDB));
          fclose(cellDB);
        }
        bldnThis->child = cellHead;
        bldnThis->next = NULL;
        bldnThis->prev = bldnPrev;
        bldnPrev = bldnThis;
        bldnThis = NULL;
      } while (!feof(bldnDB));
      fclose(bldnDB);
    }
    estateThis->child = bldnHead;
    estateThis->next = NULL;
    estateThis->prev = estatePrev;
    estatePrev = estateThis;
    estateThis = NULL;
  } while (!feof(estateDB));
  fclose(estateDB);
  EST_head = estateHead;
  return estateHead;
}

static void
IO_save()
{
    FILE *estateDB = fopen("db\\database_estate.db","w");
    FILE *bldnDB = NULL;
    FILE *cellDB = NULL;
    GEstate *estateThis = EST_head;
    GBldn *bldnHead, *bldnThis;
    GCell *cellHead, *cellThis;
    char bldnFileName[40], cellFileName[40], idString[8];
    fprintf(estateDB,"<meta>EstateDB</meta>"); // print line 1
    do {
        fprintf(estateDB,
                "\n<%d><%s><%s><%s><%s>",
                estateThis->id,
                estateThis->name,
                estateThis->addr,
                estateThis->propertyDevr,
                estateThis->propertyMC);
        strcpy(bldnFileName , "db\\database_bldn_");
        sprintf(idString, "%d", estateThis->id);
        strcat(bldnFileName,idString);
        strcat(bldnFileName, ".db");
        bldnDB = fopen(bldnFileName,"w"); //write building database
        bldnHead = estateThis->child;
        if (bldnDB != NULL)
        {
            bldnThis = bldnHead;
            fprintf(bldnDB,"<meta>BldnDB</meta>"); // print line 1
            while (bldnThis != NULL)
            {
                fprintf(bldnDB,
                        "\n<%d><%d><%d>",
                        bldnThis->id,
                        bldnThis->unitCount,
                        bldnThis->storeyCount);
                cellHead = bldnThis->child;
                strcpy(cellFileName , "db\\database_cell_");
                sprintf(idString, "%d", estateThis->id);
                strcat(cellFileName, idString);
                strcat(cellFileName, "_");
                sprintf(idString, "%d", bldnThis->id);
                strcat(cellFileName, idString);
                strcat(cellFileName, ".db");
                cellDB = fopen(cellFileName,"w"); //write cell database
                if (cellDB != NULL && cellHead != NULL)
                {
                    cellThis = cellHead;
                    fprintf(cellDB,"<meta>CellDB</meta>");
                    while (cellThis != NULL)
                    {
                        fprintf(cellDB,
                                "\n<%d><%d><%d><%0.1f><%0.2f><%0.0f>",
                                cellThis->id, cellThis->roomCount,
                                cellThis->hallCount,
                                cellThis->area,
                                cellThis->shareRate,
                                cellThis->price);
                        cellThis = cellThis->next;
                    }
                    fclose(cellDB);
                }
                bldnThis = bldnThis->next;
            }
            fclose(bldnDB);
        }
        estateThis = estateThis->next;
    }while (estateThis!=NULL);
    fclose(estateDB);
}
