# Lista 1

## 1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?

OpenGL Shading Language (GLSL) é uma linguagem de programação de alto nível, semelhante ao C, usada para escrever shaders em OpenGL. Na versão do OpenGL 4 que usamos, os dois shaders essenciais são:

- **Vertex Shader**: responsável pelo processamento de vértices.
- **Fragment Shader**: lida com a forma como as áreas das superfícies são renderizadas.

## 2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?

As primitivas gráficas representam os elementos mais básicos na construção de formas em OpenGL:

- **GL_POINTS**: desenha pontos individuais.
- **GL_LINES**: cria uma linha para cada par de pontos.
- **GL_LINE_STRIP**: conecta todos os pontos em uma sequência de linhas.
- **GL_LINE_LOOP**: similar ao GL_LINE_STRIP, mas fecha o loop ao conectar o último ponto ao primeiro.
- **GL_TRIANGLES**: forma triângulos a partir de grupos de três pontos.
- **GL_TRIANGLE_STRIP**: cria uma série de triângulos conectados.
- **GL_TRIANGLE_FAN**: forma triângulos a partir de um ponto central e dois pontos adjacentes.

Na OpenGL, os vértices são geralmente armazenados usando Vertex Buffer Objects (VBOs).

## 3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).

- **Vertex Buffer Object (VBO)**: é um buffer que armazena dados de vértices, como posições e cores, diretamente na memória da GPU, acelerando o processo de renderização.
- **Vertex Array Object (VAO)**: é utilizado para configurar os atributos dos vértices, determinando qual VBO será usado, além de especificar o formato e a localização desses dados.
- **Element Buffer Object (EBO)**: também conhecido como Index Buffer Object (IBO), armazena índices que definem quais vértices formarão cada primitiva gráfica, permitindo reutilizar vértices e economizar memória.

## 4. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.

## 5. Faça o desenho de 2 triângulos na tela. Desenhe-os:

a. **Apenas com o polígono preenchido**
![Apenas com o polígono preenchido - Vitor](/Lista1/Lista1_5_Triangulo/Prints/5_a.png)
b. **Apenas com contorno**  
![Apenas com contorno - Vitor](/Lista1/Lista1_5_Triangulo/Prints/5_b.png)
c. **Apenas como pontos**  
![Apenas como pontos - Vitor](/Lista1/Lista1_5_Triangulo/Prints/5_c.png)
d. **Com as 3 formas de desenho juntas**
![Com as 3 formas de desenho juntas - Vitor](/Lista1/Lista1_5_Triangulo/Prints/5_d.png)
