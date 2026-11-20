#ifndef JOGO_H
#define JOGO_H

#include "asteroides.h"
#include "itens.h"
#include "nave.h"


typedef enum {
    ESTADO_MENU,
    ESTADO_PARTIDA,
    ESTADO_GAME_OVER
} EstadoJogo;


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
   
    int acum_spawn_ast;
    int acum_move_ast;
    int tick_spawn_item;
    int tick_move_item;
    
    int ticks_sem_dano;
    int ticks_bonus_restantes;
    int ticks_aviso;
    int executando;
} Jogo;


void jogo_inicializar(Jogo *jogo);


void jogo_executar(Jogo *jogo);

#endif 
