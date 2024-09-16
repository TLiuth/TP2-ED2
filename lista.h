#ifndef LISTA_H
#define LISTA_H

#include "quicksort.h"
#include <stdbool.h>
/* procedimentos e funcoes do TAD */
void ListaCria(Lista *lista);
void ListaDestroi(Lista* pLista);
bool ListaEhVazia(Lista* pLista);
bool ListaInsereFinal(Lista* pLista, TipoRegistro x);
bool ListaInsereInicio(Lista* pLista, TipoRegistro x);
bool ListaRetiraFinal(Lista* pLista, TipoRegistro *pX);
bool ListaRetiraInicio(Lista* pLista, TipoRegistro *pX);
bool ListaGet(Lista* pLista, int p, TipoRegistro *pX);
int ListaTamanho(Lista* pLista);
void insere(Lista *v, TipoRegistro ultimoLido, int i);
void ListaImprime(Lista* pLista);

#endif // !LISTA_H