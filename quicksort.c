#include <stdio.h>
#include "quicksort.h"
#include "lista.h"
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#define DEBUG_MODE true

void PrintSubfileContents(FILE *file, int start, int end, const char *subfileName);

void debugger(const char* linha){
    if(DEBUG_MODE) printf("> %s\n", linha);
}

void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir){
    if(DEBUG_MODE) printf("Entrou em QuicksortExterno\n");
    int i = Esq, j = Dir;
    TipoArea Area;
    ListaCria(&Area);
    if(Dir - Esq < 1) 
        return;

    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j);

    if( i - Esq < Dir - i){
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    }else{
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i); 
    }

    free(Area.itens);
}




void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer){
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    if((fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs)) != 1){
        printf("Erro ao ler máximo\n");
    }
    (*Ls)--;
    if(DEBUG_MODE) printf(">>> Lendo superior.... %f | %ld\n", UltLido->nota, UltLido->inscricao);

    *OndeLer = false;
    printf("Ls: %d\n", *Ls);

}



void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer){
    if((fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi)) != 1){
        printf("Erro ao ler mínimo\n");
    }
    (*Li)++;
    if(DEBUG_MODE) printf(">>> Lendo inferior.... %f | %ld\n", UltLido->nota, UltLido->inscricao);
    *OndeLer = true;
    printf("Li: %d\n", *Li);
}




void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea){
    insere(Area, *UltLido, Area->tam - 1);
    *NRArea = ListaTamanho(Area);

}




void EscreveMax(FILE **ArqLEs, TipoRegistro R, int* Es){
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    printf("<<< Escrevendo Superior... %f | %ld\n", R.nota, R.inscricao);
    if((fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs))!= 1)
        printf("Erro ao escrever máximo\n");
    (*Es)--;
    printf("Es: %d\n", *Es);

}

void EscreveMin(FILE **ArqEi, TipoRegistro R, int* Ei){
    printf("<<< Escrevendo Inferior... %f | %ld\n", R.nota, R.inscricao);
    if((fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi))!= 1){
        printf("Erro ao escrever mínimo\n");
    }
    (*Ei)++;
    printf("Ei: %d\n", *Ei);
}


void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea){
    ListaRetiraFinal(Area, R);
     if(DEBUG_MODE) printf("---- Retirando maximo... %f | %ld\n", R->nota, R->inscricao);
    *NRArea = ListaTamanho(Area);
}

void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea){
    ListaRetiraInicio(Area, R);
     if(DEBUG_MODE) printf("---- Retirando minimo... %f | %ld\n", R->nota, R->inscricao);
    *NRArea = ListaTamanho(Area);
}

#define DEBUG_PARTICAO true
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs,
              TipoArea area, int esq, int dir, int *i, int *j){
    
    int lSup = dir, eSup = dir, lInf = esq, eInf = esq,
        numArea = 0;
    float limInf = INT_MIN, limSup = INT_MAX;
    short ondeLer = true; 
    TipoRegistro ultimoLido, R;
    fseek(*ArqLi, (lInf - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (eInf - 1) * sizeof(TipoRegistro), SEEK_SET);
    *i = esq - 1;
    *j = dir + 1;

    while(lSup >= lInf){
        PrintSubfileContents(*ArqEi, esq, *i, "A1");

        if(numArea < TAM_AREA - 1){
            if(ondeLer)
                LeSup(ArqLEs, &ultimoLido, &lSup, &ondeLer);
            else LeInf(ArqLi, &ultimoLido, &lInf, &ondeLer);

                InserirArea(&area, &ultimoLido, &numArea);
                continue;
        }
        if(lSup == eSup)
            LeSup(ArqLEs, &ultimoLido, &lSup, &ondeLer);
            else if(lInf == eInf) LeInf(ArqLi, &ultimoLido, &lInf, &ondeLer);
                else if (ondeLer) LeSup(ArqLEs, &ultimoLido, &lSup, &ondeLer);
                    else LeInf(ArqLi, &ultimoLido, &lInf, &ondeLer);

        if(ultimoLido.nota > limSup){
            *j = eSup;
            EscreveMax(ArqLEs, ultimoLido, &eSup);
            // continue;
        } else if(ultimoLido.nota < limInf){
            *i = eInf;
            EscreveMin(ArqEi, ultimoLido, &eInf);
            // continue;
        } else {
            InserirArea(&area, &ultimoLido, &numArea);

            PrintSubfileContents(*ArqEi, esq, *i, "A1");
            PrintSubfileContents(*ArqLEs, *j, dir, "A2");
            printf("===========================================\n");

            if(eInf - esq < dir - eSup){
                RetiraMin(&area, &R, &numArea);
                EscreveMin(ArqEi, R, &eInf); 
                limInf = R.nota;
            }
            else {
                RetiraMax(&area, &R, &numArea);
                EscreveMax(ArqLEs, R, &eSup);
                limSup = R.nota; 
            }
        }
    }

    while(eInf <= eSup){
        RetiraMin(&area, &R, &numArea);
        EscreveMin(ArqEi, R, &eInf);
    }
}

void PrintSubfileContents(FILE *file, int start, int end, const char *subfileName) {
    TipoRegistro reg;
    printf("%s: ", subfileName);
    fseek(file, (start - 1) * sizeof(TipoRegistro), SEEK_SET);

    // printf("start: %d\n", start);
    // printf("end: %d\n", end);
    

    for (int pos = start; pos <= end; pos++) {
        if((fread(&reg, sizeof(TipoRegistro), 1, file)) != 1)
            printf("Sem registros para ler\n");
        else 
            printf("| %.1f ", reg.nota);
    }
    printf("\n");
}