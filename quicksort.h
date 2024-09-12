#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdio.h>

#define TAM_AREA 20


typedef struct {
    long int inscricao;
    float nota;
    char restante[150];
} TipoRegistro;


typedef struct {
    TipoRegistro itens[TAM_AREA];
    int tam;
} TipoArea;

typedef TipoArea Lista;


void QuicksortExterno(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, int Esq, int Dir);
void LeSup(FILE **ArqLEs, TipoRegistro *UltLido, int *Ls, short *OndeLer);
void LeInf(FILE **ArqLi, TipoRegistro *UltLido, int *Li, short *OndeLer);
void InserirArea(TipoArea *Area, TipoRegistro *UltLido, int *NRArea);
void EscreveMax(FILE **ArqLEs, TipoRegistro R, int* Es);
void EscreveMin(FILE **ArqEi, TipoRegistro R, int* Ei);
void RetiraMax(TipoArea *Area, TipoRegistro *R, int *NRArea);
void RetiraMin(TipoArea *Area, TipoRegistro *R, int *NRArea);
void Particao(FILE **ArqLi, FILE **ArqEi, FILE **ArqLEs, TipoArea Area, int Esq, int Dir, int *i, int *j);
void debugger(const char* linha);

#endif