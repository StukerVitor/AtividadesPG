#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry();
int triangulo();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
								   "layout (location = 0) in vec3 position;\n"
								   "void main()\n"
								   "{\n"
								   //...pode ter mais linhas de código aqui!
								   "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
								   "}\0";

// Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
									 "uniform vec4 inputColor;\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = inputColor;\n"
									 "}\n\0";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5 - Guilherme Maia Nogueira", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/exercicio8.vs", "../shaders/exercicio8.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = triangulo();

	shader.Use();

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO); // Conectando ao buffer de geometria

		// 1 - Polígono Preenchido
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 2 - Polígono com contorno
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glDrawArrays(GL_LINE_LOOP, 3, 3);

		// 3 - Apenas os vértices dos polígonos
		glDrawArrays(GL_POINTS, 0, 6);

		glBindVertexArray(0); // Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Pede para OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int triangulo()
{
	// Aqui setamos as coordenadas x, y e z
	GLfloat vertices[] = {
		// x   y     z    r   g    b
		// Triângulo 0
		-0.5, 0.5, 0.0, 1.0, 0.0, 0.0,	// v0
		0.0, 0.0, 0.0, 0.0, 1.0, 0.0,	// v1
		0.5, 0.5, 0.0, 0.0, 0.0, 1.0,	// v2
										// Triângulo 1
		0.0, 0.0, 0.0, 1.0, 1.0, 0.0,	// v3
		-0.5, -0.5, 0.0, 0.0, 1.0, 1.0, // v4
		0.5, -0.5, 0.0, 1.0, 0.0, 1.0,	// v5
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//                                              vertices.data()
	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vértice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Número de valores que o atributo tem (por ex, 3 coordenadas xyz)
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes
	// Deslocamento a partir do byte zero

	// Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}
