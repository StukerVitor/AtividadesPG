Nome: Vitor Stüker de Almeida
Link apresentação: https://www.youtube.com/watch?v=xHYw-K3d3DI

# Explicação Completa do Código

Este documento fornece uma explicação detalhada do código fornecido, que é um aplicativo em Python utilizando a biblioteca OpenCV para aplicar filtros e stickers (adesivos) em imagens e vídeos capturados pela webcam. O programa permite que o usuário interaja em tempo real com a imagem, aplicando diferentes efeitos visuais e adicionando stickers em posições específicas.

## Importação de Bibliotecas

```python
import cv2
import numpy as np
```

- cv2: Biblioteca OpenCV para processamento de imagens e vídeos.
- numpy: Biblioteca para manipulação de arrays e operações numéricas.

## Dicionário de Filtros

```python
filters = {
    ord('c'): {'active': False, 'function': lambda frame: cv2.Canny(frame, 50, 100)},
    ord('b'): {'active': False, 'function': lambda frame: cv2.GaussianBlur(frame, (15, 15), 0)},
    ord('m'): {'active': False, 'function': lambda frame: apply_magenta(frame)},
    ord('s'): {'active': False, 'function': lambda frame: cv2.Sobel(frame, cv2.CV_64F, 1, 0, ksize=5)},
    ord('l'): {'active': False, 'function': lambda frame: cv2.Laplacian(frame, cv2.CV_64F)},
    ord('h'): {'active': False, 'function': lambda frame: cv2.equalizeHist(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))},
    ord('t'): {'active': False, 'function': lambda frame: cv2.threshold(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)[1]},
    ord('e'): {'active': False, 'function': lambda frame: cv2.erode(frame, np.ones((5, 5), np.uint8), iterations=1)},
    ord('d'): {'active': False, 'function': lambda frame: cv2.dilate(frame, np.ones((5, 5), np.uint8), iterations=1)},
    ord('n'): {'active': False, 'function': lambda frame: cv2.bitwise_not(frame)}
}
```

- Descrição: Um dicionário que mapeia teclas (convertidas para seus códigos ASCII usando ord()) para filtros específicos. Cada filtro tem um estado (active) que indica se está ativo ou não, e uma função (function) que aplica o filtro à imagem.

## Filtros Disponíveis:

1. Canny Edge Detection (c):

   - Detecta bordas na imagem.
   - Parâmetros: Limite inferior de 50 e superior de 100.
   - Função: cv2.Canny(frame, 50, 100)

2. Gaussian Blur (b):

   - Aplica um desfoque gaussiano para suavizar a imagem.
   - Parâmetros: Kernel de tamanho (15, 15) e desvio padrão 0.
   - Função: cv2.GaussianBlur(frame, (15, 15), 0)

3. Filtro Magenta (m):

   - Aplica um efeito magenta personalizado.
   - Função: apply_magenta(frame)

4. Filtro Sobel (s):

   - Calcula gradientes da imagem para detecção de bordas.
   - Parâmetros: Profundidade de saída cv2.CV_64F, derivada em X, kernel de tamanho 5.
   - Função: cv2.Sobel(frame, cv2.CV_64F, 1, 0, ksize=5)

5. Filtro Laplaciano (l):

   - Detecta bordas e detalhes finos.
   - Parâmetros: Profundidade de saída cv2.CV_64F.
   - Função: cv2.Laplacian(frame, cv2.CV_64F)

6. Equalização de Histograma (h):

   - Melhora o contraste da imagem em escala de cinza.
   - Função: cv2.equalizeHist(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))

7. Binarização (t):

   - Converte a imagem em uma imagem binária (preto e branco).
   - Parâmetros: Limite de 127, valor máximo de 255.
   - Função: cv2.threshold(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)[1]

8. Erosão (e):

   - Erode áreas brancas em uma imagem binária.
   - Parâmetros: Kernel de tamanho (5, 5), 1 iteração.
   - Função: cv2.erode(frame, np.ones((5, 5), np.uint8), iterations=1)

9. Dilatação (d):

   - Expande áreas brancas em uma imagem binária.
   - Parâmetros: Kernel de tamanho (5, 5), 1 iteração.
   - Função: cv2.dilate(frame, np.ones((5, 5), np.uint8), iterations=1)

10. Inversão de Cores (n):
    - Inverte as cores da imagem.
    - Função: cv2.bitwise\*not(frame)

## Dicionário de Stickers

```python
stickers_map = {
    ord('a'): cv2.imread('cabeca_explodindo.png', cv2.IMREAD_UNCHANGED),
    ord('g'): cv2.imread('cabra.png', cv2.IMREAD_UNCHANGED),
    ord('i'): cv2.imread('choro_gritando.png', cv2.IMREAD_UNCHANGED),
    ord('k'): cv2.imread('dollyinho_rindo.png', cv2.IMREAD_UNCHANGED),
    ord('o'): cv2.imread('gato_surpreso.png', cv2.IMREAD_UNCHANGED),
    ord('p'): cv2.imread('joinha.png', cv2.IMREAD_UNCHANGED),
    ord('u'): cv2.imread('monstros_sa.png', cv2.IMREAD_UNCHANGED),
    ord('z'): cv2.imread('nazare_pensativa.png', cv2.IMREAD_UNCHANGED),
    ord('y'): cv2.imread('patrick_bolado.png', cv2.IMREAD_UNCHANGED),
    ord('w'): cv2.imread('sapo.png', cv2.IMREAD_UNCHANGED),
}
```

- Descrição: Mapeia teclas para imagens de stickers carregadas usando cv2.imread() com a flag cv2.IMREAD_UNCHANGED para preservar o canal alfa (transparência).

## Variáveis Globais

```python
current_sticker = None
stickers = []
```

- current_sticker: Armazena o sticker atualmente selecionado pelo usuário.
- stickers: Lista de tuplas contendo a posição (x, y) e o sticker a ser aplicado na imagem.

## Função apply_magenta

```python
def apply_magenta(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    magenta = np.zeros_like(frame)
    magenta[:, :, 0] = gray * 0.5 # Canal Azul - Menor intensidade
    magenta[:, :, 1] = gray * 0 # Canal Verde - Zero intensidade
    magenta[:, :, 2] = gray # Canal Vermelho - Maior intensidade
    return magenta
```

- Descrição: Cria um efeito magenta personalizado na imagem.
- Processo:
  - Converte a imagem para escala de cinza.
  - Cria uma nova imagem magenta com zeros (preta) com a mesma forma da imagem original.
  - Ajusta os canais de cor:
    - Canal Azul (B): Metade da intensidade do cinza.
    - Canal Verde (G): Zero intensidade.
    - Canal Vermelho (R): Total da intensidade do cinza.
- Retorna a imagem com o efeito magenta aplicado.

## Função display_instructions

```python
def display_instructions():
print("\nINSTRUÇÕES DE COMANDO") # ... (Imprime as instruções de uso)
```

- Descrição: Exibe as instruções de comandos disponíveis para o usuário.

## Função overlay_sticker

```python
def overlay_sticker(background, sticker, x, y): # ... (Código para sobrepor o sticker na imagem de fundo)
    return background
```

- Descrição: Sobrepõe um sticker na imagem de fundo na posição (x, y).
- Processo:
  - Obtém as dimensões do sticker.
  - Verifica se o sticker possui canal alfa (4 canais) para transparência.
  - Calcula as regiões de interesse (ROI) na imagem de fundo onde o sticker será aplicado.
  - Verifica se o sticker cabe na imagem sem ultrapassar os limites.
  - Combina o sticker com a imagem de fundo usando o canal alfa ou aplica diretamente se não houver transparência.
  - Retorna a imagem de fundo com o sticker aplicado.

## Função mouse_callback

```python
def mouse_callback(event, x, y, flags, param):
    global stickers, current_sticker
    if event == cv2.EVENT_LBUTTONDOWN and current_sticker is not None:
        stickers.append((x, y, current_sticker))
```

- Descrição: Função de callback para eventos do mouse.
- Processo:
  - Verifica se o evento é um clique com o botão esquerdo do mouse.
  - Se um sticker estiver selecionado (current_sticker não é None), adiciona a posição (x, y) e o sticker à lista stickers.

## Função edit_image

```python
def edit_image(image_path): # ... (Código para editar uma imagem estática)
```

- Descrição: Permite que o usuário edite uma imagem estática aplicando filtros e adicionando stickers.
- Processo:
- Carrega a imagem a partir do caminho fornecido.
- Configura a janela de visualização e define o callback do mouse.
- Entra em um loop onde:
  - Copia a imagem original para frame_filtered.
  - Captura a tecla pressionada pelo usuário.
  - Atualiza o estado dos filtros e stickers com base na tecla pressionada.
  - Aplica o filtro ativo à imagem.
  - Aplica todos os stickers armazenados na lista stickers.
  - Exibe a imagem filtrada.
  - Permite salvar a imagem ao pressionar a tecla q.
- Sai do loop ao pressionar a tecla f.
- Limpa a lista de stickers após a edição.

## Função edit_webcam

```python
def edit_webcam(): # ... (Código para editar o vídeo da webcam em tempo real)
```

- Descrição: Permite que o usuário aplique filtros e stickers ao vídeo capturado pela webcam em tempo real.
- Processo:

  - Inicia a captura de vídeo da webcam.
  - Configura a janela de visualização e define o callback do mouse.
  - Entra em um loop onde:

    - Lê o frame atual da webcam.
    - Copia o frame para frame_filtered.
    - Captura a tecla pressionada pelo usuário.
    - Atualiza o estado dos filtros e stickers com base na tecla pressionada.
    - Aplica o filtro ativo ao frame.
    - Aplica todos os stickers armazenados na lista stickers.
    - Exibe o frame filtrado.
    - Permite salvar o frame ao pressionar a tecla q.

  - Sai do loop ao pressionar a tecla f.
  - Libera a captura de vídeo e fecha as janelas.

## Função main

```python
def main():
    global current_sticker, stickers
    while True:
        print("\nMENU PRINCIPAL") # ... (Imprime o menu e captura a escolha do usuário)
        if choice == '1': # Limpa o estado de edição # Chama edit_image()
        elif choice == '2': # Limpa o estado de edição # Chama edit_webcam()
        elif choice == '3':
            break
    else:
    print("\nOpção inválida. Tente novamente.")
```

- Descrição: Ponto de entrada do programa que apresenta um menu principal ao usuário.
- Processo:

  - Exibe o menu com as opções:

    - Editar imagem.
    - Editar vídeo da webcam.
    - Sair.

  - Captura a escolha do usuário.
  - Limpa o estado atual (filtros e stickers) antes de iniciar uma nova edição.
  - Chama a função correspondente à escolha:
  - edit_image() se o usuário escolher editar uma imagem.
  - edit_webcam() se o usuário escolher editar o vídeo da webcam.
  - Sai do loop se o usuário escolher sair.
  - Informa se a opção é inválida e solicita outra entrada.

## Execução do Programa

```python
if **name** == "**main**":
main()
```

- Descrição: Verifica se o script está sendo executado diretamente e, em caso afirmativo, chama a função main() para iniciar o programa.

## Resumo do Fluxo do Programa

1.  Inicialização:

    - Importa bibliotecas necessárias.
    - Define dicionários de filtros e stickers.
    - Declara variáveis globais.

2.  Definição de Funções:

    - apply_magenta(): Aplica o filtro magenta personalizado.
    - display_instructions(): Exibe instruções de comandos.
    - overlay_sticker(): Sobrepõe stickers na imagem.
    - mouse_callback(): Gerencia eventos do mouse para posicionar stickers.
    - edit_image(): Permite edição de imagens estáticas.
    - edit_webcam(): Permite edição do vídeo da webcam em tempo real.
    - main(): Gerencia o menu principal e a execução geral do programa.

3.  Execução:

    - O programa inicia chamando main().
    - O usuário interage através do menu e comandos exibidos.
    - O programa aplica filtros e stickers com base nas interações do usuário.
    - O usuário pode salvar as imagens editadas ou sair do programa.
