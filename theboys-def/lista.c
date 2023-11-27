// TAD lista de números inteiros - implementação
// Autor: Leonardo Amorim Carpwiski (GRR 20232331) 

// Implementação com lista encadeada dupla não-circular

// Coloque os includes necessários a este arquivo aqui.
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Eventuais funções auxiliares (usadas somente neste arquivo) podem vir aqui.

// Implemente as funções definidas em liblista.h aqui.
lista_t* lista_cria () {
   lista_t *ptr_lista;

   ptr_lista = malloc (sizeof (lista_t));
   // Caso não conseguiu alocar, retorne NULO;
   if (!ptr_lista)
      return NULL;

   // Lista vazia inicialmente;
   ptr_lista->head = NULL;
   ptr_lista->tail = NULL;
   ptr_lista->size = 0;

   return ptr_lista;
}

lista_t *lista_destroi (lista_t *lst) {
   int i;
   lnode_t *percorre_node, *destroi_node;

   // percorre_node serve para percorrer cada node, sem tornar node nulo;
   // destroi_node serve para liberar memória e nulificar o ponteiro de node;
   percorre_node = lst->head;
   for (i = 0; i < lst->size; i++) {
      destroi_node = percorre_node;
      free (destroi_node);
      destroi_node = NULL;
      percorre_node = percorre_node->next;
   }

   free (lst);
   lst = NULL;

   return NULL;
}

int lista_insere (lista_t *lst, int elem, int pos) {
   int i;
   lnode_t *node_ins, *ptr_node;

   // Verificação da existência da lista;
   if (!lst)
      return -1;

   // Alocação e verificação de sucesso da alocação;
   node_ins = malloc (sizeof (lnode_t));
   if (!node_ins)
      return -1;

   node_ins->prev = NULL;
   node_ins->next = NULL;
   node_ins->val = elem;

   // Lista vazia;
   if (lst->size == 0) {
      lst->head = node_ins;
      lst->tail = node_ins;
   }
   else {
      // Insere node em head;
      if (pos == 0) {
         node_ins->next = lst->head;   // next do node a ser inserido -> head atual;
         lst->head->prev = node_ins;   // prev do head atual -> node a ser inserido;
         lst->head = node_ins;         // as três sentenças, juntas, posicionam node antes de head;
      }
      // Insere node em tail;
      else if (pos == -1 || pos >= (lst->size) - 1) {
         lst->tail->next = node_ins;   // next do tail atual-> node a ser inserido;
         node_ins->prev = lst->tail;   // prev do node a ser inserido -> tail atual;
         lst->tail = node_ins;         // as três sentenças, juntas, posicionam node depois de tail;
      }
      // Insere node no nodo de posição pos;
      else {
         ptr_node = lst->head;
         for (i = 0; i < pos; i++)
            ptr_node = ptr_node->next;

         ptr_node->prev->next = node_ins;   // 1. next do anterior de pos -> node;
         node_ins->next = ptr_node;         // 2. next de node -> pos;
         node_ins->prev = ptr_node->prev;   // 3. prev de node -> anterior de pos;
         ptr_node->prev = node_ins;         // 4. prev de pos -> node;
      }
   }
   (lst->size)++;

   return (lst->size);
}

int lista_retira (lista_t *lst, int *elem, int pos) {
   int i;
   lnode_t *ptr_node;

   // Verificação de existência da lista;
   if (!lst)
      return -1;
   // Verificação de lista vazia;
   if (!lst->size)
      return -1;

   // Lista com um único elemento;
   if (lst->size == 1) {
      *elem = lst->head->val;
      free (lst->head);
      // Volta-se à uma lista vazia;
      lst->head = NULL;
      lst->tail = NULL;
   }
   else {
      // Remoção do head;
      if (pos == 0) {
         ptr_node = lst->head->next;

         *elem = lst->head->val;
         lst->head->next->prev = NULL;   // prev do posterior do head não aponta para ninguém, pois vira o novo head;
         lst->head->next = NULL;         // next do atual head não aponta para ninguém, pois será removido;

         free (lst->head);
         lst->head = ptr_node;
      }
      // Remoção do tail;
      else if (pos == -1 || pos == lst->size - 1) {
         ptr_node = lst->tail->prev;

         *elem = lst->tail->val;
         lst->tail->prev->next = NULL;   // next do anterior do tail não aponta para ninguém, pois vira o novo tail;
         lst->tail->prev = NULL;         // prev do atual tail não aponta para ninguém, pois será removido;

         free (lst->tail);
         lst->tail = ptr_node;
      }
      // Remoção do elemento na posição pos;
      else {
         ptr_node = lst->head;
         for (i = 0; i < pos; i++)
            ptr_node = ptr_node->next;

         *elem = ptr_node->val;
         ptr_node->prev->next = ptr_node->next;   // 1. next do anterior de pos -> posterior de pos;
         ptr_node->next->prev = ptr_node->prev;   // 2. prev do posterior de pos -> anterior de pos;
         ptr_node->next = NULL;                   // 3. next do pos deve apontar para nada;
         ptr_node->prev = NULL;                   // 4. prev do pos deve apontar para nada;
         free (ptr_node);
         ptr_node = NULL;
      }
   }
   (lst->size)--;

   return (lst->size);
}

int lista_consulta (lista_t* lst, int *elem, int pos) {
   int i;
   lnode_t *ptr_node;

   // Verificação de existência da lista;
   if (!lst)
      return -1;
   // Verificação de lista vazia;
   if (!lst->size)
      return -1;

   if (pos == -1)
      *elem = lst->tail->val;
   else {
      i = 0;
      ptr_node = lst->head;
      while (i < pos && ptr_node) {
         ptr_node = ptr_node->next;
         i++;
      }

      if (!ptr_node)
         return -1;
      else
         *elem = ptr_node->val;
   }

   return (lst->size);
}

int lista_procura (lista_t* lst, int elem) {
   int i;
   lnode_t *ptr_node;

   // Verificação de existência da lista;
   if (!lst)
      return -1;
   // Verificação de lista vazia;
   if (!lst->size)
      return -1;

   i = 0;
   ptr_node = lst->head;
   while (i < lst->size) {
      if (ptr_node->val == elem)
         return i;
      ptr_node = ptr_node->next;
      i++;
   }

   return -1;
}

int lista_vazia (lista_t* lst) {
   if (lst->size == 0)
      return 1;
   else
      return 0;
}

int lista_tamanho (lista_t* lst) {
   // Verificação da existência da lista;
   if (!lst)
      return -1;

   return (lst->size);
}

void lista_imprime (char *nome, lista_t *lst) {
   char *ptr_char;
   lnode_t *ptr_node;

   // Imprime a string nome, caractere por caractere;
   for (ptr_char = nome; *ptr_char != '\0'; ptr_char++)
      putchar (*ptr_char);

   printf (" [ ");

   ptr_node = lst->head;
   while (ptr_node) {
      printf ("%d ", ptr_node->val);
      ptr_node = ptr_node->next;
   }

   printf ("]\n");
}