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
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j){
    printf("Entrou em Particao *************************************************************\n");
    
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0;
    float Linf = INT_MIN, Lsup = INT_MAX;
    short OndeLer = true;
    TipoRegistro UltLido, R;
    fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(TipoRegistro), SEEK_SET);
    *i = Esq - 1;
    *j = Dir + 1;
    //debugger("Antes do while");
    while(Ls >= Li){
        // PrintSubfileContents(*ArqEi, Esq, *i, "A1");
        // ListaImprime(&Area);
        // PrintSubfileContents(*ArqLEs, *j, Dir, "A2");
        // printf("===========================================\n");

        if(NRArea < (TAM_AREA - 1)){
            //debugger("Entrou no 1 if");
            if(OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            else
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            InserirArea(&Area, &UltLido, &NRArea);

            printf("Área: < ");

            continue;
        }

        // if(NRArea == TAM_AREA - 1)
        //     ListaImprime(&Area);

        if(Ls == Es){
            printf("Entrou no 2 if\n");
            //printf("Es: %d", Es);
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        }else if(Li == Ei){
            printf("Entrou no 3 if\n");
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }else if(OndeLer){
            printf("Entrou no 4 if\n");
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        }else {
            
            printf("Entrou no 5 if\n");
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }
        
        if(UltLido.nota > Lsup){
            printf("Entrou no 6 if\n");
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }

        if(UltLido.nota < Linf){
            printf("Entrou no 7 if\n");
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        InserirArea(&Area, &UltLido, &NRArea);

        // PrintSubfileContents(*ArqEi, Esq, *i, "A1");
        // ListaImprime(&Area);
        // PrintSubfileContents(*ArqLEs, *j, Dir, "A2");
        // printf("===========================================\n");

        if((Ei - Esq) < (Dir - Es)){
            if(DEBUG_PARTICAO) printf("|||||| Arquivo A1\n");
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
            if(DEBUG_PARTICAO) printf("Linf: %f\n", Linf);
        }else{
            if(DEBUG_PARTICAO) printf("|||||| Arquivo A2\n");
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
            if(DEBUG_PARTICAO) printf("Lsup: %f\n", Lsup);
        }  
    }
    debugger("Antes do 2 while");
    while(Ei <= Es){
        
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
        //PrintSubfileContents(*ArqEi, Esq, *i, "A1");
    }

    debugger("terminou particao");
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