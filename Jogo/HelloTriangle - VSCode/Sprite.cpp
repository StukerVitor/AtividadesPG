#include <iostream>
#include "Sprite.h"

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

Sprite::Sprite()
{
	fallSpeed = 1.0f; // Velocidade de queda padrão
}

Sprite::~Sprite()
{
	// Limpar recursos
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::inicializar(GLuint texID, vec3 pos, vec3 escala, float angulo)
{
	this->texID = texID;
	this->pos = pos;
	this->escala = escala;
	this->angulo = angulo;

	// Vertex data
	GLfloat vertices[] = {
		// x     y    z    r    g    b    s    t
		// Triangle 0
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  // v0
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // v1
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	  // v2
		// Triangle 1
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // v1
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  // v3
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f	  // v2
	};

	GLuint VBO;
	// Generate buffers
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

float Sprite::GetXPos()
{
	return pos.x;
}

float Sprite::GetYPos()
{
	return pos.y;
}

void Sprite::desenhar()
{
	// Update transformations and send to shader
	atualizar();

	// Activate shader program
	glUseProgram(shaderID);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO);

	// Draw sprite
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind texture and VAO
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Sprite::moveRight(float offset)
{
	if (pos.x >= 800)
	{
		pos.x = 1;
	}
	else
		pos.x += offset;
}

void Sprite::moveLeft(float offset)
{
	if (pos.x <= 0)
	{
		pos.x = 799;
	}
	else
		pos.x -= offset;
}

void Sprite::cair(bool x)
{
	if (pos.y < 100 || x)
	{
		pos.x = 10.0f + static_cast<float>(rand() % 781);
		pos.y = 1000.0f;
	}
	else
	{
		pos.y -= fallSpeed; // Usar a velocidade de queda definida
	}
}

void Sprite::atualizar()
{
	// Create model matrix and apply transformations
	mat4 model = mat4(1.0f); // Identity matrix
	model = translate(model, pos);
	model = rotate(model, radians(angulo), vec3(0.0f, 0.0f, 1.0f));
	model = scale(model, escala);

	// Activate shader program
	glUseProgram(shaderID);

	// Send 'model' matrix to shader
	GLint modelLoc = glGetUniformLocation(shaderID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
}

void Sprite::switchSide(GLuint texID_Ref)
{
	texID = texID_Ref;
}

void Sprite::finalizar()
{
	// Clean up resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderID);
}

void Sprite::setFallSpeed(float speed)
{
	fallSpeed = speed;
}