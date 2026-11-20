# DIÁRIO de Desenvolvimento — Cosmo Dash

Este arquivo registra atividades reais do projeto.

Toda entrada deve separar claramente:
- [Planejado]
- [Implementado]
- [Testado]

## 2026-11-19

- [Planejado]
  - Arquitetura inicial do Cosmo Dash definida (módulos previstos, responsabilidades, uso de listas encadeadas para asteroides e itens, estados MENU, PARTIDA e GAME_OVER, persistência de recorde em arquivo texto e etapas de implementação).

- [Implementado]
  - O ambiente Ubuntu/WSL foi configurado.
  - GCC, Make e Git foram instalados e verificados.
  - A CLI-LIB foi baixada em /home/flepersons/Projetos/cli-lib.
  - O exemplo da CLI-LIB foi compilado e executado com sucesso.
  - A pasta /home/flepersons/Projetos/cosmo-dash foi criada.
  - A API real da CLI-LIB foi analisada.
  - A arquitetura inicial do Cosmo Dash foi planejada.
  - Naquele momento, nenhum arquivo de código do jogo havia sido criado ainda.

- [Testado]
  - O exemplo da CLI-LIB foi compilado e executado com sucesso (conforme registro das atividades realizadas).
  - Nenhum recurso do jogo foi implementado ou testado ainda.

## 2026-11-19 — Regras do projeto e programa mínimo

- [Planejado]
  - Criar um programa mínimo para validar inicialização e encerramento da CLI-LIB (tela, teclado e timer), sem implementar menu, partida, asteroides, itens, colisão ou recorde.

- [Implementado]
  - Criação do arquivo AGENTS.md na raiz do projeto com regras persistentes do Cosmo Dash.
  - Criação do arquivo src/main.c com programa mínimo funcional: inicializa tela com bordas (screenInit(1)), teclado (keyboardInit()) e timer (timerInit(50)); exibe mensagem de teste; aguarda tecla com keyhit() e readch(); encerra com keyboardDestroy(), screenDestroy() e timerDestroy().
  - Criação do arquivo src/jogo.h com guarda de inclusão e enum dos estados ESTADO_MENU, ESTADO_PARTIDA e ESTADO_GAME_OVER, com comentários indicando que a estrutura completa do jogo será definida nas próximas etapas.
  - Atualização deste DIÁRIO.md para registrar as ações realizadas.

- [Testado]
  - Compilação com `make` executada com sucesso a partir de /home/flepersons/Projetos/cosmo-dash (geração dos objetos e do executável build/cosmo-dash sem erros).
  - Execução com `make run` executada com sucesso em nova tentativa: o programa foi iniciado, exibiu "Cosmo Dash - teste inicial" e "Pressione qualquer tecla para sair...", e aguardou a leitura de uma tecla antes de encerrar e restaurar os recursos da CLI-LIB. Observação: uma tentativa anterior de `make run` falhou com "Text file busy" durante execução concorrente com `make`; não houve alteração de código ou do Makefile para essa correção, apenas nova execução sequencial.
  - O programa mínimo ainda não implementa menu, partida, asteroides, itens, colisão ou recorde.

## 2026-11-19 — Menu funcional e nave jogável

- [Planejado]
  - Implementar nave jogável com movimento horizontal, HUD mínimo e controle de tempo por timerTimeOver() na partida, mantendo o menu funcional.

- [Implementado]
  - Correção da frase histórica na primeira entrada do diário: substituição de "Nenhum arquivo de código do jogo foi criado ainda." por "Naquele momento, nenhum arquivo de código do jogo havia sido criado ainda.".
  - Implementação do módulo de jogo (src/jogo.h e src/jogo.c) com estado inicial em ESTADO_MENU, loop principal por estados e funções separadas para renderização/processamento do menu e da partida.
  - Menu funcional exibindo "COSMO DASH", opções "1 - Iniciar partida" e "2 - Sair", e instrução "Pressione 1 ou 2"; teclas diferentes de 1 e 2 são ignoradas.
  - Transição do menu para a partida (1), retorno da partida para o menu (M/m) e saída do programa pelo menu (2) e pela partida (Q/q).
  - Partida com nave jogável (src/nave.h e src/nave.c): nave fixa na parte inferior da área de jogo, movimento apenas horizontal com A/a (esquerda) e D/d (direita), limites respeitando SCRSTARTX/SCRENDX/SCRSTARTY/SCRENDY considerando a largura da nave, posição inicial aproximadamente centralizada e 3 vidas iniciais.
  - HUD mínimo na partida exibindo "Vidas: 3", "Pontos: 0" e "Tempo: 0", com incremento do tempo apenas quando timerTimeOver() retorna 1.
  - Atualização do src/main.c para inicializar recursos da CLI-LIB, executar o loop principal do jogo e garantir encerramento com keyboardDestroy(), screenDestroy() e timerDestroy().

- [Testado]
  - Compilação com make executada com sucesso em /home/flepersons/Projetos/cosmo-dash.
  - Execução automatizada do executável (./build/cosmo-dash, equivalente ao fluxo de make run após compilação) concluída com sucesso (encerramento limpo), exercitando a sequência: entrada na partida pelo menu (1), comandos de movimento da nave (A/D), retorno ao menu (M/m) e saída pelo menu (2).
  - Movimento da nave: comandos de movimentação horizontal foram processados durante a execução automatizada sem falhas.
  - Limites da nave: verificados pela lógica implementada (restrição aos limites SCRSTARTX/SCRENDX considerando a largura) e exercitados em execução automatizada; não houve validação visual interativa adicional registrada nesta etapa.
  - Encerramento: o programa finalizou corretamente na execução automatizada; a restauração do terminal é realizada pelas chamadas de destruição da CLI-LIB no encerramento do programa.
  - Asteroides, itens, colisão, pontuação real e recorde ainda não foram implementados.
## 2026-11-19 — Correções de apresentação/tempo, asteroides, colisão e GAME OVER

- [Planejado]
  - Corrigir flicker do menu e melhorar apresentação da partida (moldura fixa), ajustar contagem de tempo, implementar asteroides, colisão e tela de GAME OVER, mantendo simplicidade e sem itens/pontuação real/recorde.

- [Implementado]
  - Menu: renderização apenas ao entrar no estado ESTADO_MENU, sem redesenho contínuo no loop.
  - Partida: mapa fixo com moldura ASCII visível (+, -, |), HUD e linha de instruções reservadas; limpeza da área interna com espaços e redesenho dos objetos por quadro (evitando screenClear() a cada tick), com screenUpdate() ao final do desenho.
  - Tempo: contagem aproximada por ticks do timer de 50 ms (contador de ticks; incremento de 1 segundo a cada 20 ticks), exibição "Tempo: N s", reinício de ticks/tempo e da base do timer (timerUpdateTimer(50)) ao iniciar cada partida.
  - Asteroides (src/asteroides.h, src/asteroides.c): lista simplesmente encadeada (x, y, prox), cabeça em Jogo, criação periódica (a cada 10 ticks), movimento a cada 3 ticks, remoção de asteroides que saem da área contando como desviados, desenho com *, srand inicializado uma vez e tratamento de falha de malloc.
  - Colisão e vidas: verificação após movimento da nave e após atualização dos asteroides; remoção/liberação do asteroide colidido, desconto de uma vida (sem valores negativos) e transição para ESTADO_GAME_OVER ao chegar a 0 vidas.
  - GAME OVER: tela renderizada ao entrar no estado com pontuação 0, tempo sobrevivido, asteroides desviados, itens coletados 0 e "Recorde: ainda nao implementado"; opções M (menu) e Q (sair); reinício de partida restaura 3 vidas e zera contadores.
  - Memória/encerramento: liberação centralizada de todos os asteroides ao voltar ao menu, entrar em GAME OVER, reiniciar e sair; cabeça definida como NULL após liberar; keyboardDestroy(), screenDestroy() e timerDestroy() mantidos uma única vez em main.c.
  - Nave (src/nave.h, src/nave.c): limites ajustados para a área interna da moldura fixa.
  - README.md: atualizado com status atual, controles, requisitos de janela (mínimo 80 colunas, preferencialmente 26 linhas) e nota sobre contagem de tempo aproximada.

- [Testado]
  - Compilação com make executada com sucesso em /home/flepersons/Projetos/cosmo-dash (incluindo novos módulos de asteroides).
  - Testes visuais/interativos de teclado (menu sem flicker, moldura, movimento/limites em diferentes tamanhos de janela, tempo, geração/movimento de asteroides, colisão, GAME OVER, reinício e restauração de terminal) não foram executados neste ambiente; dependem de execução manual do usuário em terminal interativo adequado.
  - Pontuação real, itens coletáveis e recorde em arquivo texto permanecem não implementados.
## 2026-11-19 — Dificuldade progressiva, itens, pontuação, recorde e reinício

- [Planejado]
  - Implementar dificuldade progressiva por tempo, pontuação real, itens coletáveis (VIDA_EXTRA e PONTOS_BONUS) com listas encadeadas, tratamento de colisões simultâneas, persistência de recorde em arquivo texto e reinício rápido (R) no GAME OVER, preservando os módulos existentes e a simplicidade do código.

- [Implementado]
  - Módulos novos: `src/itens.h` e `src/itens.c` (lista simplesmente encadeada de itens, criação/movimento/remoção, coleta com efeitos +1 vida ou +50 pontos e incremento único de itens coletados, desenho `+` verde e `$` amarelo, limpeza com ponteiro para ponteiro e tratamento de falha de `malloc`); `src/recorde.h` e `src/recorde.c` (carregar/salvar inteiro em `recorde.txt`, arquivo ausente/inválido/negativo tratado como 0, verificação de abertura/leitura/escrita/fechamento, sem alocação dinâmica).
  - `src/asteroides.h` e `src/asteroides.c`: `asteroides_mover_e_remover_fora` passou a contabilizar pontos (+10 por asteroide desviado) além dos desviados.
  - `src/jogo.h`: inclusão de `itens.h` e novos campos em `Jogo` (lista de itens, pontos, itens coletados, recorde, flag de falha ao salvar, contadores de ticks para spawn/movimento de asteroides e itens).
  - `src/jogo.c`: integração completa — dificuldade progressiva por `tempo_segundos` (0–14 s: spawn 8/move 3; 15–29 s: 6/2; >=30 s: 4/1) com timer base 50 ms e tempo aproximado (20 ticks/s); itens criados a cada 100 ticks e movidos a cada 3 ticks (independente da dificuldade); ordem de processamento com colisões de asteroides antes da coleta de itens e sem coleta após GAME OVER; colisões simultâneas tratadas em laço (remoção de todos os asteroides sobrepostos antes do próximo movimento dos obstáculos), sem vidas negativas e com finalização imediata ao chegar a 0; HUD e telas atualizados (pontuação/itens/recorde reais, legenda curta `+Vida $Pts`, aviso quando não foi possível salvar o recorde); GAME OVER com `R` reiniciar direto, `M` menu e `Q` sair; `jogo_iniciar_partida` reutilizado para início/reinício (limpa listas, zera contadores/partida, restaura 3 vidas, reinicia timer, preserva recorde); menu preservado (`1` inicia, `2` sai); inicialização dos ponteiros de listas como `NULL` e limpeza centralizada ao abandonar/reiniciar/terminar/sair; carregamento do recorde ao iniciar e salvamento apenas ao superar recorde (com verificação de retorno).
  - `README.md`: atualizado com controles, legenda/valores de itens e pontuação, dificuldade progressiva, funcionamento do recorde e status real das funcionalidades.

- [Testado]
  - Compilação com `make` executada com sucesso via WSL em `/home/flepersons/Projetos/cosmo-dash` (sem erros), gerando `build/cosmo-dash` com os módulos do projeto e da CLI-LIB.
  - Não foram realizados testes visuais/interativos em terminal real pelo agente (renderização, teclado, restauração de terminal, progressão de dificuldade percebida, coletas/colisões em jogo e persistência de `recorde.txt` em cenário real dependem de execução manual do usuário). Nenhuma execução interativa foi deixada bloqueada na ferramenta e `recorde.txt` real não foi sobrescrito para teste.
## 2026-11-19 — Balanceamento suave, itens mais lentos e coleta imediata

- [Planejado]
  - Substituir as faixas fixas de dificuldade por acumuladores inteiros com sobra preservada, diminuir a velocidade dos itens e verificar coleta imediatamente após o movimento da nave, mantendo controles e demais funcionalidades.

- [Implementado]
  - `src/jogo.h`: campos `tick_spawn_ast` e `tick_move_ast` renomeados para `acum_spawn_ast` e `acum_move_ast`, com comentário indicando que são acumuladores inteiros.
  - `src/jogo.c`: removidas as faixas 0–14 s, 15–29 s e >=30 s; movimento de asteroides passou a usar `taxa_movimento = 30 + tempo_segundos / 3` (limite 55) e criação passou a usar `taxa_criacao = 10 + tempo_segundos / 5` (limite 20), ambos somados a acumuladores independentes que movem/criam ao atingir 100 e subtraem 100 preservando a sobra. Timer base de 50 ms e contagem de 20 ticks por segundo mantidos.
  - `src/jogo.c`: movimento dos asteroides, colisões e retorno imediato em GAME OVER foram mantidos antes da criação de asteroides e do processamento de itens; ambos os acumuladores são zerados ao iniciar e reiniciar a partida.
  - `src/jogo.c`: `TICKS_MOVE_ITEM` alterado de 3 para 5; criação de itens permanece a cada 100 ticks; efeitos e pontuações não foram alterados.
  - `src/jogo.c`: função estática curta `jogo_coletar_itens` centraliza a chamada de coleta; após A/D a ordem é colisões com asteroides, coleta (se ainda em PARTIDA) e redesenho consistente; a verificação de coleta durante a atualização de ticks foi mantida. O item coletado já é removido pelo módulo de itens, evitando pontuação duplicada; nenhuma coleta ocorre após GAME OVER.
  - Controles inalterados: menu `1`/`2`, partida `A/D`, `M`, `Q`, GAME OVER `R/M/Q`. Não foi adicionada simulação de tecla segurada.
  - `README.md`: dificuldade, velocidade dos itens, coleta imediata e observação sobre o atraso de repetição do teclado documentados.

- [Testado]
  - Relatos manuais do usuário (não executados pelo agente nesta etapa): itens de vida e pontos funcionando; recorde preservado após fechar e abrir o jogo; pontuação inferior não substitui recorde maior; controles funcionando; dificuldade anterior apresentava salto excessivo aos 30 segundos.
  - Compilação com `make` executada com sucesso via WSL em `/home/flepersons/Projetos/cosmo-dash` após as alterações, gerando `build/cosmo-dash` sem erros.
  - Não foi realizada validação visual/interativa do novo balanceamento, da coleta imediata na prática ou da restauração de terminal nesta etapa. O atraso de repetição do teclado permanece como limitação a avaliar, não como correção declarada. Não foi realizada revisão completa de memória.

## 2026-11-19 — Limite de vidas, mais asteroides e itens sobrepostos

- [Planejado]
   - Proposta de balanceamento para testar: teto de vidas, criação de asteroides mais frequente ao longo da partida e coleta repetida de itens sobrepostos. Os valores são proposta deste bloco, não aprovação manual do usuário.
   - Manter timer de 50 ms, 20 ticks/s, acumuladores com sobra, velocidade dos itens, controles, recorde e ausência de BONUS TIME/matriz.

- [Implementado]
   - `src/nave.h`: `NAVE_VIDAS_MAXIMAS` definido como 5. `src/nave.c` não foi alterado, preservando as 3 vidas iniciais.
   - `src/itens.c`: `VIDA_EXTRA` usa a constante e só aumenta vidas abaixo do teto. No máximo, o item é removido e contabilizado, sem vida ou pontos extras.
   - `src/jogo.c`: HUD mostra `Vidas:atual/5` usando a constante; `taxa_criacao = 10 + tempo_segundos / 3`, limitada a 40, com criação ao atingir 100 e sobra preservada. `taxa_movimento` e o timer permanecem como antes.
   - `src/jogo.c`: `jogo_coletar_itens` repete `itens_verificar_coleta` enquanto houver coleta em PARTIDA. Colisões com asteroides continuam antes das coletas, sem coleta após GAME OVER e com remoção/contagem única por item.
   - Não foram alterados Makefile, CLI-LIB, `recorde.txt` nem `src/nave.c`. Não foi adicionada simulação de tecla segurada; controle inalterado.

- [Testado]
   - Relato manual do usuário em testes anteriores (não executados pelo agente nesta etapa): a queda dos objetos melhorou; após partida longa a dificuldade ficou baixa; houve acúmulo de 11 vidas; controles, itens e recorde funcionaram segundo os testes anteriores.
   - `make -n clean` confirmou que a limpeza remove somente `build`. Em seguida, `make clean` e `make` foram executados sequencialmente via WSL, com compilação limpa e geração de `build/cosmo-dash` sem erros.
   - O novo balanceamento ainda não foi testado manualmente. Nenhuma execução interativa foi deixada presa na ferramenta. Seguem pendentes a validação visual do HUD, da coleta de itens sobrepostos e da progressão até 90 segundos; o atraso de repetição do teclado não foi corrigido nem declarado como corrigido.

## 2026-11-19 — Nova taxa de criação de asteroides

- [Planejado]
   - Ajustar apenas a criação de asteroides para tentar elevar a dificuldade, mantendo as demais regras da partida.

- [Implementado]
   - `src/jogo.c`: `taxa_criacao = 15 + tempo_segundos / 2`, limitada a 70. Acumulador com limiar 100 e sobra preservada mantido.
   - Movimento dos asteroides, timer de 50 ms, itens, pontuação, limite de vidas, controles e arena inalterados.
   - `README.md`: fórmula e limite atualizados. Makefile, CLI-LIB e `recorde.txt` não alterados; sem BONUS TIME, rajadas ou novos obstáculos.

- [Testado]
   - Relato do usuário: o bloco anterior passou nos testes manuais solicitados, mas a dificuldade ainda é considerada baixa. A captura enviada mostra uma partida isolada com 301 segundos, não uma duração média de várias partidas.
   - A nova frequência é uma proposta ainda não validada manualmente; nenhuma execução interativa foi feita ou deixada presa na ferramenta.
   - `make` executado via WSL após as alterações, com compilação sem erros e geração de `build/cosmo-dash`.

## 2026-11-19 — Redesenho local após A/D

- [Planejado]
   - Reduzir o trabalho de desenho por entrada A/D, mantendo movimento de uma coluna, colisões, coletas e o quadro completo no tick.

- [Implementado]
   - `src/jogo.c`: função estática curta `jogo_mover_nave` usada por A/D. Ela apaga a nave, move uma coluna respeitando os limites, processa colisões, processa coleta se ainda em PARTIDA e desenha apenas nave e HUD, com um `screenUpdate()`.
   - `renderizar_partida()` não é mais chamada por A/D; continua no tick de 50 ms e nas entradas de estado. Se a colisão causar GAME OVER, não há desenho de nave/HUD nessa entrada.
   - Asteroides e itens removidos por colisão/coleta passam a ser limpos no próximo quadro do tick.
   - Entrada inalterada: um caractere por volta, sem descartar ou coalescer repetições, sem timeout, sem direção persistente e sem mudança em M/Q. Dificuldade, itens, pontuação, vidas, recorde e arena preservados.
   - `README.md` descreve a redução de redesenho; sem BONUS TIME ou matriz. Makefile, CLI-LIB e `recorde.txt` não alterados.

- [Testado]
   - `make` executado via WSL após as alterações, com compilação sem erros.
   - Não houve teste visual ou medição de desempenho nesta etapa. A melhora percebida depende de teste manual do usuário. O atraso de repetição do teclado não foi corrigido, não foi medido e não foi atribuído a um gargalo medido.

## 2026-11-19 — Matrizes na renderização da arena

- [Planejado]
   - Usar matrizes estáticas para representar a área interna no quadro completo, mantendo as listas encadeadas como fonte dos objetos.

- [Implementado]
   - `src/jogo.c`: macros `ARENA_LINHAS` e `ARENA_COLUNAS` derivadas dos limites internos; matrizes estáticas `arena` (`char`) e `arena_cores` (`screenColor`).
   - Funções estáticas: `arena_limpar()`, `arena_marcar()` (com verificação de limites), `arena_montar()` (asteroides, itens e nave, nessa ordem) e `arena_exibir()` (uma linha por vez, cores da matriz e `screenUpdate()` no quadro completo).
   - `renderizar_partida()` passou a usar as matrizes; a limpeza por espaços e as chamadas diretas de desenho dos objetos no quadro completo foram removidas. O desenho incremental de nave/HUD após A/D foi preservado.
   - A nave usa localmente `^` e `GREEN`, mesma aparência anterior. Módulos nave, itens e asteroides não foram alterados; funções públicas desses módulos permanecem.
   - `README.md` descreve as matrizes; sem BONUS TIME e sem mudanças em dificuldade, timer, recorde, arena ou controles. Makefile, CLI-LIB e `recorde.txt` não alterados.

- [Testado]
   - `make` executado via WSL após as alterações, com compilação sem erros.
   - Não houve validação visual pelo agente: cores, sobreposições e ausência de resíduos dependem de teste manual do usuário. A limitação de repetição de A/D continua aceita e não foi tratada neste bloco.

## 2026-11-19 - BONUS TIME

- [Planejado]
   - Implementar BONUS TIME com ativação por 400 ticks sem dano, duração de 200 ticks, multiplicador apenas em pontos novos, aviso piscando três vezes e indicador x2 no HUD.
   - Manter timer, dificuldade, controles, dimensões, velocidades, recorde e a renderização por matrizes já existente.

- [Implementado]
   - src/jogo.h: campos inteiros ticks_sem_dano, ticks_bonus_restantes e ticks_aviso.
   - src/jogo.c: constantes derivadas dos 20 ticks/s (400 ticks para ativar, 200 de duração, 30 de aviso em seis fases de 5 ticks) e funções estáticas jogo_cancelar_bonus(), jogo_multiplicador_pontos(), jogo_atualizar_bonus() e desenhar_aviso().
   - A atualização temporal ocorre uma vez no início de cada tick; a ativação não consome um tick da duração recém-iniciada. A sequência fica zerada durante o bônus e recomeça apenas no tick seguinte ao fim dele. Contadores nunca ficam negativos.
   - Colisões cancelam bônus e aviso no momento da perda de vida, inclusive em A/D entre ticks, antes da coleta de itens. Início/reinício, M, Q e GAME OVER também encerram o bônus.
   - src/asteroides.h e src/asteroides.c, src/itens.h e src/itens.c: funções que concedem pontos recebem multiplicador; bases 10 e 50 são multiplicadas uma única vez nos próprios módulos. VIDA_EXTRA e a pontuação acumulada não são afetadas; coleta única, teto de vidas e remoção de nós preservados.
   - desenhar_hud() mostra x2 ao lado da pontuação e Bonus:Ns (arredondado para cima), com campos de largura fixa que apagam resíduos quando o bônus termina. Vidas, tempo e desviados foram preservados.
   - Aviso exato >>> BONUS TIME!!! <<<, centralizado na linha 1 (SCRSTARTY), acima do HUD e da moldura, em YELLOW. Três ciclos de 5 ticks visíveis e 5 apagados; ao apagar, toda a largura reservada é preenchida com espaços. O estado visual vem dos contadores de ticks, sem sleep, espera ocupada ou screenClear() para piscar.
   - desenhar_aviso() e o HUD são chamados tanto no quadro completo quanto no redesenho local após A/D, sem duplicar a lógica de temporização. Matrizes, listas e desenho incremental continuam presentes.
   - README.md documenta regras, multiplicador, ativação, cancelamento, duração e status. Não foram alterados Makefile, CLI-LIB, nave, recorde.txt, dificuldade, controles, dimensões ou velocidades.

- [Testado]
   - make -n clean confirmou que a limpeza remove somente build (rm -rf build). Em seguida, make clean e make foram executados sequencialmente via WSL, com compilação limpa (-Wall -Werror) e geração de build/cosmo-dash sem erros.
   - As assinaturas alteradas em asteroides e itens tiveram todas as chamadas atualizadas; a compilação confirmou a consistência das interfaces.
   - Não houve teste visual/interativo pelo agente: piscadas, cor, centralização, sobreposições e resíduos do HUD/aviso dependem de validação manual do usuário em terminal real. Nenhum jogo interativo foi deixado preso na ferramenta, o recorde.txt real não foi alterado para teste e a limitação de repetição de A/D não foi corrigida nem declarada como corrigida.

## 2026-11-19 - Correções finais autorizadas

- [Planejado]
   - Atualizar README e DIÁRIO com os relatos manuais reais do usuário, sem reescrever entradas históricas nem declarar validação completa.
   - Substituir `fscanf("%d")` em `recorde_carregar()` por validação com `fgets` e `strtol`.
   - Permitir configurar a raiz da CLI-LIB no Makefile por `CLI_LIB_DIR` e documentar o uso.

- [Implementado]
   - src/recorde.c: leitura em blocos de 64 bytes, ignora espaços antes/depois do número (inclusive quebras de linha), usa `strtol` com `errno`/`ERANGE`, exige `0..INT_MAX`, rejeita conteúdo extra não branco, vazio, inválido, negativo, overflow, erro de leitura e falha de fechamento. `fclose` continua chamado uma única vez após a abertura; `recorde_salvar()` não foi alterado.
   - Makefile: `CLI_LIB_DIR ?= ../cli-lib`; `-I$(CLI_LIB_DIR)/include` e `$(CLI_LIB_DIR)/src` derivados dessa variável. Flags, alvos, objetos, executável e `clean` preservados; `main.c` da CLI-LIB continua fora da compilação.
   - README.md: evidências separadas entre relatos manuais, compilação/testes automatizados e verificações pendentes; documentação de recorde, dependências, pastas irmãs e configuração de `CLI_LIB_DIR`.

- [Testado]
   - Relatos manuais do usuário recebidos nesta etapa (não executados pelo agente): menu deixou de piscar; controles e reinício rápido funcionaram; itens de vida e pontos funcionaram; recorde permaneceu após fechar e abrir o jogo; pontuação inferior não substituiu recorde maior; bloco de cinco vidas e coleta passou nos testes solicitados; quantidade de asteroides aumentou gradualmente e exigiu reposicionamento; após matrizes, aparência relatada como semelhante; BONUS TIME testado e relatado como funcionando; redução do redesenho não resolveu a pausa ao segurar/inverter A/D, limitação aceita.
   - Compilação executada via WSL, em comandos sequenciais:
      - `make -n clean` → `rm -rf build`, confirmando que `clean` remove somente `build`.
      - `make clean` → `rm -rf build`.
      - `make` → recompilação completa com `-Wall -Werror -g -Isrc -I../cli-lib/include`, seis objetos do projeto, três objetos da CLI-LIB e linkagem em `build/cosmo-dash`, sem erros.
      - `make clean` → `rm -rf build`.
      - `make CLI_LIB_DIR=/home/flepersons/Projetos/cli-lib` → recompilação completa com `-I/home/flepersons/Projetos/cli-lib/include` e fontes absolutos da CLI-LIB, sem erros. Não houve resultado `Nothing to be done for 'all'` nas recompilações acima.
   - Testes automatizados de `recorde_carregar()` executados via WSL, sem inicializar o jogo:
      - Programa temporário criado em `build/teste_recorde.c`, compilado com `gcc -Wall -Werror -g -Isrc build/teste_recorde.c src/recorde.c -o build/teste_recorde` e executado com `./build/teste_recorde`.
      - Resultado: 13 casos, 0 falhas — arquivo ausente=0; vazio=0; `0`=0; `123`=123; espaços e quebras em torno de `123`=123; `-1`=0; `abc`=0; `123abc`=0; `123 456`=0; `12` seguido de quebra de linha e `34`=0; valor maior que `INT_MAX`=0; `INT_MAX`=2147483647; entrada longa com prefixo numérico=0.
      - Os arquivos de entrada temporários foram criados e removidos em `build/`. `build/teste_recorde.c` e `build/teste_recorde` foram removidos ao final; `build/cosmo-dash` e `build/obj` foram preservados. `recorde.txt` real não foi aberto para escrita.
   - Não executado nesta etapa: jogo interativo/visual pelo agente, confirmação individual de cada cenário do BONUS TIME, simulação de erro de leitura/falha de `fclose` e de caminho `NULL` no programa de teste, Valgrind/ASan, janelas com menos de 80 colunas e pontuações extremas. Nenhuma ausência comprovada de vazamentos foi afirmada.
