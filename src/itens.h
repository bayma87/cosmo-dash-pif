#ifndef ITENS_H
#define ITENS_H

/* Tipos de itens coletáveis */
typedef enum {
    ITEM_VIDA_EXTRA,
    ITEM_PONTOS_BONUS
} TipoItem;

/* Nó da lista simplesmente encadeada de itens */
typedef struct Item {
    int x;
    int y;
    TipoItem tipo;
    struct Item *prox;
} Item;

/* Inicializa a lista de itens como vazia */
void itens_inicializar(Item **lista);

/* Cria e insere um item aleatório na linha de surgimento */
int itens_criar(Item **lista, int limite_esq, int limite_dir, int y_inicial);

/* Move itens para baixo e remove os que saíram da área (não contam como coletados) */
void itens_mover_e_remover_fora(Item **lista, int limite_inferior);

/* Verifica coleta: aplica efeitos e multiplicador de pontos; retorna 1 se coletou */
int itens_verificar_coleta(Item **lista, int nave_x, int nave_y, int nave_largura,
                           int *vidas, int *pontos, int *itens_coletados, int multiplicador);

/* Desenha todos os itens na tela */
void itens_desenhar(const Item *lista);

/* Libera todos os itens e define a cabeça da lista como NULL */
void itens_limpar(Item **lista);

#endif /* ITENS_H */