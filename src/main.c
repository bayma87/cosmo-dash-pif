#include "jogo.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

/*
 * Ponto de entrada do Cosmo Dash.
 * Inicializa recursos da CLI-LIB, executa o jogo e garante a restauração
 * do teclado e da tela ao encerrar.
 */
int main(void) {
    Jogo jogo;

    /* Inicializa tela, teclado e timer */
    screenInit(1);
    keyboardInit();
    timerInit(50);

    /* Inicializa e executa o loop principal do jogo */
    jogo_inicializar(&jogo);
    jogo_executar(&jogo);

    /* Encerra recursos na ordem definida */
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}