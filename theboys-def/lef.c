// Includes necessários
#include <stdlib.h>
#include <stdio.h>
#include "lef.h"

// Implementação de funções
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2) {
   struct evento_t *ev;

   ev = malloc (sizeof (struct evento_t));
   if (!ev)
      return NULL;

   ev->tempo = tempo;
   ev->tipo = tipo;
   ev->dado1 = dado1;
   ev->dado2 = dado2;

   return ev;
}

struct evento_t *destroi_evento (struct evento_t *e) {
   free (e);
   e = NULL;
   return NULL;
}

struct lef_t *cria_lef () {
   struct lef_t *l;

   l = malloc (sizeof (struct lef_t));
   if (!l)
      return NULL;

   l->primeiro = NULL;

   return l;
}

struct lef_t *destroi_lef (struct lef_t *l) {
   struct nodo_lef_t *percorre_lef, *free_nodo;

   if (!l)
      return NULL;

   // Percorre a lista de eventos futuros, liberando
   // os eventos, os nodos e a lista.
   percorre_lef = l->primeiro;
   while (percorre_lef) {
      free_nodo = percorre_lef;
      // Libera evento do nodo atual
      free (free_nodo->evento);
      free_nodo->evento = NULL;

      percorre_lef = percorre_lef->prox;
      
      // Libera nodo atual
      free (free_nodo);
      free_nodo = NULL;
   }
   free (l);
   l = NULL;

   return NULL;
}

int insere_lef (struct lef_t *l, struct evento_t *e) {
   struct nodo_lef_t *nodo_ins, *nodo_ant, *percorre_lef;

   if (!l || !e)
      return 0;

   nodo_ins = malloc (sizeof (struct nodo_lef_t));
   if (!nodo_ins)
      return 0;
   nodo_ins->evento = e;
   nodo_ins->prox = NULL;

   // Lista de eventos está vazia
   if (!l->primeiro)
      l->primeiro = nodo_ins;
   else {
      percorre_lef = l->primeiro;
      while (percorre_lef && percorre_lef->evento->tempo <= e->tempo) {
         nodo_ant = percorre_lef;
           percorre_lef = percorre_lef->prox;
      }
      if (percorre_lef == l->primeiro) {
         nodo_ins->prox = l->primeiro;
         l->primeiro = nodo_ins;
      }
      else {
         nodo_ant->prox = nodo_ins;
         nodo_ins->prox = percorre_lef;
      }
   }

   return 1;
}

struct evento_t *retira_lef (struct lef_t *l) {
   struct nodo_lef_t *remove_lef;
   struct evento_t *evento;

   if (!l)
      return NULL;
   // Lista está vazia
   if (!l->primeiro)
      return NULL;

   remove_lef = l->primeiro;
   evento = l->primeiro->evento;
   l->primeiro = l->primeiro->prox;
   free (remove_lef);

   return (evento);
}

int vazia_lef (struct lef_t *l) {
   if (!l)
      return -1;

   if (!l->primeiro)
      return 1;
   else
      return 0;
}

void imprime_lef (struct lef_t *l) {
   struct nodo_lef_t *percorre_lef;
   struct evento_t *ev;

   if (!l)
      return ;

   percorre_lef = l->primeiro;
   while (percorre_lef) {
      ev = percorre_lef->evento;
      printf ("tempo %d tipo %d d1 %d d2 %d\n", 
              ev->tempo, ev->tipo, ev->dado1, ev->dado2);
      percorre_lef = percorre_lef->prox;
   }
}