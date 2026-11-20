#ifndef ITENS_H
#define ITENS_H


typedef enum {
    ITEM_VIDA_EXTRA,
    ITEM_PONTOS_BONUS
} TipoItem;


typedef struct Item {
    int x;
    int y;
    TipoItem tipo;
    struct Item *prox;
} Item;


void itens_inicializar(Item **lista);


int itens_criar(Item **lista, int limite_esq, int limite_dir, int y_inicial);


void itens_mover_e_remover_fora(Item **lista, int limite_inferior);


int itens_verificar_coleta(Item **lista, int nave_x, int nave_y, int nave_largura,
                           int *vidas, int *pontos, int *itens_coletados, int multiplicador);


void itens_desenhar(const Item *lista);


void itens_limpar(Item **lista);

#endif 
