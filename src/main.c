#include "jogo.h"
#include "keyboard.h"
#include "screen.h"
#include "timer.h"


int main(void) {
    Jogo jogo;

    
    screenInit(1);
    keyboardInit();
    timerInit(50);

    
    jogo_inicializar(&jogo);
    jogo_executar(&jogo);

    
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
