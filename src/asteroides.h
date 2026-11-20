#ifndef ASTEROIDES_H
#define ASTEROIDES_H


typedef struct Asteroide {
    int x;
    int y;
    struct Asteroide *prox;
} Asteroide;


void asteroides_inicializar(Asteroide **lista);


int asteroides_criar(Asteroide **lista, int limite_esq, int limite_dir, int y_inicial);

void asteroides_mover_e_remover_fora(Asteroide **lista, int limite_inferior, int *desviados, int *pontos,
                                     int multiplicador);


int asteroides_verificar_colisao(Asteroide **lista, int nave_x, int nave_y, int nave_largura);


void asteroides_desenhar(const Asteroide *lista);


void asteroides_limpar(Asteroide **lista);

#endif 
