#ifndef RECORDE_H
#define RECORDE_H

/* Carrega o recorde de um arquivo texto; retorna 0 se ausente/inválido/negativo */
int recorde_carregar(const char *caminho);

/* Salva o recorde em arquivo texto; retorna 1 em sucesso e 0 em falha */
int recorde_salvar(const char *caminho, int valor);

#endif /* RECORDE_H */