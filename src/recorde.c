#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "recorde.h"

#define TAM_LINHA 64

int recorde_carregar(const char *caminho) {
    if (caminho == NULL) {
        return 0;
    }

    FILE *arquivo = fopen(caminho, "r");
    if (arquivo == NULL) {
        /* Arquivo ausente: recorde inicial 0 */
        return 0;
    }

    char linha[TAM_LINHA];
    int valor = 0;
    int encontrado = 0;
    int invalido = 0;

    while (fgets(linha, sizeof linha, arquivo) != NULL) {
        char *ponteiro = linha;

        while (*ponteiro != '\0' && isspace((unsigned char)*ponteiro)) {
            ponteiro++;
        }

        if (!encontrado) {
            if (*ponteiro == '\0') {
                continue; /* Linha em branco antes do numero */
            }

            errno = 0;
            char *fim = NULL;
            long numero = strtol(ponteiro, &fim, 10);

            if (errno == ERANGE || fim == ponteiro || numero < 0 || numero > INT_MAX) {
                invalido = 1;
                break;
            }

            valor = (int)numero;
            encontrado = 1;
            ponteiro = fim;
        }

        while (*ponteiro != '\0' && isspace((unsigned char)*ponteiro)) {
            ponteiro++;
        }

        if (*ponteiro != '\0') {
            invalido = 1; /* Nao aceita prefixo nem conteudo extra apos o numero */
            break;
        }
    }

    if (ferror(arquivo)) {
        invalido = 1;
    }

    int fechado = fclose(arquivo);
    if (fechado != 0) {
        invalido = 1;
    }

    if (invalido || !encontrado) {
        return 0;
    }

    return valor;
}

int recorde_salvar(const char *caminho, int valor) {
    if (caminho == NULL) {
        return 0;
    }

    FILE *arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        return 0;
    }

    int escritos = fprintf(arquivo, "%d\n", valor);
    int fechado = fclose(arquivo);

    if (escritos < 0 || fechado != 0) {
        return 0;
    }

    return 1;
}