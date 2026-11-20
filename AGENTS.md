# Regras do projeto Cosmo Dash

## Escopo
- Cosmo Dash é um jogo de terminal em C para desviar de asteroides.
- É um projeto universitário.
- O jogo deve ser executado no Ubuntu via WSL.
- Não usar bibliotecas gráficas externas.
- Usar apenas a CLI-LIB como dependência externa.

## Estados do jogo
- O jogo deve ter os estados MENU, PARTIDA e GAME_OVER.
- Não deve existir estado de pausa.

## Funcionalidades planejadas
- Nave controlada apenas horizontalmente.
- A nave deve permanecer dentro da área visível do mapa.
- Asteroides descem pela tela.
- Itens VIDA_EXTRA e PONTOS_BONUS.
- Colisão com asteroide encerra a partida quando não há vidas restantes.
- GAME_OVER deve exibir pontuação, tempo sobrevivido, asteroides desviados, itens coletados e recorde.
- O recorde deve ser salvo em arquivo texto.

## Estrutura do projeto
- src/ contém os arquivos .c e os respectivos .h.
- docs/ contém documentação.
- O Makefile está na raiz.
- O executável é gerado em build/cosmo-dash.
- Não copiar nem modificar a CLI-LIB.

## CLI-LIB
- A CLI-LIB está em /home/flepersons/Projetos/cli-lib.
- Usar apenas as funções e tipos declarados em:
  - include/keyboard.h
  - include/screen.h
  - include/timer.h
- Não inventar funções, constantes, estruturas ou comportamentos.
- Antes de usar qualquer função, confirmar sua existência nos cabeçalhos ou no código-fonte da biblioteca.
- Não compilar nem incluir o arquivo src/main.c da CLI-LIB no projeto Cosmo Dash.
- O Makefile já compila keyboard.c, screen.c e timer.c da CLI-LIB.

## Regras de código
- Escrever código em C.
- Usar comentários em português.
- Priorizar legibilidade e tratamento seguro de memória.
- Usar listas encadeadas para asteroides e itens.
- Na remoção de nós, usar ponteiro para ponteiro e liberar a memória corretamente.
- Evitar vazamentos de memória, ponteiros inválidos e dupla liberação.
- Sempre restaurar teclado e tela ao encerrar o programa.

## Regras de documentação
- Diferenciar claramente:
  - [Planejado]
  - [Implementado]
  - [Testado]
- Não atribuir contribuições inexistentes ao grupo.
- Não marcar funcionalidades como implementadas ou testadas sem evidência real.
- Atualizar DIÁRIO.md somente após ações realmente executadas.

## Comandos
- make: compila o projeto.
- make run: compila, se necessário, e executa o jogo.
- make clean: remove apenas a pasta build do projeto.