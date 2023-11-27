// programa principal do projeto "The Boys - 2023"
// Autor: Leonardo Amorim Carpwiski, GRR: 20232331

// seus #includes vão aqui
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "lef.h"
#include "lista.h"
#include "set.h"

// seus #defines vão aqui
#define EV_CHEGA 0
#define EV_ESPERA 1
#define EV_DESISTE 2
#define EV_AVISA 3
#define EV_ENTRA 4
#define EV_SAI 5
#define EV_VIAJA 6
#define EV_MISSAO 7
#define EV_FIM -1

struct heroi_t {
   int id_heroi;
   struct set_t *habilidades;
   int paciencia;
   int vel;
   int xp;
   int base_atual;
} ;

struct base_t {
   int id_base;
   int capacidade;
   struct set_t *herois_presentes;
   lista_t *herois_pendentes;
   int coord_x, coord_y;
} ;

struct missao_t {
   int id_missao;
   struct set_t *req_habilidades;
   int coord_x, coord_y;
   int n_tentativas;
   int missao_cumprida;
} ;

struct distancia_base_missao {
   int id_base;
   int distancia;
} ;

struct mundo_t {
   int n_herois;
   struct heroi_t *v_herois;
   int n_bases;
   struct base_t *v_bases;
   int n_missoes;
   struct missao_t *v_missoes;
   int n_habilidades;
   int n_tamanho_mundo;
   int clock;
} ;
// Gera um número inteiro aleatório entre min e max.
int rand_num (int min, int max) {
    return (rand () % ((max + 1) - min) + min);
}
// Inicializa cada característica dos heróis do mundo.
int inicializa_herois (struct mundo_t *mundo) {
   int i;
   int hab, n_hab_heroi;
   
   mundo->v_herois = malloc (mundo->n_herois * sizeof (struct heroi_t));
   if (!(mundo->v_herois))
      return (0);
   // id: número inteiro de 0 a n_herois - 1
   // experiência: começa inicialmente em 0
   // paciencia: número aleatório entre 0 e 100
   // velocidade: número aleatório entre 50 e 5000
   // habilidades: conjunto de 1 a 3 elementos (elementos = [0,9])
   for (i = 0; i < mundo->n_herois; i++) {
      mundo->v_herois[i].id_heroi = i;
      mundo->v_herois[i].xp = 0;
      mundo->v_herois[i].paciencia = rand_num (0, 100);
      mundo->v_herois[i].vel = rand_num (50, 5000);
      mundo->v_herois[i].habilidades = set_create (mundo->n_habilidades);
      n_hab_heroi = rand_num (1, 3);
      do {
         hab = rand_num (0, 9);
         while (set_in (mundo->v_herois[i].habilidades, hab))
            hab = rand_num (0, 9);
         set_add (mundo->v_herois[i].habilidades, hab);
         n_hab_heroi -= 1;
      } while (n_hab_heroi != 0) ;
   }
   return (1);
}
// Inicializa cada característica das bases de heróis do mundo.
int inicializa_bases (struct mundo_t *mundo) {
   int i;
   
   mundo->v_bases = malloc (mundo->n_bases * sizeof (struct base_t));
   if (!(mundo->v_bases))
      return (0);
   // id: número inteiro de 0 a n_bases - 1
   // coordenada x: número aleatório entre 0 e n_tamanho_mundo
   // coordenada y: número aleatório entre 0 e n_tamanho_mundo
   // capacidade: número aleatório entre 3 e 10
   // heróis presentes: conjunto que guarda o id dos heróis na base
   // heróis pendentes: fila implementada como lista que guarda o id dos heróis na fila
   for (i = 0; i < mundo->n_bases; i++) {
      mundo->v_bases[i].id_base = i;
      mundo->v_bases[i].coord_x = rand_num (0, mundo->n_tamanho_mundo);
      mundo->v_bases[i].coord_y = rand_num (0, mundo->n_tamanho_mundo);
      mundo->v_bases[i].capacidade = rand_num (3, 10);
      mundo->v_bases[i].herois_presentes = set_create (mundo->v_bases[i].capacidade);
      mundo->v_bases[i].herois_pendentes = lista_cria ();
   }

   return (1);
} 
// Inicializa cada característica das missões geradas do mundo.
int inicializa_missoes (struct mundo_t *mundo) {
   int i;
   int hab, n_hab_missao;
   
   mundo->v_missoes = malloc (mundo->n_missoes * sizeof (struct missao_t));
   if (!(mundo->v_missoes))
      return (0);
   // id: número inteiro entre 0 e n_missoes - 1
   // número de tentativas: número que descreve quantas vezes tentou-se executar a missão
   // missão cumprida: sinaliza se a missão foi ou não cumprida até o fim da simulação
   // coordenada x: número aleatório entre 0 e n_tamanho_mundo
   // coordenada y: número aleatório entre 0 e n_tamanho_mundo
   // habilidades requisitadas: conjunto que guarda as habilidades necessárias para a missão
   for (i = 0; i < mundo->n_missoes; i++) {
      mundo->v_missoes[i].id_missao = i;
      mundo->v_missoes[i].n_tentativas = 0;
      mundo->v_missoes[i].missao_cumprida = 0;
      mundo->v_missoes[i].coord_x = rand_num (0, mundo->n_tamanho_mundo);
      mundo->v_missoes[i].coord_y = rand_num (0, mundo->n_tamanho_mundo);
      mundo->v_missoes[i].req_habilidades = set_create (mundo->n_habilidades);
      n_hab_missao = rand_num (6, 10);
      do {
         hab = rand_num (0, 9);
         while (set_in (mundo->v_missoes[i].req_habilidades, hab))
            hab = rand_num (0, 9);
         set_add (mundo->v_missoes[i].req_habilidades, hab);
         n_hab_missao -= 1;
      } while (n_hab_missao != 0) ;
   }

   return (1);
}
// Inicializa cada característica do mundo.
void inicializa_mundo (struct mundo_t *mundo, struct lef_t *l, int t_fim) {
   int i;
   struct evento_t *ev;

   mundo->n_tamanho_mundo = 20000;
   mundo->n_habilidades = 10;
   mundo->n_herois = mundo->n_habilidades * 5;
   mundo->n_bases = mundo->n_herois / 6;
   mundo->n_missoes = t_fim / 100;

   inicializa_herois (mundo);
   inicializa_bases (mundo);
   inicializa_missoes (mundo);

   for (i = 0; i < mundo->n_herois; i++) {
      mundo->v_herois[i].base_atual = rand_num (0, mundo->n_bases - 1);
      ev = cria_evento (rand_num (0, 4320), EV_CHEGA, mundo->v_herois[i].id_heroi, mundo->v_herois[i].base_atual);
      insere_lef (l, ev);
   }
   
   for (i = 0; i < mundo->n_missoes; i++) {
      ev = cria_evento (rand_num (0, t_fim), EV_MISSAO, mundo->v_missoes[i].id_missao, 0);
      insere_lef (l, ev);
   }
   
   ev = cria_evento (t_fim, EV_FIM, 0, 0);
   insere_lef (l, ev);
}
// Executa o evento CHEGA.
void exec_ev_chega (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   // dados: T, H, B
   int espera;
   int heroi, base;
   int capacidade, n_herois_presentes;

   heroi = ev->dado1;
   base = ev->dado2;
   n_herois_presentes = set_card (mundo->v_bases[base].herois_presentes);
   capacidade = mundo->v_bases[base].capacidade;

   // Atualiza base de H <- Base de chegada
   mundo->v_herois[heroi].base_atual = mundo->v_bases[base].id_base;

   // Se há vagas em B e a fila de espera está vazia:
      // espera = true
   if (n_herois_presentes < capacidade) {
      if (lista_vazia (mundo->v_bases[base].herois_pendentes))
         espera = 1;
   }
   // Senão:
      // espera = (paciência de H) > (10 * tamanho da fila de B)
   else
      espera = (mundo->v_herois[heroi].paciencia) > (10 * lista_tamanho (mundo->v_bases[base].herois_pendentes));

   // Se espera:
      // Cria evento ESPERA (agora, H, B)
   if (espera) {
      printf ("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", ev->tempo, heroi, base, n_herois_presentes, capacidade);
      insere_lef (l, cria_evento (ev->tempo, EV_ESPERA, heroi, base));
   }
   // Senão:
      // Cria evento DESISTE (agora, H, B)
   else {
      printf ("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", ev->tempo, heroi, base, n_herois_presentes, capacidade);
      insere_lef (l, cria_evento (ev->tempo, EV_DESISTE, heroi, base));
   }
}
// Executa o evento ESPERA.
void exec_ev_espera (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   // dados: T, H, B
   int heroi, base, n_herois_pendentes;

   heroi = ev->dado1;
   base = ev->dado2;
   n_herois_pendentes = lista_tamanho (mundo->v_bases[base].herois_pendentes);

   // adiciona H no fim da fila de espera de B
   lista_insere (mundo->v_bases[base].herois_pendentes, heroi, -1);
   printf ("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", ev->tempo, heroi, base, n_herois_pendentes);
   // cria evento AVISA (agora, B)
   insere_lef (l, cria_evento (ev->tempo, EV_AVISA, base, 0));
}
// Executa o evento AVISA.
void exec_ev_avisa (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   // dados: T, B
   int heroi, base;
   int capacidade, n_herois_presentes;

   base = ev->dado1;
   capacidade = mundo->v_bases[base].capacidade;
   n_herois_presentes = set_card (mundo->v_bases[base].herois_presentes);
   // enquanto houver vaga em B e houver heróis esperando na fila:
    // retira primeiro herói (H') da fila de B
    // adiciona H' ao conjunto de heróis presentes em B
    // cria evento ENTRA (agora, H', B)
   printf ("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) ", ev->tempo, base, n_herois_presentes, mundo->v_bases[base].capacidade);
   lista_imprime ("FILA", mundo->v_bases[base].herois_pendentes);
   while (n_herois_presentes < capacidade && !lista_vazia (mundo->v_bases[base].herois_pendentes)) {
      lista_retira (mundo->v_bases[base].herois_pendentes, &heroi, 0);
      printf ("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, base, heroi);
      set_add (mundo->v_bases[base].herois_presentes, heroi);
      n_herois_presentes = set_card (mundo->v_bases[base].herois_presentes);
      insere_lef (l, cria_evento (ev->tempo, EV_ENTRA, heroi, base));
   }
}
// Executa o evento DESISTE.
void exec_ev_desiste (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   // dados: T, H
   int heroi, base_destino;

   heroi = ev->dado1;
   printf ("%6d: DESISTE HEROI %2d BASE %d\n", ev->tempo, heroi, mundo->v_herois[heroi].base_atual);
   // escolhe uma base destino D aleatória
   base_destino = rand_num (0, mundo->n_bases - 1);
   // cria evento VIAJA (agora, H, D)
   insere_lef (l, cria_evento (ev->tempo, EV_VIAJA, heroi, base_destino));
}
// Executa o evento ENTRA.
void exec_ev_entra (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   int tpb;
   int heroi, base, capacidade, n_herois_presentes;

   heroi = ev->dado1;
   base = ev->dado2;
   capacidade = mundo->v_bases[base].capacidade;
   n_herois_presentes = set_card (mundo->v_bases[base].herois_presentes);
   //calcula TPB = tempo de permanência na base:
      // TPB = 15 + paciência de H * aleatório [1...20]
   tpb = 15 + mundo->v_herois[heroi].paciencia * rand_num (1, 20);
   printf ("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", ev->tempo, heroi, base, n_herois_presentes, capacidade, ev->tempo + tpb);
   // cria evento SAI (agora + TPB, H, B)
   insere_lef (l, cria_evento (ev->tempo + tpb, EV_SAI, heroi, base));
}
// Executa o evento SAI.
void exec_ev_sai (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   int heroi, base, base_destino, capacidade, n_herois_presentes;

   heroi = ev->dado1;
   base = ev->dado2;
   capacidade = mundo->v_bases[base].capacidade;
   n_herois_presentes = set_card (mundo->v_bases[base].herois_presentes);
   // retira H do conjunto de heróis presentes em B
   set_del (mundo->v_bases[base].herois_presentes, heroi);
   printf ("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", ev->tempo, heroi, base, n_herois_presentes, capacidade);
   // escolhe uma base destino D aleatória
   base_destino = rand_num (0, mundo->n_bases - 1);
   // cria evento VIAJA (agora, H, D)
   insere_lef (l, cria_evento (ev->tempo, EV_VIAJA, heroi, base_destino));
   // cria evento AVISA (agora, B)
   insere_lef (l, cria_evento (ev->tempo, EV_AVISA, base, 0));
}
// Executa o evento VIAJA.
void exec_ev_viaja (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   int heroi, base_heroi, base_d;
   int distancia, duracao;
   int x_base_heroi, y_base_heroi, x_base_d, y_base_d;

   heroi = ev->dado1;
   base_d = ev->dado2;
   base_heroi = mundo->v_herois[heroi].base_atual;
   x_base_heroi = mundo->v_bases[base_heroi].coord_x;
   y_base_heroi = mundo->v_bases[base_heroi].coord_x;
   x_base_d = mundo->v_bases[base_d].coord_x;
   y_base_d = mundo->v_bases[base_d].coord_y;
   // calcula duração da viagem:
      // distância = distância cartesiana entre a base atual de H e a base D 
      // duração = distância / velocidade de H
   distancia = (int) roundf (sqrt((x_base_heroi - x_base_d) * (x_base_heroi - x_base_d) + (y_base_heroi - y_base_d) * (y_base_heroi - y_base_d)));
   duracao = distancia / mundo->v_herois[heroi].vel;
   printf ("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", ev->tempo, heroi, base_heroi, base_d, distancia, mundo->v_herois[heroi].vel, ev->tempo + duracao);
   // cria evento CHEGA (agora + duração, H, D)
   insere_lef (l, cria_evento (ev->tempo + duracao, EV_CHEGA, heroi, base_d));
}
// Ordena um vetor de structs distancia_base_missao por Insertion Sort.
void insertion_sort_missao (struct distancia_base_missao *v, int tam) {
   int i, j;
   struct distancia_base_missao chave;

   for (i = 1; i < tam; i++) {
      chave = v[i];
      j = i - 1;
      while (j >= 0 && v[j].distancia > chave.distancia) {
         v[j + 1] = v[j];
         j--;
      }
      v[j + 1] = chave;
   }
}
// Executa o evento MISSÃO.
void exec_ev_missao (struct mundo_t *mundo, struct lef_t *l, struct evento_t *ev) {
   int i, j;
   int achou;
   int base, missao, bmp;
   int x_base, y_base, x_missao, y_missao;
   struct set_t *conj_hab, *set_copia;
   struct distancia_base_missao *v_distancia_missao;

   missao = ev->dado1;
   printf ("%6d: MISSAO %d HAB REQ: ", ev->tempo, missao);
   set_print (mundo->v_missoes[missao].req_habilidades);
   // calcula a distância de cada base ao local da missão M
   v_distancia_missao = malloc (mundo->n_bases * sizeof (struct distancia_base_missao));
   for (i = 0; i < mundo->n_bases; i++) {
      base = mundo->v_bases[i].id_base;
      v_distancia_missao[i].id_base = base;
      x_base = mundo->v_bases[base].coord_x;
      y_base = mundo->v_bases[base].coord_x;
      x_missao = mundo->v_missoes[missao].coord_x;
      y_missao = mundo->v_missoes[missao].coord_y;
      v_distancia_missao[i].distancia = (int) sqrt ((x_base - x_missao) * (x_base - x_missao) + (y_base - y_missao) * (y_base - y_missao));
   }
   insertion_sort_missao (v_distancia_missao, mundo->n_bases);
   // encontra BMP = base mais próxima da missão cujos heróis possam cumpri-la 
   // se houver uma BMP:
      // incrementa a experiência dos heróis presentes na base BMP
   // senão:
      // cria evento MISSAO (T + 24*60, M) para o dia seguinte
   achou = 0;
   j = 0;
   while (!achou && j < mundo->n_bases) {
      bmp = v_distancia_missao[j].id_base; // BMP = Id da base mais próxima
      conj_hab = set_create (mundo->n_habilidades); // conj-hab = Conjunto de habilidades dos heróis da base
      set_copia = set_create (mundo->n_habilidades);
      for (i = 0; i < mundo->n_herois; i++)
         if (!set_empty (mundo->v_bases[bmp].herois_presentes) && set_in (mundo->v_bases[bmp].herois_presentes, i)) {
            set_copy (conj_hab, set_copia);
            set_union (set_copia, mundo->v_herois[i].habilidades, conj_hab);
         }
      printf ("%6d: MISSAO %d HAB BASE %d: ", ev->tempo, missao, bmp);
      set_print (conj_hab);
      if (set_contains (conj_hab, mundo->v_missoes[missao].req_habilidades))
         achou = 1;
      j++;
   }
   set_destroy (set_copia);
   set_destroy (conj_hab);
   free (v_distancia_missao);
   if (achou) {
      printf ("%6d: MISSAO %d CUMPRIDA BASE %d HEROIS: ", ev->tempo, missao, bmp);
      set_print (mundo->v_bases[bmp].herois_presentes);
      for (i = 0; i < mundo->n_herois; i++)
         if (set_in (mundo->v_bases[bmp].herois_presentes, i))
            (mundo->v_herois[i].xp)++;
      mundo->v_missoes[missao].missao_cumprida = 1;
   }
   else {
      printf ("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, missao);
      insere_lef (l, cria_evento (ev->tempo + 24 * 60, EV_MISSAO, missao, 0));
   }
   (mundo->v_missoes[missao].n_tentativas)++;
}
// Executa o evento FIM.
void exec_ev_fim (struct mundo_t *mundo, struct evento_t *ev) {
   int i;
   int n_missoes_cumpridas, soma_n_tentativas;
   float taxa_missoes, media_tent_missoes;

   printf ("%6d: FIM\n", ev->tempo);
   // Apresenta as experiências dos heróis
   for (i = 0; i < mundo->n_herois; i++) {
      printf ("HEROI %2d PAC %3d VEL %4d EXP %4d HABS ", mundo->v_herois[i].id_heroi, mundo->v_herois[i].paciencia, mundo->v_herois[i].vel, mundo->v_herois[i].xp);
      set_print (mundo->v_herois[i].habilidades);
   }
   // Apresenta as estatísticas das missões
   n_missoes_cumpridas = 0;
   soma_n_tentativas = 0;
   for (i = 0; i < mundo->n_missoes; i++) {
      if (mundo->v_missoes[i].missao_cumprida)
         n_missoes_cumpridas++;
      soma_n_tentativas += mundo->v_missoes[i].n_tentativas;
   }
   taxa_missoes = n_missoes_cumpridas / (mundo->n_missoes * 1.0);
   media_tent_missoes = soma_n_tentativas / (mundo->n_missoes * 1.0);
   printf ("%d/%d MISSOES CUMPRIDAS (%.2f%%), MEDIA %.2f TENTATIVAS/MISSAO\n", n_missoes_cumpridas, mundo->n_missoes, taxa_missoes, media_tent_missoes);
}
// Libera toda e qualquer estrutura do mundo.
void destroi_mundo (struct mundo_t *mundo) {
   int i;

   for (i = 0; i < mundo->n_herois; i++)
      set_destroy (mundo->v_herois[i].habilidades);
   free (mundo->v_herois);
   mundo->v_herois = NULL;

   for (i = 0; i < mundo->n_bases; i++) {
      set_destroy (mundo->v_bases[i].herois_presentes);
      lista_destroi (mundo->v_bases[i].herois_pendentes);
   }
   free (mundo->v_bases);
   mundo->v_bases = NULL;

   for (i = 0; i < mundo->n_missoes; i++)
      set_destroy (mundo->v_missoes[i].req_habilidades);
   free (mundo->v_missoes);
   mundo->v_missoes = NULL;

   free (mundo);
   mundo = NULL;
}

int main () {
   // Variáveis que indicam o tempo de início e tempo de fim
   // da simulação, respectivamente.
   int t_inicio, t_fim_do_mundo;

   struct lef_t *l;
   struct mundo_t *mundo;
   struct evento_t *ev;

   // Geração da semente
   srand (0);

   // Inicialização da Lista de Eventos Futuros
   l = cria_lef ();
   // Inicialização do mundo da simulação
   mundo = malloc (sizeof (struct mundo_t));
   if (!mundo)
      return (1);
   t_inicio = 0;
   t_fim_do_mundo = 525600;
   inicializa_mundo (mundo, l, t_fim_do_mundo);

   // Execução da simulação
   mundo->clock = t_inicio;
   // 1. Retira-se o primeiro evento da LEF
   // 2. O tempo da simulação torna-se o tempo do evento
   // 3. É avaliado o tipo do evento
   // 4. Executa-se o evento correspondente ao tipo do evento
   while (mundo->clock < t_fim_do_mundo) {
      ev = retira_lef (l);
      mundo->clock = ev->tempo;
      switch (ev->tipo) {
         case EV_CHEGA:
            // Herói H chega na base B no instante T
            exec_ev_chega (mundo, l, ev);
            break;
         case EV_ESPERA:
            // Herói H entra na fila de espera de B no instante T
            exec_ev_espera (mundo, l, ev);
            break;
         case EV_DESISTE:
            // Herói H desiste de entrar na base B e voa para uma base D
            exec_ev_desiste (mundo, l, ev);
            break;
         case EV_AVISA:
            // Porteiro da base B trata a fila de espera
            exec_ev_avisa (mundo, l, ev);
            break;
         case EV_ENTRA:
            // Herói H entra na base B. Herói decide quanto tempo fica e agenda sua saída.
            exec_ev_entra (mundo, l, ev);
            break;
         case EV_SAI:
            // Herói H sai da base B. Ao sair, escolhe uma base de destino; O porteiro da base B é avisado.
            exec_ev_sai (mundo, l, ev);
            break;
         case EV_VIAJA:
            // Herói H se desloca para base D (pode ser a mesma onde está)
            exec_ev_viaja (mundo, l, ev);
            break;
         case EV_MISSAO:
            // Missão M é disparada no instante T. Características da missão:
            // 1. Cada missão ocorre em um local aleatório e requer um conjunto aleatório de habilidades; ambos são definidos durante a inicialização.
            // 2. Cada equipe é formada pelo conjunto de heróis presentes em uma base.
            // 3. Uma equipe está apta para a missão se a união das habilidades de seus heróis contém as habilidades requeridas pela missão.
            // 4. Deve ser escolhida para a missão a equipe da base mais próxima ao local da missão e que esteja apta para ela.
            // 5. Ao completar uma missão, os heróis da equipe escolhida ganham pontos de experiência.
            // 6. Se uma missão não puder ser completada, ela é marcada como “impossível” e adiada de 24 horas. 
            exec_ev_missao (mundo, l, ev);
            break;
         case EV_FIM:
            // Encerra a simulação no instante T
            exec_ev_fim (mundo, ev);
            break;
      }
      destroi_evento (ev);
   }

   // Destruição do mundo...
   destroi_mundo (mundo);
   destroi_lef (l);

   return (0) ;
}