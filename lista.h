#ifndef LISTA_H
#define LISTA_H

#include "quicksort.h"
#include <stdbool.h>
/* procedimentos e funcoes do TAD */
Lista* ListaCria();
void ListaDestroi(Lista** pLista);
bool ListaEhVazia(Lista* pLista);
bool ListaInsereFinal(Lista* pLista, TipoRegistro x);
bool ListaInsereInicio(Lista* pLista, TipoRegistro x);
bool ListaRetiraFinal(Lista* pLista, TipoRegistro *pX);
bool ListaRetiraInicio(Lista* pLista, TipoRegistro *pX);
void ListaImprime(Lista* pLista);
bool ListaGet(Lista* pLista, int p, TipoRegistro *pX);
int ListaTamanho(Lista* pLista);
void insertion(Lista *v);

#endif // !LISTA_H