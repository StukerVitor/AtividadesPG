import cv2
import numpy as np
# Dicionário para mapear teclas aos filtros
filters = {
    # Filtro de Detecção de Bordas Canny: 'frame' é a imagem de entrada, '50' é o limite inferior e '100' é o limite superior para detecção de bordas.
    ord('c'): {'active': False, 'function': lambda frame: cv2.Canny(frame, 50, 100)},
    
    # Filtro de Desfoque Gaussiano: 'frame' é a imagem de entrada, '(15, 15)' define o tamanho do kernel (largura e altura), e '0' é o desvio padrão no eixo X e Y.
    ord('b'): {'active': False, 'function': lambda frame: cv2.GaussianBlur(frame, (15, 15), 0)},
    
    # Aplica filtro magenta customizado, definindo tons magenta na imagem.
    ord('m'): {'active': False, 'function': lambda frame: apply_magenta(frame)},
    
    # Filtro Sobel: 'frame' é a imagem de entrada, 'cv2.CV_64F' define o tipo de profundidade de saída, '1' e '0' especificam a derivada no eixo X e não no Y, 'ksize=5' é o tamanho do kernel.
    ord('s'): {'active': False, 'function': lambda frame: cv2.Sobel(frame, cv2.CV_64F, 1, 0, ksize=5)},
    
    # Filtro Laplaciano: 'frame' é a imagem de entrada, 'cv2.CV_64F' define a profundidade de saída para capturar bordas e contrastes em múltiplas direções.
    ord('l'): {'active': False, 'function': lambda frame: cv2.Laplacian(frame, cv2.CV_64F)},
    
    # Equalização de Histograma: 'frame' é a imagem de entrada convertida para tons de cinza, melhorando o contraste pela redistribuição dos valores de pixel.
    ord('h'): {'active': False, 'function': lambda frame: cv2.equalizeHist(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY))},
    
    # Filtro de Binarização (Threshold): 'frame' é a imagem de entrada convertida para tons de cinza, '127' é o valor limite, '255' é o valor máximo, e 'cv2.THRESH_BINARY' cria uma imagem binária.
    ord('t'): {'active': False, 'function': lambda frame: cv2.threshold(cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY), 127, 255, cv2.THRESH_BINARY)[1]},
    
    # Erosão: 'frame' é a imagem de entrada, 'np.ones((5, 5), np.uint8)' cria o kernel de erosão (tamanho 5x5), 'iterations=1' define o número de vezes que a erosão é aplicada.
    ord('e'): {'active': False, 'function': lambda frame: cv2.erode(frame, np.ones((5, 5), np.uint8), iterations=1)},
    
    # Dilatação: 'frame' é a imagem de entrada, 'np.ones((5, 5), np.uint8)' cria o kernel de dilatação (tamanho 5x5), 'iterations=1' define o número de vezes que a dilatação é aplicada.
    ord('d'): {'active': False, 'function': lambda frame: cv2.dilate(frame, np.ones((5, 5), np.uint8), iterations=1)},
    
    # Inversão de Cores: 'frame' é a imagem de entrada, invertendo as cores, ou seja, cada valor de pixel é subtraído de 255.
    ord('n'): {'active': False, 'function': lambda frame: cv2.bitwise_not(frame)}
}

# Dicionário para mapear teclas aos stickers
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

# Variável global para armazenar o sticker selecionado
current_sticker = None

# Lista para armazenar as posições dos stickers e os stickers correspondentes
stickers = []

def apply_magenta(frame):
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    magenta = np.zeros_like(frame)
    magenta[:, :, 0] = gray * 0.5  # Canal Azul - Menor intensidade
    magenta[:, :, 1] = gray * 0    # Canal Verde - Zero intensidade
    magenta[:, :, 2] = gray        # Canal Vermelho - Maior intensidade
    return magenta

def display_instructions():
    print("\nINSTRUÇÕES DE COMANDO")
    print("===============================")
    print("c: Canny edge detection")
    print("b: Gaussian blur")
    print("m: Magenta filter")
    print("s: Sobel filter")
    print("l: Laplacian filter")
    print("h: Histogram equalization")
    print("t: Binary thresholding")
    print("e: Erosion")
    print("d: Dilation")
    print("n: Negative transformation")
    print("===============================")
    print("r: Remover o filtro ativo")
    print("f: Finalizar edição")
    print("q: Salvar")
    print("===============================")
    print("a: Adicionar sticker cabeca_explodindo")
    print("g: Adicionar sticker cabra")
    print("i: Adicionar sticker choro_gritando")
    print("k: Adicionar sticker dollyinho_rindo")
    print("o: Adicionar sticker gato_surpreso")
    print("p: Adicionar sticker joinha")
    print("u: Adicionar sticker monstros_sa")
    print("z: Adicionar sticker nazare_pensativa")
    print("y: Adicionar sticker patrick bolado")
    print("w: Adicionar sticker sapo")
    print("===============================")

def overlay_sticker(background, sticker, x, y):
    # Dimensões do sticker
    sticker_height, sticker_width = sticker.shape[:2]
    
    # Verifica se o sticker tem 4 canais (RGBA) ou 3 canais (RGB)
    if sticker.shape[2] == 4:
        # Canal Alfa presente
        sticker_alpha = sticker[:, :, 3] / 255.0
        bg_alpha = 1.0 - sticker_alpha

        # Limites da área do sticker na imagem de fundo
        y1, y2 = y, y + sticker_height
        x1, x2 = x, x + sticker_width
        
        # Verifica se o sticker cabe na imagem de fundo sem ultrapassar os limites
        if y2 > background.shape[0] or x2 > background.shape[1]:
            return background
        
        # Calcula a região de interesse (ROI) para a aplicação do sticker
        roi = background[y1:y2, x1:x2]
        
        # Combinar o sticker com a imagem de fundo usando a máscara alfa
        for c in range(0, 3):
            background[y1:y2, x1:x2, c] = (sticker_alpha * sticker[:, :, c] +
                                            bg_alpha * background[y1:y2, x1:x2, c])
    else:
        # Sem canal Alfa, aplica o sticker diretamente
        y1, y2 = y, y + sticker_height
        x1, x2 = x, x + sticker_width
        
        # Verifica se o sticker cabe na imagem de fundo sem ultrapassar os limites
        if y2 > background.shape[0] or x2 > background.shape[1]:
            return background
        
        # Aplica o sticker diretamente na região de interesse (ROI)
        background[y1:y2, x1:x2] = sticker
    
    return background

# Função para capturar eventos do mouse
def mouse_callback(event, x, y, flags, param):
    global stickers, current_sticker
    
    if event == cv2.EVENT_LBUTTONDOWN and current_sticker is not None:
        stickers.append((x, y, current_sticker))  # Adiciona a posição do sticker e o sticker à lista

def edit_image(image_path):
    global current_sticker, stickers

    # Carrega a imagem
    image = cv2.imread(image_path)
    if image is None:
        print("Não foi possível carregar a imagem. Verifique o caminho e tente novamente.")
        return

    display_instructions()

    # Cria a janela onde a imagem editada será exibida
    cv2.namedWindow('Imagem - Editada')
    cv2.setMouseCallback('Imagem - Editada', mouse_callback)
    salvar = 0
    count = 0
    while True:
        frame_filtered = image.copy()
        
        # Verifica se alguma tecla foi pressionada
        key = cv2.waitKey(1) & 0xFF
        
        if key == ord('f'):
            break
        
        # Remover o filtro ativo ao pressionar 'r'
        if key == ord('r'):
            for filter_key in filters:
                filters[filter_key]['active'] = False
        
        # Salva a imagem editada ao pressionar 'q'
        if key == ord('q'):
            salvar = 1

        # Ativar ou desativar o filtro correspondente à tecla pressionada
        if key in filters:
            filter_key = key
            
            # Desativa todos os outros filtros antes de ativar o filtro selecionado
            for fkey in filters:
                if fkey != filter_key:
                    filters[fkey]['active'] = False
            
            # Inverte o estado do filtro selecionado
            filters[filter_key]['active'] = not filters[filter_key]['active']
        
        # Verifica se a tecla pressionada corresponde a um sticker
        if key in stickers_map:
            current_sticker = stickers_map[key]
        
        # Aplica o filtro ativo ao frame atual
        for key in filters:
            if filters[key]['active']:
                frame_filtered = filters[key]['function'](frame_filtered)
                # Verifica se a imagem está em escala de cinza e converte para 3 canais
                if len(frame_filtered.shape) == 2:
                    frame_filtered = cv2.cvtColor(frame_filtered, cv2.COLOR_GRAY2BGR)
        
        # Aplica todos os stickers na lista
        for (x, y, sticker) in stickers:
            frame_filtered = overlay_sticker(frame_filtered, sticker, x, y)

        if salvar == 1: 
            cv2.imwrite(f'./images/imagem_editada{count}.jpg', frame_filtered)
            print(f"Imagem editada salvo como 'imagem_editada{count}.jpg'")
            salvar = 0
            count+=1

        # Exibe o frame filtrado
        cv2.imshow('Imagem - Editada', frame_filtered)

    cv2.destroyAllWindows()
    stickers = []  # Limpa a lista de stickers após a edição

def edit_webcam():
    global current_sticker

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Não foi possível abrir a câmera.")
        return

    display_instructions()

    # Cria a janela onde a imagem da webcam será exibida
    cv2.namedWindow('Webcam - Editada')
    cv2.setMouseCallback('Webcam - Editada', mouse_callback)
    salvar = 0
    count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Erro ao capturar o quadro.")
            break

        frame_filtered = frame.copy()
        
        # Verifica se alguma tecla foi pressionada
        key = cv2.waitKey(1)
        
        if key == ord('f'):
            break
        
        # Remover o filtro ativo ao pressionar 'r'
        if key == ord('r'):
            for filter_key in filters:
                filters[filter_key]['active'] = False
  
        # Salva a imagem editada ao pressionar 'q'
        if key == ord('q'):
            salvar = 1
        
        # Ativar ou desativar o filtro correspondente à tecla pressionada
        if key in filters:
            filter_key = key
            
            # Desativa todos os outros filtros antes de ativar o filtro selecionado
            for fkey in filters:
                if fkey != filter_key:
                    filters[fkey]['active'] = False
            
            # Inverte o estado do filtro selecionado
            filters[filter_key]['active'] = not filters[filter_key]['active']
        
        # Verifica se a tecla pressionada corresponde a um sticker
        if key in stickers_map:
            current_sticker = stickers_map[key]
        
        # Aplica o filtro ativo ao frame atual
        for key in filters:
            if filters[key]['active']:
                frame_filtered = filters[key]['function'](frame_filtered)
                # Verifica se a imagem está em escala de cinza e converte para 3 canais
                if len(frame_filtered.shape) == 2:
                    frame_filtered = cv2.cvtColor(frame_filtered, cv2.COLOR_GRAY2BGR)

        # Aplica todos os stickers na lista
        for (x, y, sticker) in stickers:
            frame_filtered = overlay_sticker(frame_filtered, sticker, x, y)
            #Salva o frame da webcam editado ao pressionar 'q'
        if salvar == 1: 
            cv2.imwrite(f'./images/webcam_editada{count}.jpg', frame_filtered)
            print(f"Frame da webcam editado salvo como 'webcam_editada{count}.jpg'")
            salvar = 0
            count+=1

        # Exibe o frame filtrado
        cv2.imshow('Webcam - Editada', frame_filtered)

    cap.release()
    cv2.destroyAllWindows()

def main():
    global current_sticker, stickers

    while True:
        print("\nMENU PRINCIPAL")
        print("\n===============================")
        print("1: Editar imagem")
        print("2: Editar vídeo da webcam")
        print("3: Sair")
        print("===============================")
        choice = input("Selecione uma opção: ")

        if choice == '1':
            # Limpar estado de edição
            current_sticker = None
            stickers = []
            for filter_key in filters:
                filters[filter_key]['active'] = False

            image_path = input("\nDigite o caminho da imagem: ")
            edit_image(image_path)
        elif choice == '2':
            # Limpar estado de edição
            current_sticker = None
            stickers = []
            for filter_key in filters:
                filters[filter_key]['active'] = False

            edit_webcam()
        elif choice == '3':
            break
        else:
            print("\nOpção inválida. Tente novamente.")

if __name__ == "__main__":
    main()
