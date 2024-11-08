import numpy as np
import cv2 as cv

############################################################
#original - babuino
img = cv.imread('baboon.png') #original
print("Atributos da imagem",img.shape,"\n")
imgResult = img.copy()
imgColored = img.copy()
imgInverted = img.copy()
############################################################

############################################################
#colorizada - babuino
mColor = [255, 0, 255]
############################################################


############################################################
#saturation - babuino
imgTest = cv.cvtColor(img, cv.COLOR_BGR2HSV)
H, S, V = cv.split(imgTest)
S = S + 15
S = np.clip(S,0,255)
imgTest = cv.merge([H, S, V])
imgTest = cv.cvtColor(imgTest, cv.COLOR_HSV2BGR)
#R, G, B = cv.split(img)
############################################################

############################################################
#original - bolinhas
img2 = cv.imread('bolinhas.png') #original
imgResult2 = img2.copy()
############################################################

############################################################
#grayScale - babuino
imgGray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)
print("Atributos da imagem",imgGray.shape,"\n")
############################################################

#Babuino
for i in range(img.shape[0]): #percorre linhas
	for j in range(img.shape[1]): #percorre colunas

		#canal R - babuino
		imgResult.__setitem__((i,j,0),img.item(i,j,2)) # canal B!!!!!!!!!!!!!!!!
		imgResult.__setitem__((i,j,1),img.item(i,j,2)) # canal G
		imgResult.__setitem__((i,j,2),img.item(i,j,2)) # canal R!!!!

		#colorizada - babuino
		imgColored.__setitem__((i,j,0),img.item(i,j,0) | mColor[0]) # canal B!!!!!!!!!!!!!!!!
		imgColored.__setitem__((i,j,1),img.item(i,j,1) | mColor[1]) # canal G
		imgColored.__setitem__((i,j,2),img.item(i,j,2) | mColor[2]) # canal R!!!!

		#inversão - babuino
		imgInverted.__setitem__((i,j,0),img.item(i,j,0) ^ 255) # canal B!!!!!!!!!!!!!!!!
		imgInverted.__setitem__((i,j,1),img.item(i,j,1) ^ 255) # canal G
		imgInverted.__setitem__((i,j,2),img.item(i,j,2) ^ 255) # canal R!!!!

#Bolinhas	
for i in range(img2.shape[0]): #percorre linhas
	for j in range(img2.shape[1]): #percorre colunas

		#canal R - bolinhas
		imgResult2.__setitem__((i,j,0),img2.item(i,j,2)) # canal B!!!!!!!!!!!!!!!!
		imgResult2.__setitem__((i,j,1),img2.item(i,j,2)) # canal G
		imgResult2.__setitem__((i,j,2),img2.item(i,j,2)) # canal R!!!!

cv.imshow("Original - Babuino", img)
cv.imshow("Canal R - Babuino",imgResult)
cv.imshow("Colorizada - Babuino",imgColored)
cv.imshow("Inversão - Babuino",imgInverted)
cv.imshow("Saturation - Babuino",imgTest)
cv.imshow("Grayscale (cvtColor) - Babuino",imgGray)
cv.imshow("Canal R - Bolinhas",imgResult2)

k = cv.waitKey(0)


# Filtros e Transformações
# Imagem Original - Babuino
# A imagem é lida usando cv.imread('baboon.png') e armazenada na variável img. 
# Suas cópias são feitas em imgResult, imgColored e imgInverted para serem manipuladas 
# por diferentes filtros.

# Imagem Colorizada - Babuino
# Define-se uma cor mColor = [255, 0, 255] (um magenta forte). 
# Essa cor será usada para modificar os valores dos canais da imagem 
# no filtro imgColored, aplicando um efeito de cor em toda a imagem do babuíno.

# Saturação - Babuino
# Converte a imagem do babuíno para o espaço de cores HSV (Hue, Saturation, Value) usando cv.COLOR_BGR2HSV.
# Aumenta a saturação (S) em 15 unidades para intensificar as cores e depois restringe os valores entre 0 e 255 com np.clip().
# Converte a imagem de volta para o espaço de cores BGR, resultando em uma versão da imagem com cores mais saturadas.

# Imagem Original - Bolinhas
# Lê a imagem bolinhas.png e cria uma cópia chamada imgResult2 para aplicar alterações nos canais de cor.

# Escala de Cinza - Babuino
# Converte a imagem do babuíno para escala de cinza usando cv.COLOR_RGB2GRAY, reduzindo a imagem para um único canal e resultando em uma versão sem cores.



# Loops para Modificações de Cor
# Estes loops percorrem cada pixel das imagens e aplicam transformações específicas nos canais RGB.

# Canal R - Babuino
# Em imgResult, os valores dos três canais (B, G, R) de cada pixel são substituídos
# pelo valor do canal vermelho (R) da imagem original, resultando em uma 
# imagem monocromática baseada no canal vermelho.

# Imagem Colorizada - Babuino
# Em imgColored, cada pixel é modificado usando a cor mColor definida anteriormente. 
# Isso é feito com uma operação OR (|) entre o valor de cada canal do pixel original
# e o valor de mColor. Isso cria um efeito colorizado onde o magenta é sobreposto 
# aos canais da imagem original.

# Inversão - Babuino
# Em imgInverted, cada canal RGB de cada pixel é invertido com uma operação XOR (^ 255).
# Isso gera uma imagem negativa do babuíno.

# Canal R - Bolinhas
# Similar ao filtro Canal R - Babuino, mas aplicado à imagem bolinhas.png. 
# Todos os canais de cada pixel são substituídos pelo valor do canal vermelho, 
# resultando em uma versão monocromática.