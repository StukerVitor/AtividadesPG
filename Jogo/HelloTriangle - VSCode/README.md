# 🐰 Rabbit Game

Rabbit Game é um jogo 2D simples desenvolvido usando OpenGL e C++, onde o objetivo é controlar um coelho para coletar cenouras e evitar espadas que caem do céu. O projeto foi criado para demonstrar conceitos básicos de manipulação de sprites, renderização 2D, transformações de objetos e interações com o usuário utilizando OpenGL.

## 📜 Sumário

- [Instalação](#instalação)
- [Fluxo de Execução do Jogo](#fluxo-de-execução-do-jogo)
- [Como Jogar](#como-jogar)
- [Controles](#controles)
- [Estrutura do Código](#estrutura-do-código)
- [Shaders](#shaders)
- [Texturas Utilizadas](#texturas-utilizadas)
- [Detalhes Técnicos](#detalhes-técnicos)
- [Créditos](#créditos)

## 📥 Instalação

Para compilar e executar o jogo Rabbit Game, siga os passos abaixo:

### Pré-requisitos

Certifique-se de que seu ambiente tenha as seguintes bibliotecas instaladas:

- **GLFW** - Biblioteca para criação de janelas e gerenciamento de contexto OpenGL.
- **GLAD** - Biblioteca para carregar extensões OpenGL.
- **STB_IMAGE** - Biblioteca para carregamento de imagens como texturas.
- **GLM** - Biblioteca para manipulação de vetores e matrizes.
- **OpenGL** - Necessário para renderização gráfica.

### Compilação

Clone o repositório do projeto:

```bash
git clone https://github.com/StukerVitor/AtividadesPG
```

Entre no diretório do projeto:

```bash
cd '.\Jogo\HelloTriangle - VSCode\'
```

Rode o jogo através do botão play do VSCode.

Certifique-se de que as bibliotecas necessárias estejam corretamente instaladas e linkadas no seu ambiente de desenvolvimento.

## 🔄 Fluxo de Execução do Jogo

A execução do jogo Rabbit Game segue um fluxo estruturado que pode ser dividido em várias etapas:

### Inicialização do Ambiente e Configuração de Janelas

- A biblioteca GLFW é inicializada para criar a janela principal e gerenciar eventos de entrada.
- Em seguida, é criado um contexto OpenGL para essa janela.
- A biblioteca GLAD é usada para carregar as funções OpenGL necessárias.
- O jogo então configura funções de callback para eventos de teclado e mouse, como `key_callback` e `mouse_button_callback`.
- Os parâmetros do OpenGL são ajustados para habilitar recursos como transparência (blending) e definir a matriz de projeção ortográfica para renderizar sprites em 2D.

### Carregamento de Texturas

O jogo utiliza a biblioteca STB Image para carregar texturas a partir de arquivos de imagem, que são aplicadas aos diferentes sprites do jogo. As texturas são carregadas com a função `loadTexture`, que gera um identificador para cada imagem e a configura como textura 2D:

- **Texturas de Cenoura, Espadas, Coelho (direita/esquerda), Botões (`start`, `play again`, `exit`) e Telas de `Game Over` e `Vitória`**.
- Essas texturas são então vinculadas a diferentes sprites e armazenadas para uso posterior.

### Criação de Sprites

Com as texturas carregadas, o programa inicializa diferentes instâncias da classe `Sprite`, definindo a posição, escala e rotação de cada objeto. Cada sprite representa um elemento na tela, como:

- `coelho` (personagem principal)
- `espada_1` e `espada_2` (obstáculos)
- `cenoura` (objetivo de coleta)
- `background`, `gameOver`, `vitoria`, `placar` e botões de controle.

Cada sprite é configurado com sua textura correspondente e é preparado para ser desenhado na tela durante a execução do jogo.

### Loop Principal do Jogo

Após a configuração inicial, o jogo entra no loop principal, onde os seguintes passos são executados repetidamente:

1. **Verificação de Eventos de Entrada**:  
   A cada iteração, o jogo verifica se algum evento de entrada (como um clique de mouse ou uma tecla pressionada) foi acionado e atualiza o estado do jogo de acordo.

2. **Controle de Estados do Jogo**:  
   O fluxo principal depende do valor da variável `gameState`, que pode assumir os seguintes estados:

   - `gameState == 0`: **Menu Inicial** – Exibe o fundo e o botão de início.
   - `gameState == 1`: **Jogo em Andamento** – O jogador controla o coelho para coletar cenouras e desviar das espadas.
   - `gameState == 2`: **Tela de Game Over** – Exibe a tela de Game Over e oferece opções para reiniciar ou sair.
   - `gameState == 3`: **Tela de Vitória** – Exibe a tela de vitória com as mesmas opções.

3. **Renderização**:  
   O OpenGL é utilizado para renderizar cada sprite na tela com base em suas propriedades de transformação (posição, escala e rotação).  
   A função `desenhar()` de cada sprite é chamada para enviar os dados ao pipeline gráfico e desenhar o objeto na tela.

4. **Lógica de Jogo**:  
   A cada frame, o estado do jogo é atualizado, verificando se o jogador atingiu a pontuação máxima ou se colidiu com algum obstáculo.

   - As posições dos sprites (espadas e cenoura) são atualizadas com base em suas velocidades de queda.
   - As colisões são verificadas usando a função `colisao()` para determinar se o coelho entrou em contato com uma espada ou uma cenoura.

5. **Atualização e Renderização de Estado**:  
   Dependendo do estado atual (`gameState`), o jogo exibe diferentes elementos na tela e realiza transições de estado (por exemplo, de Menu para Jogo ou de Jogo para Game Over).

6. **Troca de Buffers**:  
   A cada iteração do loop, os buffers são trocados (`glfwSwapBuffers`) para atualizar a tela com as novas posições e estados dos objetos.

### Finalização

Quando a janela é fechada ou o jogo é encerrado, o programa libera todos os recursos e encerra a execução corretamente, chamando `glfwTerminate()`.

## 🎮 Como Jogar

O objetivo do jogo é controlar o coelho para coletar cenouras e evitar as espadas que caem do topo da tela. A cada cenoura coletada, o jogador ganha 1 ponto. Se o jogador atingir 3 pontos, ele ganha o jogo. Se colidir com uma espada, o jogo termina.

### Objetivo

- **Coletar Cenouras**: O coelho deve coletar cenouras para ganhar pontos.
- **Evitar Espadas**: O jogador deve desviar das espadas para não perder.

## 🕹️ Controles

- **Seta Direita** ou **Tecla D**: Move o coelho para a direita.
- **Seta Esquerda** ou **Tecla A**: Move o coelho para a esquerda.
- **Tecla ESC**: Fecha o jogo.

### Interações com o Mouse

- Clique no Botão **"Start"**: Inicia o jogo.
- Clique no Botão **"Play Again"**: Reinicia o jogo após Game Over ou Vitória.
- Clique no Botão **"Exit"**: Fecha o jogo.

## 📂 Estrutura do Código

Cada arquivo no projeto tem um papel específico:

- **`Sprite.h`**: Define a classe Sprite para representar objetos 2D.
- **`Sprite.cpp`**: Implementa as funções de transformação, desenho e movimento dos sprites.
- **`Source.cpp`**: Contém a lógica principal do jogo, como a inicialização, manipulação de eventos, lógica de colisão e renderização.

## 📜 Detalhes Técnicos

- **Linguagem de Programação**: C++
- **Frameworks**: OpenGL, GLFW, GLAD, GLM
- **Bibliotecas**: STB Image para carregamento de texturas
- **Shaders**: Shaders personalizados para renderização dos sprites.

## 🙌 Créditos

- **Desenvolvedor**: Vitor Stüker
- **Texturas**: CraftPix, OpenGameArt, e outras fontes de arte livre.
- **Bibliotecas**: GLFW, GLAD, STB Image, GLM.
