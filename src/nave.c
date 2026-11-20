#include <stdio.h>

#include "nave.h"
#include "screen.h"

#define NAVE_LARGURA 3
#define NAVE_VIDAS_INICIAIS 3
#define NAVE_SPRITE "^^^"

/* Limites internos da área jogável (dentro da moldura ASCII da partida) */
#define JOGO_X_ESQ (SCRSTARTX + 1)
#define JOGO_X_DIR (SCRENDX - 1)
#define JOGO_Y_BASE (SCRENDY - 1)

void nave_inicializar(Nave *nave) {
    nave->largura = NAVE_LARGURA;
    nave->vidas = NAVE_VIDAS_INICIAIS;
    /* Posiciona a nave fixa na parte inferior da área interna */
    nave->y = JOGO_Y_BASE;
    /* Centraliza horizontalmente dentro da área interna */
    nave->x = JOGO_X_ESQ + ((JOGO_X_DIR - JOGO_X_ESQ + 1 - nave->largura) / 2);

    /* Garante que a posição inicial respeite os limites internos */
    if (nave->x < JOGO_X_ESQ) {
        nave->x = JOGO_X_ESQ;
    }
    if (nave->x + nave->largura - 1 > JOGO_X_DIR) {
        nave->x = JOGO_X_DIR - nave->largura + 1;
    }
}

void nave_mover_esquerda(Nave *nave) {
    if (nave->x > JOGO_X_ESQ) {
        nave->x--;
    }
}

void nave_mover_direita(Nave *nave) {
    if (nave->x + nave->largura - 1 < JOGO_X_DIR) {
        nave->x++;
    }
}

void nave_desenhar(const Nave *nave) {
    screenSetColor(GREEN, BLACK);
    screenGotoxy(nave->x, nave->y);
    printf("%s", NAVE_SPRITE);
}

void nave_apagar(const Nave *nave) {
    screenSetColor(WHITE, BLACK);
    screenGotoxy(nave->x, nave->y);
    printf("   ");
}