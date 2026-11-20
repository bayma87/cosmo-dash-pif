#include <stdio.h>

#include "asteroides.h"
#include "itens.h"
#include "jogo.h"
#include "keyboard.h"
#include "nave.h"
#include "recorde.h"
#include "screen.h"
#include "timer.h"


#define ARQUIVO_RECORDE "recorde.txt"


#define HUD_Y           (SCRSTARTY + 1)
#define INSTRUCOES_Y    (SCRSTARTY + 2)
#define MOLDURA_TOPO_Y  (SCRSTARTY + 3)
#define MOLDURA_BASE_Y  (SCRENDY)
#define JOGO_X_ESQ      (SCRSTARTX + 1)
#define JOGO_X_DIR      (SCRENDX - 1)
#define JOGO_Y_TOPO     (MOLDURA_TOPO_Y + 1)
#define JOGO_Y_BASE     (MOLDURA_BASE_Y - 1)

#define ARENA_LINHAS    (JOGO_Y_BASE - JOGO_Y_TOPO + 1)
#define ARENA_COLUNAS   (JOGO_X_DIR - JOGO_X_ESQ + 1)


#define NAVE_CARACTERE  '^'


#define TICKS_POR_SEGUNDO      20
#define TICKS_SPAWN_ITEM      100
#define TICKS_MOVE_ITEM         5


#define TICKS_PARA_BONUS        (20 * TICKS_POR_SEGUNDO)
#define TICKS_DURACAO_BONUS     (10 * TICKS_POR_SEGUNDO)
#define TICKS_FASE_AVISO         5
#define TICKS_AVISO_BONUS        (3 * 2 * TICKS_FASE_AVISO)
#define MULTIPLICADOR_NORMAL     1
#define MULTIPLICADOR_BONUS      2


#define AVISO_Y                 SCRSTARTY
#define AVISO_TEXTO             ">>> BONUS TIME!!! <<<"
#define AVISO_LARGURA           (sizeof(AVISO_TEXTO) - 1)
#define AVISO_X                 (SCRSTARTX + (SCRENDX - SCRSTARTX + 1 - AVISO_LARGURA) / 2)


static char arena[ARENA_LINHAS][ARENA_COLUNAS];
static screenColor arena_cores[ARENA_LINHAS][ARENA_COLUNAS];


static void jogo_cancelar_bonus(Jogo *jogo) {
    jogo->ticks_sem_dano = 0;
    jogo->ticks_bonus_restantes = 0;
    jogo->ticks_aviso = 0;
}


static int jogo_multiplicador_pontos(const Jogo *jogo) {
    return (jogo->ticks_bonus_restantes > 0) ? MULTIPLICADOR_BONUS : MULTIPLICADOR_NORMAL;
}


static void jogo_atualizar_bonus(Jogo *jogo) {
    if (jogo->ticks_aviso > 0) {
        jogo->ticks_aviso--;
    }

    
    if (jogo->ticks_bonus_restantes > 0) {
        jogo->ticks_bonus_restantes--;
        return;
    }

    if (jogo->ticks_sem_dano < TICKS_PARA_BONUS) {
        jogo->ticks_sem_dano++;
        if (jogo->ticks_sem_dano >= TICKS_PARA_BONUS) {
           
            jogo->ticks_sem_dano = 0;
            jogo->ticks_bonus_restantes = TICKS_DURACAO_BONUS;
            jogo->ticks_aviso = TICKS_AVISO_BONUS;
        }
    }
}


static void jogo_limpar_listas(Jogo *jogo) {
    asteroides_limpar(&jogo->asteroides);
    itens_limpar(&jogo->itens);
}


static void jogo_finalizar_partida(Jogo *jogo) {
    
    jogo_cancelar_bonus(jogo);
   
    jogo_limpar_listas(jogo);

    jogo->falha_salvar_recorde = 0;
    if (jogo->pontos > jogo->recorde) {
        jogo->recorde = jogo->pontos;
        
        if (!recorde_salvar(ARQUIVO_RECORDE, jogo->recorde)) {
            jogo->falha_salvar_recorde = 1;
        }
    }

    jogo->estado = ESTADO_GAME_OVER;
}


static void jogo_processar_colisoes_asteroides(Jogo *jogo) {
    while (jogo->nave.vidas > 0 &&
           asteroides_verificar_colisao(&jogo->asteroides, jogo->nave.x, jogo->nave.y, jogo->nave.largura)) {
        jogo->nave.vidas--;
       
        jogo_cancelar_bonus(jogo);
    }

    if (jogo->nave.vidas <= 0) {
        jogo->nave.vidas = 0;
        jogo_finalizar_partida(jogo);
    }
}


static void jogo_coletar_itens(Jogo *jogo) {
    int multiplicador = jogo_multiplicador_pontos(jogo);


    while (jogo->estado == ESTADO_PARTIDA) {
        if (!itens_verificar_coleta(&jogo->itens, jogo->nave.x, jogo->nave.y, jogo->nave.largura,
                                    &jogo->nave.vidas, &jogo->pontos, &jogo->itens_coletados,
                                    multiplicador)) {
            break;
        }
    }
}


static void jogo_iniciar_partida(Jogo *jogo) {
    jogo_limpar_listas(jogo);
    jogo_cancelar_bonus(jogo);
    jogo->ticks_partida = 0;
    jogo->tempo_segundos = 0;
    jogo->asteroides_desviados = 0;
    jogo->itens_coletados = 0;
    jogo->pontos = 0;
    jogo->falha_salvar_recorde = 0;
    jogo->acum_spawn_ast = 0;
    jogo->acum_move_ast = 0;
    jogo->tick_spawn_item = 0;
    jogo->tick_move_item = 0;
    nave_inicializar(&jogo->nave);
   
    timerUpdateTimer(50);
}


static void renderizar_menu(void) {
    screenClear();
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(30, 6);
    printf("COSMO DASH");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(28, 10);
    printf("1 - Iniciar partida");
    screenGotoxy(28, 12);
    printf("2 - Sair");
    screenGotoxy(24, 16);
    printf("Pressione 1 ou 2");

    screenUpdate();
}


static int processar_menu(Jogo *jogo) {
    if (!keyhit()) {
        return 1;
    }

    int tecla = readch();
    if (tecla == '1') {
        jogo_iniciar_partida(jogo);
        jogo->estado = ESTADO_PARTIDA;
    } else if (tecla == '2') {
        jogo->executando = 0;
        return 0;
    }
    
    return 1;
}


static void desenhar_moldura(void) {
    int x;
    int y;

    screenSetColor(WHITE, BLACK);

 
    screenGotoxy(SCRSTARTX, MOLDURA_TOPO_Y);
    printf("+");
    for (x = SCRSTARTX + 1; x < SCRENDX; x++) {
        printf("-");
    }
    printf("+");

 
    for (y = JOGO_Y_TOPO; y <= JOGO_Y_BASE; y++) {
        screenGotoxy(SCRSTARTX, y);
        printf("|");
        screenGotoxy(SCRENDX, y);
        printf("|");
    }

    
    screenGotoxy(SCRSTARTX, MOLDURA_BASE_Y);
    printf("+");
    for (x = SCRSTARTX + 1; x < SCRENDX; x++) {
        printf("-");
    }
    printf("+");
}


static void desenhar_hud(const Jogo *jogo) {
    int segundos_bonus;

    screenSetColor(CYAN, BLACK);
    screenGotoxy(SCRSTARTX, HUD_Y);
    printf("Vidas:%d/%d Pontos:%d", jogo->nave.vidas, NAVE_VIDAS_MAXIMAS, jogo->pontos);
    if (jogo_multiplicador_pontos(jogo) == MULTIPLICADOR_BONUS) {
        printf(" x2");
    } else {
        printf("   ");
    }
    printf(" Tempo:%ds Desv:%d  +Vida $Pts", jogo->tempo_segundos, jogo->asteroides_desviados);
    if (jogo->ticks_bonus_restantes > 0) {
        segundos_bonus = (jogo->ticks_bonus_restantes + TICKS_POR_SEGUNDO - 1) / TICKS_POR_SEGUNDO;
        printf(" Bonus:%ds", segundos_bonus);
    } else {
        printf("          "); 
    }
    printf("  ");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(SCRSTARTX, INSTRUCOES_Y);
    printf("A/D:mover M:menu Q:sair                                ");
}


static void desenhar_aviso(const Jogo *jogo) {
    int visivel = 0;
    int i;

    if (jogo->ticks_aviso > 0) {
        int decorridos = TICKS_AVISO_BONUS - jogo->ticks_aviso;
        visivel = (decorridos % (2 * TICKS_FASE_AVISO)) < TICKS_FASE_AVISO;
    }

    screenSetColor(YELLOW, BLACK);
    screenGotoxy(AVISO_X, AVISO_Y);
    if (visivel) {
        printf("%s", AVISO_TEXTO);
    } else {
        for (i = 0; i < (int)AVISO_LARGURA; i++) {
            printf(" ");
        }
    }
}


static void arena_limpar(void) {
    int linha;
    int coluna;
    for (linha = 0; linha < ARENA_LINHAS; linha++) {
        for (coluna = 0; coluna < ARENA_COLUNAS; coluna++) {
            arena[linha][coluna] = ' ';
            arena_cores[linha][coluna] = WHITE;
        }
    }
}


static void arena_marcar(int x, int y, char simbolo, screenColor cor) {
    int coluna = x - JOGO_X_ESQ;
    int linha = y - JOGO_Y_TOPO;
    if (linha < 0 || linha >= ARENA_LINHAS || coluna < 0 || coluna >= ARENA_COLUNAS) {
        return;
    }
    arena[linha][coluna] = simbolo;
    arena_cores[linha][coluna] = cor;
}


static void arena_montar(const Jogo *jogo) {
    const Asteroide *asteroide;
    const Item *item;
    int coluna;

    arena_limpar();

    asteroide = jogo->asteroides;
    while (asteroide != NULL) {
        arena_marcar(asteroide->x, asteroide->y, '*', LIGHTRED);
        asteroide = asteroide->prox;
    }

    item = jogo->itens;
    while (item != NULL) {
        if (item->tipo == ITEM_VIDA_EXTRA) {
            arena_marcar(item->x, item->y, '+', GREEN);
        } else {
            arena_marcar(item->x, item->y, '$', YELLOW);
        }
        item = item->prox;
    }

    for (coluna = 0; coluna < jogo->nave.largura; coluna++) {
        arena_marcar(jogo->nave.x + coluna, jogo->nave.y, NAVE_CARACTERE, GREEN);
    }
}


static void arena_exibir(void) {
    int linha;
    int coluna;

    for (linha = 0; linha < ARENA_LINHAS; linha++) {
        screenColor cor_atual = arena_cores[linha][0];
        screenGotoxy(JOGO_X_ESQ, JOGO_Y_TOPO + linha);
        screenSetColor(cor_atual, BLACK);
        for (coluna = 0; coluna < ARENA_COLUNAS; coluna++) {
            if (arena_cores[linha][coluna] != cor_atual) {
                cor_atual = arena_cores[linha][coluna];
                screenSetColor(cor_atual, BLACK);
            }
            printf("%c", arena[linha][coluna]);
        }
    }
}


static void renderizar_partida(const Jogo *jogo) {
    desenhar_moldura();
    desenhar_hud(jogo);
    desenhar_aviso(jogo);
    arena_montar(jogo);
    arena_exibir();
    screenUpdate();
}


static void jogo_mover_nave(Jogo *jogo, int para_esquerda) {
    nave_apagar(&jogo->nave);
    if (para_esquerda) {
        nave_mover_esquerda(&jogo->nave);
    } else {
        nave_mover_direita(&jogo->nave);
    }

   
    jogo_processar_colisoes_asteroides(jogo);
    if (jogo->estado != ESTADO_PARTIDA) {
        return;
    }

    jogo_coletar_itens(jogo);
    if (jogo->estado != ESTADO_PARTIDA) {
        return;
    }

    
    nave_desenhar(&jogo->nave);
    desenhar_hud(jogo);
    desenhar_aviso(jogo);
    screenUpdate();
}


static void renderizar_game_over(const Jogo *jogo) {
    screenClear();
    screenSetColor(LIGHTRED, BLACK);
    screenGotoxy(30, 6);
    printf("GAME OVER");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(24, 9);
    printf("Pontos: %d", jogo->pontos);
    screenGotoxy(24, 10);
    printf("Tempo sobrevivido: %d s", jogo->tempo_segundos);
    screenGotoxy(24, 11);
    printf("Asteroides desviados: %d", jogo->asteroides_desviados);
    screenGotoxy(24, 12);
    printf("Itens coletados: %d", jogo->itens_coletados);
    screenGotoxy(24, 13);
    printf("Recorde: %d", jogo->recorde);

    if (jogo->falha_salvar_recorde) {
        screenSetColor(YELLOW, BLACK);
        screenGotoxy(20, 15);
        printf("Nao foi possivel salvar o recorde.");
        screenSetColor(WHITE, BLACK);
        screenGotoxy(20, 17);
        printf("R - Reiniciar / M - Menu / Q - Sair");
    } else {
        screenGotoxy(20, 15);
        printf("R - Reiniciar / M - Menu / Q - Sair");
    }

    screenUpdate();
}


static int processar_game_over(Jogo *jogo) {
    if (!keyhit()) {
        return 1;
    }

    int tecla = readch();
    if (tecla == 'r' || tecla == 'R') {
       
        jogo_iniciar_partida(jogo);
        jogo->estado = ESTADO_PARTIDA;
    } else if (tecla == 'm' || tecla == 'M') {
        jogo_limpar_listas(jogo);
        jogo->estado = ESTADO_MENU;
    } else if (tecla == 'q' || tecla == 'Q') {
        jogo->executando = 0;
        return 0;
    }
    
    return 1;
}


static int processar_partida(Jogo *jogo) {
    if (!keyhit()) {
        return 1;
    }

    int tecla = readch();
    if (tecla == 'a' || tecla == 'A') {
        jogo_mover_nave(jogo, 1);
    } else if (tecla == 'd' || tecla == 'D') {
        jogo_mover_nave(jogo, 0);
    } else if (tecla == 'm' || tecla == 'M') {
        
        jogo_cancelar_bonus(jogo);
        jogo_limpar_listas(jogo);
        jogo->estado = ESTADO_MENU;
    } else if (tecla == 'q' || tecla == 'Q') {
       
        jogo_cancelar_bonus(jogo);
        jogo->executando = 0;
        return 0;
    }
  
    return 1;
}


static void atualizar_tick_partida(Jogo *jogo) {
    int taxa_movimento;
    int taxa_criacao;
    int multiplicador;

    jogo->ticks_partida++;
    
    jogo_atualizar_bonus(jogo);
    multiplicador = jogo_multiplicador_pontos(jogo);

    
    if (jogo->ticks_partida % TICKS_POR_SEGUNDO == 0) {
        jogo->tempo_segundos++;
    }

    
    taxa_movimento = 30 + jogo->tempo_segundos / 3;
    if (taxa_movimento > 55) {
        taxa_movimento = 55;
    }

    
    jogo->acum_move_ast += taxa_movimento;
    if (jogo->acum_move_ast >= 100) {
        asteroides_mover_e_remover_fora(&jogo->asteroides, JOGO_Y_BASE,
                                        &jogo->asteroides_desviados, &jogo->pontos, multiplicador);
        jogo->acum_move_ast -= 100;
       
        if (jogo->estado == ESTADO_PARTIDA) {
            jogo_processar_colisoes_asteroides(jogo);
        }
    }

   
    if (jogo->estado != ESTADO_PARTIDA) {
        return;
    }

    
    taxa_criacao = 15 + jogo->tempo_segundos / 2;
    if (taxa_criacao > 70) {
        taxa_criacao = 70;
    }

    jogo->acum_spawn_ast += taxa_criacao;
    if (jogo->acum_spawn_ast >= 100) {
        asteroides_criar(&jogo->asteroides, JOGO_X_ESQ, JOGO_X_DIR, JOGO_Y_TOPO);
        jogo->acum_spawn_ast -= 100;
    }

    
    jogo->tick_spawn_item++;
    if (jogo->tick_spawn_item >= TICKS_SPAWN_ITEM) {
        itens_criar(&jogo->itens, JOGO_X_ESQ, JOGO_X_DIR, JOGO_Y_TOPO);
        jogo->tick_spawn_item = 0;
    }

   
    jogo->tick_move_item++;
    if (jogo->tick_move_item >= TICKS_MOVE_ITEM) {
        itens_mover_e_remover_fora(&jogo->itens, JOGO_Y_BASE);
        jogo->tick_move_item = 0;
    }

   
    jogo_coletar_itens(jogo);

    
    if (jogo->estado == ESTADO_PARTIDA) {
        renderizar_partida(jogo);
    }
}

void jogo_inicializar(Jogo *jogo) {
    
    jogo->asteroides = NULL;
    jogo->itens = NULL;

    jogo->estado = ESTADO_MENU;
    jogo->ticks_partida = 0;
    jogo->tempo_segundos = 0;
    jogo->asteroides_desviados = 0;
    jogo->itens_coletados = 0;
    jogo->pontos = 0;
    jogo->recorde = 0;
    jogo->falha_salvar_recorde = 0;
    jogo->acum_spawn_ast = 0;
    jogo->acum_move_ast = 0;
    jogo->tick_spawn_item = 0;
    jogo->tick_move_item = 0;
    jogo_cancelar_bonus(jogo);
    jogo->executando = 1;

    nave_inicializar(&jogo->nave);
    asteroides_inicializar(&jogo->asteroides);
    itens_inicializar(&jogo->itens);

    
    jogo->recorde = recorde_carregar(ARQUIVO_RECORDE);
}

void jogo_executar(Jogo *jogo) {
    EstadoJogo estado_anterior = (EstadoJogo)(-1);

    while (jogo->executando) {
        if (jogo->estado == ESTADO_MENU) {
            if (estado_anterior != ESTADO_MENU) {
                renderizar_menu();
                estado_anterior = ESTADO_MENU;
            }
            if (!processar_menu(jogo)) {
                break;
            }
            if (jogo->estado != ESTADO_MENU) {
                estado_anterior = (EstadoJogo)(-1);
            }
        } else if (jogo->estado == ESTADO_PARTIDA) {
            if (estado_anterior != ESTADO_PARTIDA) {
                screenClear();
                renderizar_partida(jogo);
                estado_anterior = ESTADO_PARTIDA;
            }

            if (!processar_partida(jogo)) {
                break;
            }

            if (jogo->estado == ESTADO_PARTIDA && timerTimeOver() == 1) {
                atualizar_tick_partida(jogo);
            }

            if (jogo->estado != ESTADO_PARTIDA) {
                estado_anterior = (EstadoJogo)(-1);
            }
        } else if (jogo->estado == ESTADO_GAME_OVER) {
            if (estado_anterior != ESTADO_GAME_OVER) {
                renderizar_game_over(jogo);
                estado_anterior = ESTADO_GAME_OVER;
            }
            if (!processar_game_over(jogo)) {
                break;
            }
            if (jogo->estado != ESTADO_GAME_OVER) {
                estado_anterior = (EstadoJogo)(-1);
            }
        }
    }

    /* Garante liberação de listas ao sair do loop */
    jogo_limpar_listas(jogo);
}
