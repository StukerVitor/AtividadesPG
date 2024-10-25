#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace glm;

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry();
int loadTexture(string filePath, int &imgWidth, int &imgHeight);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800,
			 HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
								   "layout (location = 0) in vec3 position;\n"
								   "layout(location = 1) in vec2 aTexCoord;\n"
								   "out vec2 TexCoord;\n"
								   "uniform mat4 projection;\n"
								   "uniform mat4 model;\n"
								   "void main()\n"
								   "{\n"
								   "    gl_Position = projection * model * vec4(position, 1.0);\n"
								   "    TexCoord = aTexCoord;\n"
								   "}\0";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
									 "out vec4 color;\n"
									 "in vec2 TexCoord;\n"
									 "uniform sampler2D texBuffer;\n"
									 "void main()\n"
									 "{\n"
									 "    color = texture(texBuffer, TexCoord);\n"
									 "}\n\0";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Textures - Vitor Stuker", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
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
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de dois triângulos
	GLuint VAO = setupGeometry();

	// Carregando uma textura (recebendo seu ID)
	int imgWidth, imgHeight;
	GLuint texID = loadTexture("../Texturas/Tiles.jpg", imgWidth, imgHeight);

	// Ativando o shader program
	glUseProgram(shaderID);

	// Definindo o uniform do sampler de textura
	glUniform1i(glGetUniformLocation(shaderID, "texBuffer"), 0);

	// left right bottom top near far
	mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	// Matriz de modelo - Transformações na geometria (objetos)
	mat4 model = mat4(1); // Matriz identidade (diagonal principal = 1, resto 0)

	// Translação
	model = translate(model, vec3(400.0, 300.0, 0.0)); // deslocamento x, y, z

	// Rotação
	model = rotate(model, radians(45.0f), vec3(0.0, 0.0, 1.0)); // 45 graus

	// Escala
	model = scale(model, vec3(300.0, 300.0, 1.0)); // escalamento x, y, z

	// Enviar para o shader
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(2);
		glPointSize(10);

		glBindVertexArray(VAO); // Conectando ao buffer de geometria

		// Ativando a textura
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		// Atualizando a matriz de modelo com a rotação
		float angulo = glfwGetTime();
		// Matriz de modelo - Transformações na geometria (objetos)
		mat4 model = mat4(1); // Matriz identidade
		model = translate(model, vec3(400.0, 300.0, 0.0));
		model = rotate(model, radians(angulo * 50), vec3(0.0, 0.0, 1.0));
		model = scale(model, vec3(300.0, 300.0, 1.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

		// Desenhar
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

int loadTexture(string filePath, int &imgWidth, int &imgHeight)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Configurando o wrapping da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Configurando o filtering de minificação e magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Carregamento da imagem da textura
	int nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);

	int size = imgWidth * imgHeight * nrChannels;
	for (int i = 0; i < size; i += nrChannels)
	{
		data[i] = rand() % 256;		// canal vermelho
		data[i + 1] = rand() % 256; // canal verde
		data[i + 2] = rand() % 256; // canal azul
	}

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << filePath << std::endl;
	}

	return texID;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no iniçio deste arquivo
//  A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			 << infoLog << endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			 << infoLog << endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			 << infoLog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry()
{
	// Coordenadas dos vértices e coordenadas de textura
	GLfloat vertices[] = {
		// x      y      z      s     t
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // v0
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// v1
		0.0f, 0.5f, 0.0f, 0.5f, 1.0f	// v2
	};

	GLuint VBO, VAO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);

	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Atributo de posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo de coordenadas de textura
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Desvincula o VBO e o VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}
