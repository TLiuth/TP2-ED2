#include <stdio.h>
#include "quicksort.h"
#include "lista.h"
#include <stdbool.h>
#include <limits.h>
#define DEBUG_MODE true


void debugger(const char* linha){
    if(DEBUG_MODE) printf("> %s\n", linha);
}

void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir){
    debugger("Entrou em QuicksortExterno");
    int i, j;
    TipoArea Area;
    Area.tam = 0; 
    if(Dir - Esq < 1) return;
    ListaEhVazia(&Area);
    Particao(ArqLi, ArqEi, ArqLEs, Area, Esq, Dir, &i, &j);

    if( i - Esq < Dir - i){
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
    }else{
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, j, Dir);
        QuicksortExterno(ArqLi, ArqEi, ArqLEs, Esq, i);
    }
}






void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer){
    fseek(*ArqLEs, (*Ls - 1) * sizeof(TipoRegistro), SEEK_SET);
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Ls)--;
    //printf("Ls: %d\n", *Ls);
    if(DEBUG_MODE)printf("Lendo superior.... %f |%ld", UltLido->nota, UltLido->inscricao);

    *OndeLer = false;
}



void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer){
    fread(UltLido, sizeof(TipoRegistro), 1, *ArqLi);
    (*Li)++;
    // printf("Li: %d\n", *Li);
    printf("Lendo inferior.... %f |%ld", UltLido->nota, UltLido->inscricao);
    *OndeLer = true;
}




void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea){
    debugger("Inseriu no pivo------------------------------------------");
    ListaInsereFinal(Area, *UltLido);
    *NRArea = ListaTamanho(Area);
}




void EscreveMax(FILE **ArqLEs, TipoRegistro R, int* Es){
    fseek(*ArqLEs, (*Es - 1) * sizeof(TipoRegistro), SEEK_SET);
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqLEs);
    (*Es)--;
}

void EscreveMin(FILE **ArqEi, TipoRegistro R, int* Ei){
    fwrite(&R, sizeof(TipoRegistro), 1, *ArqEi);
    (*Ei)++;
}


void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea){
    ListaRetiraFinal(Area, R);
    *NRArea = ListaTamanho(Area);
}

void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea){
    printf("Retirando... %f | %ld\n", R->nota, R->inscricao);
    ListaRetiraInicio(Area, R);
    *NRArea = ListaTamanho(Area);
}

void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j){
    debugger("------------------------------");
    debugger("Entrou em Particao");
    int Ls = Dir, Es = Dir, Li = Esq, Ei = Esq, NRArea = 0, Linf = INT_MIN, Lsup = INT_MAX;
    short OndeLer = true;
    TipoRegistro UltLido, R;
    fseek(*ArqLi, (Li - 1) * sizeof(TipoRegistro), SEEK_SET);
    fseek(*ArqEi, (Ei - 1) * sizeof(TipoRegistro), SEEK_SET);
    *i = Esq - 1;
    *j = Dir + 1;
    //debugger("Antes do while");
    while(Ls >= Li){
        if(NRArea < TAM_AREA - 1){
            //debugger("Entrou no 1 if");
            if(OndeLer)
                LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
            else
                LeInf(ArqLi, &UltLido, &Li, &OndeLer);
            InserirArea(&Area, &UltLido, &NRArea);
            continue;
        }

        if(Ls == Es){
            debugger("Entrou no 2 if");
            //printf("Es: %d", Es);
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        }else if(Li == Ei){
            debugger("Entrou no 3 if");
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }else if(OndeLer){
            debugger("Entrou no 4 if");
            LeSup(ArqLEs, &UltLido, &Ls, &OndeLer);
        }else {
            debugger("Entrou no 5 if");
            LeInf(ArqLi, &UltLido, &Li, &OndeLer);
        }
        
        if(UltLido.nota > Lsup){
            debugger("Entrou no 6 if");
            *j = Es;
            EscreveMax(ArqLEs, UltLido, &Es);
            continue;
        }

        if(UltLido.nota < Linf){
            debugger("Entrou no 7 if");
            *i = Ei;
            EscreveMin(ArqEi, UltLido, &Ei);
            continue;
        }

        InserirArea(&Area, &UltLido, &NRArea);

        if(Ei - Esq < Dir - Es){
            debugger("Arquivo A1");
            RetiraMin(&Area, &R, &NRArea);
            EscreveMin(ArqEi, R, &Ei);
            Linf = R.nota;
        }else{
            debugger("Arquivo A2");
            RetiraMax(&Area, &R, &NRArea);
            EscreveMax(ArqLEs, R, &Es);
            Lsup = R.nota;
        }   
    }
    debugger("Antes do 2 while");
    while(Ei <= Es){
        
        RetiraMin(&Area, &R, &NRArea);
        EscreveMin(ArqEi, R, &Ei);
    }
    debugger("terminou particao");
}