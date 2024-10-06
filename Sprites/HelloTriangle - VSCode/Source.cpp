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

struct Sprite
{
	GLuint VAO;
	GLuint texID;
	vec3 position;
	vec3 dimensions;
	float angle;
};

// Protótipos das funções
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry();
int loadTexture(string filePath, int &imgWidth, int &imgHeight);
void drawSprite(Sprite spr, GLuint shaderID);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
								   "layout (location = 0) in vec3 position;\n"
								   "layout (location = 1) in vec2 texc;\n"
								   "uniform mat4 projection;\n"
								   "uniform mat4 model;\n"
								   "out vec2 texCoord;\n"
								   "void main()\n"
								   "{\n"
								   //...pode ter mais linhas de código aqui!
								   "gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
								   "texCoord = vec2(texc.s, 1.0 - texc.t);\n"
								   "}\0";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
									 "in vec2 texCoord;\n"
									 "uniform sampler2D texBuffer;\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = texture(texBuffer, texCoord);\n"
									 "}\n\0";

enum directions
{
	NONE,
	LEFT,
	RIGHT
};
int dir = NONE;
float vel = 2.0;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Sprites - Vitor Stuker", nullptr, nullptr);
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

	//////////////////////////////////////////////////////////////////////////////
	// Sprite do fundo da cena
	Sprite background, character;
	int imgWidth, imgHeight;

	background.VAO = setupGeometry();
	background.texID = loadTexture("../Texturas/backgrounds/PNG/Battleground3/Bright/Battleground3.png", imgWidth, imgHeight);
	background.dimensions.x = imgWidth * 0.5;
	background.dimensions.y = imgHeight * 0.5;
	background.dimensions.z = 1.0;
	background.position = vec3(400.0, 300.0, 0.0);

	character.VAO = setupGeometry();
	character.texID = loadTexture("../Texturas/characters/PNG/1 Pink_Monster/Pink_Monster.png", imgWidth, imgHeight);
	character.dimensions = vec3(imgWidth * 2.0, imgHeight * 2.0, 1.0);
	character.position = vec3(50.0, 200.0, 0.0);
	//////////////////////////////////////////////////////////////////////////////

	// Ativando o shader program
	glUseProgram(shaderID);

	// Definindo o uniform do sampler de textura
	glUniform1i(glGetUniformLocation(shaderID, "texBuffer"), 0);

	// left right bottom top near far
	mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	//////////////////////////////////////////////////////////////////////////////
	// Ativando o primeiro buffer de textura da OpenGL
	glActiveTexture(GL_TEXTURE0);

	// Habilitar a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	//////////////////////////////////////////////////////////////////////////////

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawSprite(background, shaderID);
		if (dir == LEFT)
			character.position.x -= vel;
		if (dir == RIGHT)
			character.position.x += vel;
		drawSprite(character, shaderID);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

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

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			cout << "Texture format not supported" << endl;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture: " << filePath << endl;
	}

	return texID;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		dir = LEFT;
	}
	else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		dir = RIGHT;
	}

	if (action == GLFW_RELEASE)
	{
		dir = NONE;
	}
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
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,	// v1
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// v2
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f	// v2
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

	// Atributo 0 - Posição - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - Coordenadas de textura - s, t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Desvincula o VBO e o VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

void drawSprite(Sprite spr, GLuint shaderID)
{
	glBindVertexArray(spr.VAO); // Conectando ao buffer de geometria

	glBindTexture(GL_TEXTURE_2D, spr.texID); // conectando o buffer de textura

	// Matriz de modelo - Tranformações na geometria, nos objetos
	mat4 model = mat4(1); // matriz identidade
	// Translação
	model = translate(model, spr.position);
	// Rotação
	model = rotate(model, radians(spr.angle), vec3(0.0, 0.0, 1.0));
	// Escala
	model = scale(model, spr.dimensions);
	// Enviar para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	// Chamada de desenho - drawcall
	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0); // Desconectando o buffer de geometria
}
