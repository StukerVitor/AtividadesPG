// Sprite.h

#pragma once

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Sprite
{
public:
	Sprite(); // Constructor declaration
	~Sprite();
	void inicializar(GLuint texID, vec3 pos = vec3(0.0, 0.0, 0.0), vec3 escala = vec3(1.0, 1.0, 1.0), float angulo = 0.0);
	float GetXPos();
	float GetYPos();
	void desenhar();
	void moveRight(float offset);
	void moveLeft(float offset);
	void cair(bool reset);
	void switchSide(GLuint texID_Ref);
	void finalizar();
	void setFallSpeed(float speed); // Método para definir a velocidade de queda

	// Métodos getters
	vec3 GetPosition() const { return pos; }
	vec3 GetScale() const { return escala; }

	// Métodos setters
	void SetPosition(const vec3 &newPos) { pos = newPos; }
	void SetScale(const vec3 &newScale) { escala = newScale; }

	// Shader
	inline void setShader(GLuint shaderID) { this->shaderID = shaderID; }

protected:
	void atualizar();

	GLuint VAO;		 // Vertex Array Object identifier
	GLuint texID;	 // Texture buffer identifier
	GLuint shaderID; // Shader program identifier

	// Variables for object transformations
	vec3 pos, escala;
	float angulo;
	float fallSpeed; // Variável para armazenar a velocidade de queda
};
