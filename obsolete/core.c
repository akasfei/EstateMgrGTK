#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct estate
{
    int id;
    char *idString;
    char *name;
    char *addr;
    char *propertyDevr; //Developer
    char *propertyMC;   //Management Company
    struct estate *next;
    struct bldn *child; //its children buildings
};

struct bldn
{
    int id;
    char idString[4];
    int unitCount;
    int storeyCount;
    struct estate *parent;
    struct bldn *next;
    struct cell *child;
};

struct cell
{
    int id;
    char idString[4];
    int cellStruct;
    float area;
    float shareRate;
    float price;
    struct cell *next;
    struct bldn *parent;
};

struct searchResult
{
    int type;
    void *result;
    struct searchResult *next;
};

struct estate *sheetInit()
{
    FILE *estateDB = fopen("database_estate.db","r");
    FILE *bldnDB = NULL;
    FILE *cellDB = NULL;
    struct estate *estateHead, *estateThis = NULL, *estatePrev = NULL;
    struct bldn *bldnHead, *bldnThis, *bldnPrev = NULL;
    struct cell *cellHead, *cellThis, *cellPrev = NULL;
    char fileBuffer[255], propertyTemp[5][120], bldnFileName[32], cellFileName[32];
    int i,j,k,l;
    estateHead = (struct estate *)malloc(sizeof(struct estate));
    estateThis = estateHead;
    fgets(fileBuffer,255,estateDB); // ignore line 1
    fgets(fileBuffer,255,estateDB);
    do {
        if (estateThis == NULL)
        {
            estateThis = (struct estate *)malloc(sizeof(struct estate));
            estatePrev->next = estateThis;
        }
        for (i=0,j=k=-1;fileBuffer[i]!='\0';i++,k++)
        {
            if (fileBuffer[i]=='<')
            {
                k=0;
                j++;
            } else if (fileBuffer[i]!='>')
            {
                propertyTemp[j][k] = fileBuffer[i];
            } else {
            for (l=k;l<120;l++)
                propertyTemp[j][l] = '\0';
            }
        }

        estateThis->id = atoi(propertyTemp[0]);
        estateThis->idString = (char *)malloc(sizeof(char)*strlen(propertyTemp[0]));
        estateThis->idString = propertyTemp[0];
        /*estateThis->name = propertyTemp[1];
        estateThis->addr = propertyTemp[2];
        estateThis->propertyDevr = propertyTemp[3];
        estateThis->propertyMC = propertyTemp[4];*/
        estateThis->name = (char *)malloc(sizeof(char)*strlen(propertyTemp[1]));
        estateThis->name = propertyTemp[1];
        estateThis->addr = (char *)malloc(sizeof(char)*strlen(propertyTemp[2]));
        estateThis->addr = propertyTemp[2];
        estateThis->propertyDevr = (char *)malloc(sizeof(char)*strlen(propertyTemp[3]));
        estateThis->propertyDevr = propertyTemp[3];
        estateThis->propertyMC = (char *)malloc(sizeof(char)*strlen(propertyTemp[4]));
        estateThis->propertyMC = propertyTemp[4];
        strcpy(bldnFileName , "database_bldn_");
        strcat(bldnFileName,estateThis->idString);
        strcat(bldnFileName, ".db");
        bldnDB = fopen(bldnFileName,"r"); //open building database
        if (bldnDB != NULL)
        {
            bldnHead = (struct bldn *)malloc(sizeof(struct bldn));
            bldnThis = bldnHead;
            fgets(fileBuffer,255,bldnDB); // ignore line 1
            fgets(fileBuffer,255,bldnDB);
            do {
                for (i=0,j=k=-1;fileBuffer[i]!='\0';i++,k++)
                {
                    if (fileBuffer[i]=='<')
                    {
                        k=0;
                        j++;
                    } else if (fileBuffer[i]!='>')
                    {
                        propertyTemp[j][k] = fileBuffer[i];
                    } else {
                        for (l=k;l<120;l++)
                            propertyTemp[j][l] = '\0';
                    }
                }
                if (bldnThis == NULL)
                {
                    bldnThis = (struct bldn*)malloc(sizeof(struct bldn));
                    bldnPrev->next = bldnThis;
                }

                bldnThis->id = atoi(propertyTemp[0]);
                //bldnThis->idString = propertyTemp[0];
                strcpy(bldnThis->idString , propertyTemp[0]);
                bldnThis->unitCount = atoi(propertyTemp[1]);
                bldnThis->storeyCount = atoi(propertyTemp[2]);
                bldnThis->parent = estateThis;
                //cellFileName = strcat ( strcat( strcat("database_cell_", estateThis->idString) , "_"), bldnThis->idString );
                strcpy(cellFileName , "database_cell_");
                strcat(cellFileName, estateThis->idString);
                strcat(cellFileName, "_");
                strcat(cellFileName, bldnThis->idString);
                strcat(cellFileName, ".db");
                cellDB = fopen(cellFileName,"r"); //open cell database
                if (cellDB != NULL)
                {
                    cellHead = (struct cell*)malloc(sizeof(struct cell));
                    cellThis = cellHead;
                    fgets(fileBuffer,200,cellDB);
                    fgets(fileBuffer,200,cellDB);
                    do {
                        for (i=0,j=k=-1;fileBuffer[i]!='\0';i++,k++)
                        {
                            if (fileBuffer[i]=='<')
                            {
                                k=0;
                                j++;
                            } else if (fileBuffer[i]!='>')
                            {
                                propertyTemp[j][k] = fileBuffer[i];
                            } else {
                                for (l=k;l<120;l++)
                                    propertyTemp[j][l] = '\0';
                            }
                        }
                        /*for (i=j=k=0;fileBuffer[i]!='\0';i++,k++)
                        {
                            if (fileBuffer[i]!='|')
                            {
                                propertyTemp[j][k] = fileBuffer[i];
                            } else {
                                for (l=k;l<120;l++)
                                    propertyTemp[j][l] = '\0';
                                k=0;
                                j++;
                            }
                        }*/
                        if (cellThis == NULL)
                        {
                            cellThis = (struct cell*)malloc(sizeof(struct cell));
                            cellPrev->next = cellThis;
                        }
                        cellThis->id = atoi(propertyTemp[0]);
                        //cellThis->idString = propertyTemp[0];
                        strcpy(cellThis->idString , propertyTemp[0]);
                        cellThis->cellStruct = atoi(propertyTemp[1]);
                        cellThis->area = atof(propertyTemp[2]);
                        cellThis->shareRate = atof(propertyTemp[3]);
                        cellThis->price = atof(propertyTemp[4]);
                        cellThis->parent = bldnThis;
                        cellThis->next = NULL;
                        cellThis = NULL;
                    } while (!feof(cellDB));
                    fclose(cellDB);
                }
                bldnThis->child = cellHead;
                bldnThis->next = NULL;
                bldnPrev = bldnThis;
                bldnThis = NULL;
                fgets(fileBuffer,200,bldnDB);
            } while (!feof(bldnDB));
            fclose(bldnDB);
        }
        estateThis->child = bldnHead;
        estateThis->next = NULL;
        estatePrev = estateThis;
        estateThis = NULL;
        fgets(fileBuffer,200,estateDB);
    }while (!feof(estateDB));
    return estateHead;
}

struct estate *queryEstateByName(char *estateName, struct estate *estateHead) //Search estate by name
{
    struct estate *estateThis = estateHead;
    while (estateThis!=NULL)
    {
        if (!strcmp(estateThis->name,estateName))
            return estateThis;
        estateThis = estateThis->next;
    }
    return NULL;
}

struct searchResult *queryEstateByDevr(char *estateDevr, struct estate *estateHead) //Search estate by Developer
{
    struct estate *estateThis = estateHead;
    struct searchResult *resultHead = NULL, *resultThis = NULL;
    while (estateThis!=NULL)
    {
        if (!strcmp(estateThis->propertyDevr,estateDevr))
            if (resultThis == NULL)
            {
                resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                resultThis->type = 0;
                resultThis->result = (void *)estateThis;
                if (resultHead == NULL)
                {
                    resultHead = resultThis;
                    resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultHead->next = resultThis;
                } else
                {
                    resultThis->next = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultThis = resultThis->next;
                }
            }
        estateThis = estateThis->next;
    }
    return NULL;
}

struct searchResult *queryEstateByMC(char *estateMC, struct estate *estateHead) //Search estate by Management Company
{
    struct searchResult *resultHead = NULL, *resultThis = NULL;
    struct estate *estateThis = estateHead;
    while (estateThis!=NULL)
    {
        if (!strcmp(estateThis->propertyMC,estateMC))
            if (resultThis == NULL)
            {
                resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                resultThis->type = 0;
                resultThis->result = (void *)estateThis;
                if (resultHead == NULL)
                {
                    resultHead = resultThis;
                    resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultHead->next = resultThis;
                } else
                {
                    resultThis->next = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultThis = resultThis->next;
                }
            }
        estateThis = estateThis->next;
    }
    return resultHead;
}

struct searchResult *queryCellByStruct(int cellStruct, struct estate *estateHead) //Search cell by structure
{
    struct searchResult *resultHead = NULL, *resultThis = NULL;
    struct estate *estateThis = estateHead;
    struct bldn *bldnThis;
    struct cell *cellThis;
    while (estateThis != NULL)
    {
        bldnThis = estateThis->child;
        while (bldnThis != NULL)
        {
            cellThis = bldnThis->child;
            while (cellThis != NULL)
            {
                if (cellThis->cellStruct == cellStruct)
                {
                    if (resultThis == NULL)
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultThis->type = 2;
                    resultThis->result = (void *)cellThis;
                    if (resultHead == NULL)
                    {
                        resultHead = resultThis;
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultHead->next = resultThis;
                    } else
                    {
                        resultThis->next = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultThis = resultThis->next;
                    }
                }
            }
            bldnThis = bldnThis->next;
        }
        estateThis = estateThis->next;
    }
    return resultHead;
}

struct searchResult *queryCellByArea(float area, int minArea, struct estate *estateHead) //Search cell by area; minArea == 0 -> area will be used as minimum requirements
{
    struct searchResult *resultHead = NULL, *resultThis = NULL;
    struct estate *estateThis = estateHead;
    struct bldn *bldnThis;
    struct cell *cellThis;
    int validation;
    while (estateThis != NULL)
    {
        bldnThis = estateThis->child;
        while (bldnThis != NULL)
        {
            cellThis = bldnThis->child;
            while (cellThis != NULL)
            {
                if (minArea)
                {
                    validation = cellThis->area >= area;
                } else {
                    validation = cellThis->area <= area;
                }
                if (validation)
                {
                    if (resultThis == NULL)
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultThis->type = 2;
                    resultThis->result = (void *)cellThis;
                    if (resultHead == NULL)
                    {
                        resultHead = resultThis;
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultHead->next = resultThis;
                    } else
                    {
                        resultThis->next = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultThis = resultThis->next;
                    }
                }
            }
            bldnThis = bldnThis->next;
        }
        estateThis = estateThis->next;
    }
    return resultHead;
}

struct searchResult *queryCellByShareRate(float shareRate, int minRate, struct estate *estateHead)
{
    struct searchResult *resultHead = NULL, *resultThis = NULL;
    struct estate *estateThis = estateHead;
    struct bldn *bldnThis;
    struct cell *cellThis;
    int validation;
    while (estateThis != NULL)
    {
        bldnThis = estateThis->child;
        while (bldnThis != NULL)
        {
            cellThis = bldnThis->child;
            while (cellThis != NULL)
            {
                if (minRate)
                {
                    validation = cellThis->shareRate >= shareRate;
                } else {
                    validation = cellThis->shareRate <= shareRate;
                }
                if (validation)
                {
                    if (resultThis == NULL)
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultThis->type = 2;
                    resultThis->result = (void *)cellThis;
                    if (resultHead == NULL)
                    {
                        resultHead = resultThis;
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultHead->next = resultThis;
                    } else
                    {
                        resultThis->next = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultThis = resultThis->next;
                    }
                }
            }
            bldnThis = bldnThis->next;
        }
        estateThis = estateThis->next;
    }
    return resultHead;
}

struct searchResult *queryCellByPrice(float price, int minPrice, struct estate *estateHead)
{
    struct searchResult *resultHead = NULL, *resultThis = NULL;
    struct estate *estateThis = estateHead;
    struct bldn *bldnThis;
    struct cell *cellThis;
    int validation;
    while (estateThis != NULL)
    {
        bldnThis = estateThis->child;
        while (bldnThis != NULL)
        {
            cellThis = bldnThis->child;
            while (cellThis != NULL)
            {
                if (minPrice)
                {
                    validation = cellThis->price >= price;
                } else {
                    validation = cellThis->price <= price;
                }
                if (validation)
                {
                    if (resultThis == NULL)
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                    resultThis->type = 2;
                    resultThis->result = (void *)cellThis;
                    if (resultHead == NULL)
                    {
                        resultHead = resultThis;
                        resultThis = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultHead->next = resultThis;
                    } else
                    {
                        resultThis->next = (struct searchResult *)malloc(sizeof(struct searchResult));
                        resultThis = resultThis->next;
                    }
                }
            }
            bldnThis = bldnThis->next;
        }
        estateThis = estateThis->next;
    }
    return resultHead;
}

void saveData(struct estate *estateHead)
{
    FILE *estateDB = fopen("database_estate.db","w");
    FILE *bldnDB = NULL;
    FILE *cellDB = NULL;
    struct estate *estateThis = estateHead;
    struct bldn *bldnHead, *bldnThis;
    struct cell *cellHead, *cellThis;
    char bldnFileName[32], cellFileName[32];
    fprintf(estateDB,"<id><name><addr><developer><management company>\n"); // print line 1
    do {
        fprintf(estateDB,"<%d><%s><%s><%s><%s>\n", estateThis->id, estateThis->name, estateThis->addr, estateThis->propertyDevr, estateThis->propertyMC);
        strcpy(bldnFileName , "database_bldn_");
        strcat(bldnFileName,estateThis->idString);
        bldnDB = fopen(bldnFileName,"w"); //write building database
        bldnHead = estateThis->child;
        if (bldnDB != NULL)
        {
            bldnThis = bldnHead;
            fprintf(bldnDB,"<id><unitCount><storeyCount>\n"); // print line 1
            do {
                fprintf(bldnDB,"<%d><%d><%d>\n", bldnThis->id, bldnThis->unitCount, bldnThis->storeyCount);
                cellHead = bldnThis->child;
                strcpy(cellFileName , "database_cell_");
                strcat(cellFileName, estateThis->idString);
                strcat(cellFileName, "_");
                strcat(cellFileName, bldnThis->idString);
                cellDB = fopen(cellFileName,"w"); //open cell database
                if (cellDB != NULL)
                {
                    cellThis = cellHead;
                    fprintf(cellDB,"<id><cellStruct><area><shareRate><price>");
                    do {
                        fprintf(cellDB,"<%d><%d><%0.1f><%0.2f><%0.0f>",cellThis->id, cellThis->cellStruct, cellThis->area, cellThis->shareRate, cellThis->price);
                        cellThis = cellThis->next;
                    } while (cellThis!=NULL);
                    fclose(cellDB);
                }
                bldnThis = bldnThis->next;
            } while (bldnThis!=NULL);
            fclose(bldnDB);
        }
        estateThis = estateThis->next;
    }while (estateThis!=NULL);
}

/* int main(int argc, char *argv[])
{
    struct estate *estateThis = sheetInit(), estate = *estateThis;
    struct bldn *bldnThis, bldn;
    struct cell *cellThis, cell;
    while (estateThis != NULL)
    {
        printf("Displaying info for estate %s:\nAddress: %s\nProperty Developer: %s\nProperty Management Company: %s\n", estateThis->name, estateThis->addr, estateThis->propertyDevr, estateThis->propertyMC);
        bldnThis = estateThis->child;
        bldn = *bldnThis;
        while(bldnThis != NULL)
        {
            printf("\n\tBuilding %d: \n\tUnit Count: %d\n\tStorey Count: %d\n", bldnThis->id, bldnThis->unitCount, bldnThis->storeyCount);
            cellThis = bldnThis->child;
            cell = *cellThis;
            while(cellThis != NULL)
            {
                printf("\n\t\tCell %d: \n\t\tCell Structure: %d room(s), %d living room(s).\n\t\tArea: %0.1f square meters\n\t\tShare rate: %0.3f\n\t\tPrice: %0.1f RMB / square meter\n",cellThis->id,cellThis->cellStruct/10,cellThis->cellStruct%10,cellThis->area,cellThis->shareRate,cellThis->price);
                cellThis = cellThis->next;
            }
            bldnThis = bldnThis->next;
        }
        estateThis = estateThis->next;
    }
    printf("%d, %d, %d\n",sizeof(struct estate), sizeof(struct bldn), sizeof(struct cell));
    printf("%d = %d (int) + %d (char[244]) + %d (*)", sizeof(struct estate), sizeof(int), 244*sizeof(char), 2*sizeof(struct estate *));
    getchar();
    return 0;
}
*/
