#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "asteroides.h"
#include "screen.h"

#define ASTEROIDE_CHAR '*'
#define PONTOS_POR_ASTEROIDE_DESVIADO 10

static int srand_inicializado = 0;

void asteroides_inicializar(Asteroide **lista) {
    if (lista != NULL) {
        *lista = NULL;
    }
    if (!srand_inicializado) {
        srand((unsigned int)time(NULL));
        srand_inicializado = 1;
    }
}

int asteroides_criar(Asteroide **lista, int limite_esq, int limite_dir, int y_inicial) {
    if (lista == NULL || limite_dir < limite_esq) {
        return 0;
    }

    Asteroide *novo = (Asteroide *)malloc(sizeof(Asteroide));
    if (novo == NULL) {
       
        return 0;
    }

    int intervalo = limite_dir - limite_esq + 1;
    novo->x = limite_esq + (rand() % intervalo);
    novo->y = y_inicial;
    novo->prox = *lista;
    *lista = novo;

    return 1;
}

void asteroides_mover_e_remover_fora(Asteroide **lista, int limite_inferior, int *desviados, int *pontos,
                                     int multiplicador) {
    if (lista == NULL) {
        return;
    }

    Asteroide **atual = lista;
    while (*atual != NULL) {
        Asteroide *no = *atual;
        no->y++;

        if (no->y > limite_inferior) {
            
            *atual = no->prox;
            free(no);
            if (desviados != NULL) {
                (*desviados)++;
            }
            if (pontos != NULL) {
                *pontos += PONTOS_POR_ASTEROIDE_DESVIADO * multiplicador;
            }
            
        } else {
            atual = &no->prox;
        }
    }
}

int asteroides_verificar_colisao(Asteroide **lista, int nave_x, int nave_y, int nave_largura) {
    if (lista == NULL) {
        return 0;
    }

    Asteroide **atual = lista;
    while (*atual != NULL) {
        Asteroide *no = *atual;
       
        if (no->y == nave_y && no->x >= nave_x && no->x <= nave_x + nave_largura - 1) {
            
            *atual = no->prox;
            free(no);
            return 1;
        }
        atual = &no->prox;
    }

    return 0;
}

void asteroides_desenhar(const Asteroide *lista) {
    screenSetColor(LIGHTRED, BLACK);
    const Asteroide *atual = lista;
    while (atual != NULL) {
        screenGotoxy(atual->x, atual->y);
        printf("%c", ASTEROIDE_CHAR);
        atual = atual->prox;
    }
}

void asteroides_limpar(Asteroide **lista) {
    if (lista == NULL) {
        return;
    }

    while (*lista != NULL) {
        Asteroide *no = *lista;
        *lista = no->prox;
        free(no);
    }
   
    *lista = NULL;
}
