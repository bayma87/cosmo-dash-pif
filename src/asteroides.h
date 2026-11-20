#ifndef ASTEROIDES_H
#define ASTEROIDES_H

/* Nó da lista simplesmente encadeada de asteroides */
typedef struct Asteroide {
    int x;
    int y;
    struct Asteroide *prox;
} Asteroide;

/* Inicializa o gerador aleatório (uma vez) e define a lista como vazia */
void asteroides_inicializar(Asteroide **lista);

/* Cria e insere um asteroide em posição aleatória na linha de surgimento */
int asteroides_criar(Asteroide **lista, int limite_esq, int limite_dir, int y_inicial);

/* Move asteroides, contabiliza desviados e pontos com multiplicador */
void asteroides_mover_e_remover_fora(Asteroide **lista, int limite_inferior, int *desviados, int *pontos,
                                     int multiplicador);

/* Verifica colisão com a nave, remove o asteroide colidido e libera memória */
int asteroides_verificar_colisao(Asteroide **lista, int nave_x, int nave_y, int nave_largura);

/* Desenha todos os asteroides na tela */
void asteroides_desenhar(const Asteroide *lista);

/* Libera todos os asteroides e define a cabeça da lista como NULL */
void asteroides_limpar(Asteroide **lista);

#endif /* ASTEROIDES_H */