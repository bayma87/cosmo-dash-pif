# Cosmo Dash

**Status:** em desenvolvimento (projeto universitário).

O Cosmo Dash é um jogo de terminal em C para desviar de asteroides, desenvolvido como projeto universitário para execução no Ubuntu via WSL.

## Visão geral

- Jogo executado no terminal Linux/Ubuntu via WSL.
- O jogador controla uma nave que se move apenas horizontalmente dentro de uma área fixa.
- Asteroides descem pela tela e devem ser desviados.
- Itens coletáveis: **VIDA_EXTRA** (`+`, verde, +1 vida até o limite) e **PONTOS_BONUS** (`$`, amarelo, +50 pontos).
- Colisão com asteroide remove o asteroide e desconta 1 vida; sem vidas restantes a partida termina.
- Dificuldade progressiva conforme o tempo sobrevivido.
- **BONUS TIME:** após 20 segundos consecutivos sem dano, novos pontos valem o dobro por cerca de 10 segundos.
- A tela de **GAME OVER** exibe pontuação, tempo sobrevivido, asteroides desviados, itens coletados e recorde.
- O recorde é salvo em arquivo texto (`recorde.txt`, na raiz do jogo).
- Estados do jogo: MENU, PARTIDA e GAME_OVER.
- Não há estado de pausa.

## Controles

- **Menu:** `1` iniciar partida, `2` sair.
- **Partida:** `A`/`D` (ou `a`/`d`) mover nave, `M`/`m` voltar ao menu, `Q`/`q` sair.
- **Game Over:** `R`/`r` reiniciar partida, `M`/`m` voltar ao menu, `Q`/`q` sair.
- A repetição de `A`/`D` depende do atraso de repetição do teclado; o jogo não simula tecla segurada. Segurar/inverter `A`/`D` pode causar pausa perceptível; limitação aceita nesta versão.

## Pontuação e itens

- Asteroide desviado: **+10 pontos** (**+20** durante o BONUS TIME).
- Asteroide colidido: **não pontua**.
- Item **PONTOS_BONUS** (`$`, amarelo): **+50 pontos** (**+100** durante o BONUS TIME).
- Item **VIDA_EXTRA** (`+`, verde): **+1 vida**, até `NAVE_VIDAS_MAXIMAS` (5). No limite, o item é removido e contabilizado, sem vida ou pontos extras.
- Cada item coletado incrementa o contador de itens coletados uma única vez; itens que saem da área não contam como coletados.
- Coleta baseada na sobreposição horizontal da nave (mesma lógica de posição usada nas colisões).
- O multiplicador do BONUS TIME incide apenas sobre desvios e itens de pontos obtidos enquanto o bônus está ativo: a pontuação já acumulada e o efeito de VIDA_EXTRA não são multiplicados.

## Dificuldade progressiva

O timer base permanece em 50 ms (tempo aproximado: 20 ticks por segundo). A dificuldade usa acumuladores inteiros independentes, com sobra preservada:

- **Movimento dos asteroides:** `taxa_movimento = 30 + tempo_segundos / 3`, limitada a 55; ao atingir 100, os asteroides descem uma linha e a sobra é mantida.
- **Criação de asteroides:** `taxa_criacao = 15 + tempo_segundos / 2`, limitada a 70; ao atingir 100, um asteroide é criado e a sobra é mantida. O limite é alcançado por volta de 110 segundos.

Isso substitui as faixas fixas anteriores e suaviza o salto de dificuldade.

Itens: criação a cada 100 ticks (~5 segundos) e movimento a cada 5 ticks, independente da dificuldade.

A coleta de itens é verificada imediatamente após o movimento da nave, depois das colisões com asteroides, e se repete enquanto houver itens sobrepostos na partida ativa.

A cada A/D, apenas a nave, o HUD e o aviso são redesenhados, com um `screenUpdate()`; o quadro completo continua no tick de 50 ms. Isso reduz o trabalho de desenho por entrada e não altera o atraso de repetição do teclado.

## BONUS TIME

- Após **400 ticks** de 50 ms (20 segundos) consecutivos sem dano, o bônus é ativado.
- Duração: **200 ticks** (cerca de 10 segundos). Desvios passam a valer +20 e itens `PONTOS_BONUS`, +100; a pontuação anterior não é multiplicada.
- O HUD passa a mostrar `x2` imediatamente após a pontuação e `Bonus:Ns`, com os segundos restantes arredondados para cima. Fora do bônus, esses campos são preenchidos com espaços para remover resíduos.
- O aviso `>>> BONUS TIME!!! <<<` aparece em amarelo na linha 1, acima do HUD e da moldura. São três ciclos de 5 ticks visível e 5 ticks apagado (30 ticks). Depois disso o aviso some, mas o bônus continua até terminar.
- Qualquer perda de vida zera a sequência sem dano e cancela bônus e aviso na hora. Após o término normal, são necessários outros 400 ticks sem dano; o tempo de bônus não conta para a próxima ativação.
- Início/reinício de partida, retorno ao menu, saída e GAME OVER também encerram bônus e aviso. O recorde continua sendo calculado com a pontuação final real.
- Os contadores avançam apenas uma vez por tick da partida; redesenhar por A/D não acelera a animação.

## Renderização da arena

`src/jogo.c` mantém duas matrizes estáticas dimensionadas pelos limites internos: `arena` (caracteres) e `arena_cores` (`screenColor`). A cada quadro completo, `arena_montar()` limpa as matrizes e reconstrói a área interna a partir das listas de asteroides/itens e da nave; `arena_exibir()` imprime cada linha com as cores correspondentes.

Moldura, HUD e instruções continuam desenhados separadamente, e o desenho incremental após A/D permanece. As listas encadeadas continuam armazenando os objetos e sendo usadas para movimento, colisão e coleta; as matrizes são apenas a representação usada na saída.

## Recorde

- Arquivo: `recorde.txt` na raiz do jogo (um número inteiro não negativo).
- A leitura usa `fgets` e `strtol`, percorre o arquivo inteiro e aceita espaços em branco antes e depois do número (inclusive quebras de linha).
- Um valor é aceito apenas se estiver em `0..INT_MAX`; qualquer conteúdo adicional não branco, texto inválido, número negativo, overflow, erro de leitura ou falha de fechamento resulta em recorde inicial **0**.
- Caminho `NULL`, arquivo ausente e arquivo vazio também resultam em **0**.
- Limite da solução: o número precisa caber em `INT_MAX` e a leitura é feita em blocos de 64 bytes. Blocos longos de espaços são percorridos, mas um prefixo numérico seguido de outros caracteres não é aceito.
- Ao entrar em GAME OVER, se a pontuação for maior que o recorde, o valor é atualizado em memória e o programa tenta salvar no arquivo (sem gravar a cada quadro).
- Em falha de gravação, o jogo informa que não foi possível salvar o recorde (não afirma persistência).
- O arquivo de recorde não é apagado ao reiniciar a partida.

## Status do projeto

| Funcionalidade | Status | Observações |
|---|---|---|
| Execução no terminal Linux/Ubuntu via WSL | Implementado | Programa de terminal em C usando CLI-LIB. |
| Menu funcional | Implementado | `1` iniciar partida, `2` sair. |
| Controle da nave (apenas horizontal) | Implementado | `A/D` (ou `a/d`), nave fixa na parte inferior da área interna. |
| Limites da área de jogo (mapa fixo) | Implementado | Moldura ASCII fixa e limites internos respeitados. |
| HUD e tempo aproximado | Implementado | `Vidas:3/5`, pontos, tempo e asteroides desviados; legenda curta dos itens. |
| Asteroides em queda | Implementado | Lista encadeada, geração/movimento com dificuldade progressiva. |
| Colisão nave-asteroide e perda de vidas | Implementado | Remove asteroides colididos (inclusive sobrepostos) e desconta vidas; sem vidas leva a GAME OVER. |
| Tela de GAME_OVER | Implementado | Exibe pontuação, tempo, desviados, itens coletados e recorde; `R/M/Q`. |
| Reinício rápido (R) | Implementado | Reinicia diretamente a partida preservando o recorde. |
| Item VIDA_EXTRA (`+`, verde) | Implementado | +1 vida até 5; no teto, remove e contabiliza sem efeito; lista encadeada; criação/movimento periódicos. |
| Item PONTOS_BONUS (`$`, amarelo) | Implementado | +50 pontos; lista encadeada; criação/movimento periódicos. |
| Pontuação real | Implementado | +10 por desvio, +50 por bônus; +20/+100 no BONUS TIME. |
| BONUS TIME (x2) | Implementado | 20 s sem dano, 10 s de duração, aviso de 3 piscadas e HUD com `x2`; testado e relatado como funcionando pelo usuário, sem confirmação individual de cada cenário pelo agente. |
| Persistência de recorde em arquivo texto | Implementado | `recorde.txt`, carga validada ao iniciar e gravação ao superar recorde. |
| Testes visuais/interativos em terminal real | Parcialmente testado | Relatos manuais cobrem menu, controles, itens, recorde, limite de vidas, dificuldade e aparência após matrizes; agente não executou jogo interativo nem validou todos os casos de borda. |

## Evidências de validação

### Relatos manuais do usuário

- Menu deixou de piscar.
- Controles e reinício rápido funcionaram.
- Itens de vida e pontos funcionaram.
- Recorde permaneceu após fechar e abrir o jogo.
- Pontuação inferior não substituiu o recorde maior.
- O bloco de limite de cinco vidas e coleta passou nos testes solicitados.
- A quantidade de asteroides aumentou gradualmente, exigindo reposicionamento.
- Após a mudança para matrizes, a aparência foi relatada como semelhante; isso não comprova todos os casos de borda ou sobreposição.
- BONUS TIME foi testado e relatado como funcionando; não houve confirmação individual de cada cenário do bônus.
- A redução do redesenho não resolveu a pausa ao segurar/inverter `A`/`D`; essa limitação foi aceita para esta versão.

### Compilação e testes automatizados executados pelo agente

- `make -n clean`, `make clean`, `make` e `make CLI_LIB_DIR=/home/flepersons/Projetos/cli-lib` executados via WSL; detalhes em `DIÁRIO.md`.
- Testes automatizados de `recorde_carregar()` com arquivos temporários em `build/`; casos e resultados em `DIÁRIO.md`.

### Ainda não verificado

- Validação visual de todos os casos de borda, sobreposições, cores e resíduos do aviso/HUD.
- Cada cenário individual do BONUS TIME.
- Ausência de vazamentos por ferramenta especializada, comportamento em janelas menores que 80 colunas e pontuações extremas.

## Requisitos de janela/terminal

- O jogo utiliza **mapa fixo** baseado nas constantes da CLI-LIB (não há redimensionamento automático).
- A janela do terminal deve ter **pelo menos 80 colunas** e, **preferencialmente, 26 linhas** (ou mais) para exibir corretamente a moldura, o HUD e a área de jogo.
- Em janelas menores que o tamanho esperado, o desenho pode quebrar/encavalar linhas.
- O tempo exibido é uma **contagem aproximada** baseada em ticks do timer (50 ms).

## Dependências e caminho da CLI-LIB

- GCC e Make são necessários para compilar os fontes disponíveis.
- Git é necessário apenas para obter os repositórios por clone; não é necessário para compilar fontes já disponíveis.
- A CLI-LIB é uma dependência externa e não deve ser copiada nem modificada dentro deste projeto.
- A organização padrão é de pastas irmãs: `Projetos/cosmo-dash` e `Projetos/cli-lib`. O Makefile usa `CLI_LIB_DIR ?= ../cli-lib`.
- A API usada é `keyboard.h`, `screen.h` e `timer.h` (conforme cabeçalhos/código da biblioteca).

## Estrutura

- `src/` — arquivos `.c` e `.h`.
- `docs/` — documentação.
- `build/` — artefatos de compilação e executável.
- `Makefile` — compilação, execução e limpeza.
- `README.md` — informações do projeto.
- `DIÁRIO.md` — registro de desenvolvimento.
- `AGENTS.md` — regras do projeto.

## Compilação e execução

- `make` — compila o projeto usando `CLI_LIB_DIR=../cli-lib` por padrão.
- `make run` — compila, se necessário, e executa `build/cosmo-dash`.
- `make clean` — remove apenas a pasta `build` do projeto.

Para uma CLI-LIB em outro caminho, configure `CLI_LIB_DIR`:

- `make CLI_LIB_DIR=/caminho/para/cli-lib`
- `make run CLI_LIB_DIR=/caminho/para/cli-lib`

Exemplo no ambiente atual: `make CLI_LIB_DIR=/home/flepersons/Projetos/cli-lib`.

Executável: `build/cosmo-dash`.
## Equipe

Projeto desenvolvido em grupo como atividade acadêmica do curso de Ciência da Computação.

- André Luiz Bayma Antunes Correia
- Arthur Alencar Harten de Moraes
- Felipe Oliveira do Nascimento
