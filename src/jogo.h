#ifndef JOGO_H
#define JOGO_H

#include "asteroides.h"
#include "itens.h"
#include "nave.h"

/*
 * Estados principais do jogo Cosmo Dash.
 */
typedef enum {
    ESTADO_MENU,
    ESTADO_PARTIDA,
    ESTADO_GAME_OVER
} EstadoJogo;

/*
 * Estrutura principal do jogo.
 * Contém estado, nave, listas de asteroides/itens, contadores,
 * acumuladores de asteroides e recorde.
 */
typedef struct {
    EstadoJogo estado;
    Nave nave;
    Asteroide *asteroides;
    Item *itens;
    int ticks_partida;
    int tempo_segundos;
    int asteroides_desviados;
    int itens_coletados;
    int pontos;
    int recorde;
    int falha_salvar_recorde;
    /* Acumuladores inteiros para criação e movimento de asteroides */
    int acum_spawn_ast;
    int acum_move_ast;
    int tick_spawn_item;
    int tick_move_item;
    /* Contadores do BONUS TIME (ticks da partida) */
    int ticks_sem_dano;
    int ticks_bonus_restantes;
    int ticks_aviso;
    int executando;
} Jogo;

/* Inicializa o estado do jogo e carrega o recorde */
void jogo_inicializar(Jogo *jogo);

/* Executa o loop principal controlando os estados do jogo */
void jogo_executar(Jogo *jogo);

#endif /* JOGO_H */