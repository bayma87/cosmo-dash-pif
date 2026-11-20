#ifndef NAVE_H
#define NAVE_H

/* Teto de vidas da nave (vidas iniciais continuam em nave.c) */
#define NAVE_VIDAS_MAXIMAS 5

/* Representa a nave do jogador */
typedef struct {
    int x;
    int y;
    int largura;
    int vidas;
} Nave;

/* Inicializa a nave na posição inicial e com vidas padrão */
void nave_inicializar(Nave *nave);

/* Move a nave uma posição para a esquerda, respeitando os limites internos */
void nave_mover_esquerda(Nave *nave);

/* Move a nave uma posição para a direita, respeitando os limites internos */
void nave_mover_direita(Nave *nave);

/* Desenha a nave na tela */
void nave_desenhar(const Nave *nave);

/* Apaga a nave da posição atual (desenha espaços) */
void nave_apagar(const Nave *nave);

#endif /* NAVE_H */