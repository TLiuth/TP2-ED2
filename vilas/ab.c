#include "quicksort.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

void PrintSubfileContents(FILE *file, int start, int end, const char *subfileName) {
    TipoRegistro reg;
    printf("%s: ", subfileName);
    fseek(file, (start - 1) * sizeof(TipoRegistro), SEEK_SET);

    // printf("start: %d\n", start);
    // printf("end: %d\n", end);
    

    for (int pos = start; pos <= start + 15; pos++) {
        if((fread(&reg, sizeof(TipoRegistro), 1, file)) != 1)
            printf("Sem TipoRegistros para ler\n");
        else 
            printf("| %.1f ", reg.nota);
    }
    printf("\n");
}


void inicializaLista(TipoArea* area){
    area->tam = 0;
    area->itens = (TipoRegistro*)malloc(TAM_AREA * sizeof(TipoRegistro));
}

void quicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int esq, int dir){
    int i, j;
    TipoArea area;
    if(dir - esq < 1) return;
    inicializaLista(&area);
    particao(ArqLi, ArqEi, ArqLEs, area, esq, dir, &i, &j);
    if(i - esq < dir - j){
        quicksortExterno(ArqLi, ArqEi, ArqLEs, esq, i);
        quicksortExterno(ArqLi, ArqEi, ArqLEs, j, dir);
    } else {
        quicksortExterno(ArqLi, ArqEi, ArqLEs, j, dir);
        quicksortExterno(ArqLi, ArqEi, ArqLEs, esq, i);
    }
    free(area.itens);
}

void leSup(FILE **ArqLEs, TipoRegistro *ultimoLido, int *lSup, bool *ondeLer){
    fseek(*ArqLEs, (*lSup - 1) * sizeof(TipoRegistro), SEEK_SET);
    fread(ultimoLido, sizeof(TipoRegistro), 1, *ArqLEs);
    printf("Lendo superior... %f", ultimoLido->nota);
    (*lSup)--; *ondeLer = false;
}

void leInf(FILE **ArqLi, TipoRegistro *ultimoLido, int *lInf, bool *ondeLer){
    fread(ultimoLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*lInf)++; *ondeLer = true;
}

void inserirArea(TipoArea *area, TipoRegistro *ultimoLido, int *numArea){
    insereItem(*ultimoLido, area); 
    *numArea = obterNumCelOcupadas(*area);
}

void escreveMax(FILE **ArqLEs, TipoRegistro R, int *eSup){
    fseek(*ArqLEs, (*eSup - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*eSup)--;
}

void escreveMin(FILE **arqEInf, TipoRegistro R, int *eInf){
    fwrite(&R, sizeof(TipoRegistro), 1, *arqEInf);
    (*eInf)++;
}

void retiraUltimo(TipoArea *area, TipoRegistro *R) {
    if (area->tam > 0) {
        *R = area->itens[area->tam - 1];  // Get the last item
        area->tam--;  // Decrease the size of the area
    }
}

void retiraPrimeiro(TipoArea *area, TipoRegistro *R) {
    if (area->tam > 0) {
        *R = area->itens[0];  // Get the first item
        area->tam--;  // Decrease the size of the area
        for(int i = 0; i < area->tam; i++){
            area->itens[i] = area->itens[i + 1];
        }
    }
}

void insereItem(TipoRegistro ultimoLido, TipoArea *area) {
    int i = area->tam - 1;
    // Find the correct position to insert the new item
    while (i >= 0 && area->itens[i].nota > ultimoLido.nota) {
        area->itens[i + 1] = area->itens[i];
        i--;
    }
    // Insert the new item
    area->itens[i + 1] = ultimoLido;
    area->tam++;
}

int obterNumCelOcupadas(TipoArea area){
    return area.tam;
} 

void retiraMax(TipoArea *area, TipoRegistro *R, int *numArea){
    retiraUltimo(area, R);
    *numArea = obterNumCelOcupadas(*area);
}

void retiraMin(TipoArea *area, TipoRegistro *R, int *numArea){
    retiraPrimeiro(area, R);
    *numArea = obterNumCelOcupadas(*area);
}

void particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs,
              TipoArea area, int esq, int dir, int *i, int *j){
    
    int lSup = dir, eSup = dir, lInf = esq, eInf = esq,
        numArea = 0;
    float limInf = INT_MIN, limSup = INT_MAX;
    bool ondeLer = true; 
    TipoRegistro ultimoLido, R;
    fseek(*ArqLi, (lInf - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (eInf - 1) * sizeof(TipoRegistro), SEEK_SET);
    *i = esq - 1;
    *j = dir + 1;

    while(lSup >= lInf){
        PrintSubfileContents(*ArqEi, esq, *i, "A1");

        if(numArea < TAM_AREA - 1){
            if(ondeLer)
                leSup(ArqLEs, &ultimoLido, &lSup, &ondeLer);
            else leInf(ArqLi, &ultimoLido, &lInf, &ondeLer);

                inserirArea(&area, &ultimoLido, &numArea);
                continue;
        }
        if(lSup == eSup)
            leSup(ArqLEs, &ultimoLido, &lSup, &ondeLer);
            else if(lInf == eInf) leInf(ArqLi, &ultimoLido, &lInf, &ondeLer);
                else if (ondeLer) leSup(ArqLEs, &ultimoLido, &lSup, &ondeLer);
                    else leInf(ArqLi, &ultimoLido, &lInf, &ondeLer);

        if(ultimoLido.nota > limSup){
            *j = eSup;
            escreveMax(ArqLEs, ultimoLido, &eSup);
            // continue;
        } else if(ultimoLido.nota < limInf){
            *i = eInf;
            escreveMin(ArqEi, ultimoLido, &eInf);
            // continue;
        } else {
            inserirArea(&area, &ultimoLido, &numArea);

            PrintSubfileContents(*ArqEi, esq, *i, "A1");
            PrintSubfileContents(*ArqLEs, *j, dir, "A2");
            printf("===========================================\n");

            if(eInf - esq < dir - eSup){
                retiraMin(&area, &R, &numArea);
                escreveMin(ArqEi, R, &eInf); 
                limInf = R.nota;
            }
            else {
                retiraMax(&area, &R, &numArea);
                escreveMax(ArqLEs, R, &eSup);
                limSup = R.nota; 
            }
        }
    }

    while(eInf <= eSup){
        retiraMin(&area, &R, &numArea);
        escreveMin(ArqEi, R, &eInf);
    }
}