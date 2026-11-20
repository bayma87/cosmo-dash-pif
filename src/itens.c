#include <stdio.h>
#include <stdlib.h>

#include "itens.h"
#include "nave.h"
#include "screen.h"

#define PONTOS_BONUS_ITEM 50

void itens_inicializar(Item **lista) {
    if (lista != NULL) {
        *lista = NULL;
    }
}

int itens_criar(Item **lista, int limite_esq, int limite_dir, int y_inicial) {
    if (lista == NULL || limite_dir < limite_esq) {
        return 0;
    }

    Item *novo = (Item *)malloc(sizeof(Item));
    if (novo == NULL) {
        /* Falha de alocação: não acessa ponteiro NULL */
        return 0;
    }

    int intervalo = limite_dir - limite_esq + 1;
    novo->x = limite_esq + (rand() % intervalo);
    novo->y = y_inicial;
    novo->tipo = (rand() % 2 == 0) ? ITEM_VIDA_EXTRA : ITEM_PONTOS_BONUS;
    novo->prox = *lista;
    *lista = novo;

    return 1;
}

void itens_mover_e_remover_fora(Item **lista, int limite_inferior) {
    if (lista == NULL) {
        return;
    }

    Item **atual = lista;
    while (*atual != NULL) {
        Item *no = *atual;
        no->y++;

        if (no->y > limite_inferior) {
            /* Item perdido: remove sem contar como coletado */
            *atual = no->prox;
            free(no);
            /* Não avança: *atual já aponta para o próximo */
        } else {
            atual = &no->prox;
        }
    }
}

int itens_verificar_coleta(Item **lista, int nave_x, int nave_y, int nave_largura,
                           int *vidas, int *pontos, int *itens_coletados, int multiplicador) {
    if (lista == NULL) {
        return 0;
    }

    Item **atual = lista;
    while (*atual != NULL) {
        Item *no = *atual;
        /* Mesma lógica de sobreposição horizontal da nave */
        if (no->y == nave_y && no->x >= nave_x && no->x <= nave_x + nave_largura - 1) {
            if (no->tipo == ITEM_VIDA_EXTRA) {
                /* No teto, apenas consome o item; sem vida extra */
                if (vidas != NULL && *vidas < NAVE_VIDAS_MAXIMAS) {
                    (*vidas)++;
                }
            } else if (no->tipo == ITEM_PONTOS_BONUS) {
                if (pontos != NULL) {
                    *pontos += PONTOS_BONUS_ITEM * multiplicador;
                }
            }
            if (itens_coletados != NULL) {
                (*itens_coletados)++;
            }

            /* Remove e libera o item coletado */
            *atual = no->prox;
            free(no);
            return 1;
        }
        atual = &no->prox;
    }

    return 0;
}

void itens_desenhar(const Item *lista) {
    const Item *atual = lista;
    while (atual != NULL) {
        screenGotoxy(atual->x, atual->y);
        if (atual->tipo == ITEM_VIDA_EXTRA) {
            screenSetColor(GREEN, BLACK);
            printf("+");
        } else {
            screenSetColor(YELLOW, BLACK);
            printf("$");
        }
        atual = atual->prox;
    }
}

void itens_limpar(Item **lista) {
    if (lista == NULL) {
        return;
    }

    while (*lista != NULL) {
        Item *no = *lista;
        *lista = no->prox;
        free(no);
    }
    *lista = NULL;
}