#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

// Classe para manipulação das sprites
#include "Sprite.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Protótipos das funções
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
GLuint loadTexture(string texturePath);
bool colisao(float perX, float perY, float widthPer, float heightPer, float obsX, float obsY, float widthObs, float heightObs);
GLuint setupShader();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Variáveis globais
Sprite coelho;
Sprite startButton;
Sprite playAgainButton;
Sprite exitButton;
Sprite placar;
Sprite placarZero;
int gmOver = 0;	   // Variável para impedir movimentos após game over
int pontos = 0;	   // Variável para contar quantidade de cenouras coletadas e encerrar execução
int quantPont = 3; // Variável para limitar a quantidade de cenouras a serem coletadas, valor de 1 - 3
int gameState = 0; // 0: Menu, 1: Jogando, 2: Game Over, 3: Vitória
bool win = false;

// Vertex Shader source code
const GLchar *vertexShaderSource = R"glsl(
    #version 400 core
    layout(location = 0) in vec3 aPos;     // Vertex position
    layout(location = 1) in vec3 aColor;   // Vertex color
    layout(location = 2) in vec2 aTexCoord; // Texture coordinate

    out vec3 ourColor;    // Output color to fragment shader
    out vec2 TexCoord;    // Output texture coordinate

    uniform mat4 model;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * model * vec4(aPos, 1.0);
        ourColor = aColor;
        TexCoord = aTexCoord;
    }
)glsl";

// Fragment Shader source code
const GLchar *fragmentShaderSource = R"glsl(
    #version 400 core
    out vec4 FragColor;

    in vec3 ourColor;
    in vec2 TexCoord;

    uniform sampler2D texBuffer;

    void main()
    {
        FragColor = texture(texBuffer, TexCoord) * vec4(ourColor, 1.0);
    }
)glsl";

int main()
{
	// Inicialização da GLFW
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return -1;
	}

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "##### Rabbit Game #####", nullptr, nullptr);
	if (!window)
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

	stbi_set_flip_vertically_on_load(true);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); // a cada ciclo

	// Habilitar o modo de transparência (blend - mistura de cores)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Ativando o buffer de textura 0 da OpenGL
	glActiveTexture(GL_TEXTURE0);

	// Matriz de projeção paralela ortográfica
	mat4 projection = ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

	// Variáveis para controle do tempo
	float deltaTime = 0.0f; // Tempo entre o quadro atual e o anterior
	float lastFrame = 0.0f; // Tempo do último quadro

	// Carregando texturas
	GLuint texID_backGround = loadTexture("../Textures/PixelArt/BackGround/nature_1/orig.png");
	GLuint texID_espada_1 = loadTexture("../Textures/craftpix/PNG/Transperent/espada.png");
	GLuint texID_gameOver = loadTexture("../Textures/gameOver/gameOver.png");
	GLuint texID_vitoria = loadTexture("../Textures/gameOver/vitoria.png");
	GLuint texID_coelhoR = loadTexture("../Textures/Coelho/coelhoRight.png");
	GLuint texID_coelhoL = loadTexture("../Textures/Coelho/coelhoLeft.png");
	GLuint texID_cenoura = loadTexture("../Textures/Cenoura/cenoura.png");
	GLuint texID_placar_zero = loadTexture("../Textures/Placar/0.png");
	GLuint texID_placar_um = loadTexture("../Textures/Placar/1.png");
	GLuint texID_placar_dois = loadTexture("../Textures/Placar/2.png");
	GLuint texID_placar_tres = loadTexture("../Textures/Placar/3.png");
	// Carregar texturas dos botões
	GLuint texID_startButton = loadTexture("../Textures/Botoes/start.png");
	GLuint texID_playAgainButton = loadTexture("../Textures/Botoes/playAgain.png");
	GLuint texID_exitButton = loadTexture("../Textures/Botoes/exit.png");

	// Criação das sprites
	Sprite background;
	background.setShader(shaderID);
	background.inicializar(texID_backGround, vec3(400.0f, 300.0f, 0.0f), vec3(1920.0f / 2.0f, 1200.0f / 2.0f, 1.0f));

	Sprite espada_1;
	espada_1.setShader(shaderID);
	espada_1.inicializar(texID_espada_1, vec3(400.0f, 900.0f, 0.0f), vec3(32.0f, 32.0f, 1.0f));

	Sprite espada_2;
	espada_2.setShader(shaderID);
	espada_2.inicializar(texID_espada_1, vec3(600.0f, 1080.0f, 0.0f), vec3(32.0f, 32.0f, 1.0f));

	Sprite cenoura;
	cenoura.setShader(shaderID);
	cenoura.inicializar(texID_cenoura, vec3(200.0f, 1200.0f, 0.0f), vec3(45.0f, 52.0f, 1.0f));

	coelho.setShader(shaderID);
	coelho.inicializar(texID_coelhoR, vec3(300.0f, 120.0f, 0.0f), vec3(65.0f, 70.0f, 1.0f));

	Sprite gameOver;
	gameOver.setShader(shaderID);
	gameOver.inicializar(texID_gameOver, vec3(400.0f, 300.0f, 0.0f), vec3(350.0f, 205.0f, 1.0f));

	Sprite vitoria;
	vitoria.setShader(shaderID);
	vitoria.inicializar(texID_vitoria, vec3(400.0f, 300.0f, 0.0f), vec3(350.0f, 205.0f, 1.0f));

	placar.setShader(shaderID);
	placar.inicializar(texID_placar_zero, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	placarZero.setShader(shaderID);
	placarZero.inicializar(texID_placar_zero, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	Sprite placarUm;
	placarUm.setShader(shaderID);
	placarUm.inicializar(texID_placar_um, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	Sprite placarDois;
	placarDois.setShader(shaderID);
	placarDois.inicializar(texID_placar_dois, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	Sprite placarTres;
	placarTres.setShader(shaderID);
	placarTres.inicializar(texID_placar_tres, vec3(790.0f, 550.0f, 0.0f), vec3(60.0f, 82.0f, 1.0f));

	// Botão Iniciar
	startButton.setShader(shaderID);
	startButton.inicializar(texID_startButton, vec3(400.0f, 300.0f, 0.0f), vec3(212.0f, 78.0f, 1.0f));

	// Botão Jogar Novamente
	playAgainButton.setShader(shaderID);
	playAgainButton.inicializar(texID_playAgainButton, vec3(200.0f, 50.0f, 0.0f), vec3(200.0f, 80.0f, 1.0f));

	// Botão Sair
	exitButton.setShader(shaderID);
	exitButton.inicializar(texID_exitButton, vec3(600.0f, 50.0f, 0.0f), vec3(200.0f, 80.0f, 1.0f));

	// Definir velocidades de queda
	espada_1.setFallSpeed(0.2f); // Velocidade mais lenta para a espada 1
	espada_2.setFallSpeed(0.2f); // Velocidade mais lenta para a espada 2
	cenoura.setFallSpeed(0.1f);	 // Velocidade mais lenta para o limão

	// Ativando o programa de shader
	glUseProgram(shaderID);

	// Enviando a matriz de projeção para o shader
	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	// Configurando o sampler de textura
	GLint texBufferLoc = glGetUniformLocation(shaderID, "texBuffer");
	glUniform1i(texBufferLoc, 0);

	bool win = false;

	glfwPollEvents();

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Calcular o delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (gameState == 1 && (gmOver == 0) && (pontos < quantPont))
		{
			float coelhoSpeed = 200.0f; // Velocidade em unidades por segundo

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				coelho.switchSide(texID_coelhoR);
				coelho.moveRight(coelhoSpeed * deltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				coelho.switchSide(texID_coelhoL);
				coelho.moveLeft(coelhoSpeed * deltaTime);
			}
		}

		// Definindo as dimensões da viewport
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Limpa o buffer de cor
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderID);

		// Renderizar com base no estado do jogo
		if (gameState == 0)
		{
			// Tela de Menu
			background.desenhar();
			startButton.desenhar();
		}
		else if (gameState == 1)
		{
			// Jogo ativo
			background.desenhar();
			espada_1.desenhar();
			espada_2.desenhar();
			cenoura.desenhar();
			placar.desenhar();
			coelho.desenhar();

			// Atualizar lógica do jogo
			if ((gmOver == 0) && (pontos < quantPont))
			{
				espada_1.cair(false);
				espada_2.cair(false);
				cenoura.cair(false);
			}

			// Detecção de colisão
			bool auxespada_1 = colisao(coelho.GetXPos(), coelho.GetYPos(), 65.0f, 70.0f,
									   espada_1.GetXPos(), espada_1.GetYPos(), 32.0f, 32.0f);
			bool auxespada_2 = colisao(coelho.GetXPos(), coelho.GetYPos(), 65.0f, 70.0f,
									   espada_2.GetXPos(), espada_2.GetYPos(), 32.0f, 32.0f);
			bool aux_cenoura = colisao(coelho.GetXPos(), coelho.GetYPos(), 65.0f, 70.0f,
									   cenoura.GetXPos(), cenoura.GetYPos(), 45.0f, 52.0f);

			if (aux_cenoura)
			{
				pontos++;
				// Atualizar o placar
				switch (pontos)
				{
				case 1:
					placar = placarUm;
					break;
				case 2:
					placar = placarDois;
					break;
				case 3:
					placar = placarTres;
					break;
				}

				if (pontos >= quantPont)
				{
					gameState = 3; // Vitória
					win = true;
				}
				else
					cenoura.cair(true);
			}
			if (auxespada_1 || auxespada_2)
			{
				gmOver = 1;
				gameState = 2; // Game Over
			}
		}
		else if (gameState == 2)
		{
			// Tela de Game Over
			background.desenhar();
			gameOver.desenhar();
			playAgainButton.desenhar();
			exitButton.desenhar();
		}
		else if (gameState == 3)
		{
			// Tela de Vitória
			background.desenhar();
			vitoria.desenhar();
			playAgainButton.desenhar();
			exitButton.desenhar();
		}

		glfwSwapBuffers(window);
	}

	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

bool isButtonClicked(double mouseX, double mouseY, Sprite &button)
{
	float xmin = button.GetXPos() - button.GetScale().x / 2.0f;
	float xmax = button.GetXPos() + button.GetScale().x / 2.0f;
	float ymin = button.GetYPos() - button.GetScale().y / 2.0f;
	float ymax = button.GetYPos() + button.GetScale().y / 2.0f;

	return mouseX >= xmin && mouseX <= xmax && mouseY >= ymin && mouseY <= ymax;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	// Obter posição do cursor
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Ajustar para coordenadas do OpenGL (origem no canto inferior esquerdo)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ypos = height - ypos;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (gameState == 0)
		{
			// Verificar se o botão iniciar foi clicado
			if (isButtonClicked(xpos, ypos, startButton))
			{
				gameState = 1; // Iniciar o jogo
			}
		}
		else if (gameState == 2 || gameState == 3)
		{
			// Verificar se o botão jogar novamente foi clicado
			if (isButtonClicked(xpos, ypos, playAgainButton))
			{
				// Resetar variáveis do jogo
				pontos = 0;
				gmOver = 0;
				win = false;
				coelho.SetPosition(vec3(300.0f, 120.0f, 0.0f));
				// Resetar posições de outros sprites, se necessário
				placar = placarZero;
				gameState = 1; // Voltar a jogar
			}
			// Verificar se o botão sair foi clicado
			else if (isButtonClicked(xpos, ypos, exitButton))
			{
				glfwSetWindowShouldClose(window, GL_TRUE); // Fechar o jogo
			}
		}
	}
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - é chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

bool colisao(float perX, float perY, float widthPer, float heightPer, float obsX, float obsY, float widthObs, float heightObs)
{
	// Pontos Personagem
	float xmaxPer = perX + widthPer / 3.0f;
	float ymaxPer = perY + heightPer / 3.0f;
	float xminPer = perX - widthPer / 3.0f;
	float yminPer = perY - heightPer / 3.0f;

	// Pontos Obstáculo
	float xmaxObs = obsX + widthObs / 3.0f;
	float ymaxObs = obsY + heightObs / 3.0f;
	float xminObs = obsX - widthObs / 3.0f;
	float yminObs = obsY - heightObs / 3.0f;

	// Testa colisões
	bool collisionX = xmaxPer >= xminObs &&
					  xmaxObs >= xminPer;
	bool collisionY = ymaxPer >= yminObs &&
					  ymaxObs >= yminPer;

	return collisionX && collisionY;
}

GLuint loadTexture(string texturePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Configuração do parâmetro WRAPPING nas coords s e t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Configuração do parâmetro FILTERING na minificação e magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	// Carregamento da imagem da texture
	if (data)
	{
		if (nrChannels == 3) // jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture: " << texturePath << endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

GLuint setupShader()
{
	// 1. Compilar os shaders
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Checar por erros de compilação
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			 << infoLog << endl;
	}

	// Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checar por erros de compilação
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			 << infoLog << endl;
	}

	// 2. Linkar os shaders
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Checar por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
			 << infoLog << endl;
	}

	// Deletar os shaders após terem sido linkados
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
