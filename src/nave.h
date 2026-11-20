#ifndef NAVE_H
#define NAVE_H


#define NAVE_VIDAS_MAXIMAS 5


typedef struct {
    int x;
    int y;
    int largura;
    int vidas;
} Nave;


void nave_inicializar(Nave *nave);


void nave_mover_esquerda(Nave *nave);


void nave_mover_direita(Nave *nave);


void nave_desenhar(const Nave *nave);


void nave_apagar(const Nave *nave);

#endif 
