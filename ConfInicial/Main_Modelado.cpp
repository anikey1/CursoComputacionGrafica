//
// Practica 4
// Anikey Andrea Gomez Guzman
// 319323290
// Fecha de entrega: 6 de Marzo, 2026
// Modelado geometrico
//

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=-0.2f;
float movZ=-7.5f;
float rot = 0.0f;
int main() {
	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Anikey Andrea Gomez Guzman", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers

	

	// use with Perspective Projection
	float vertices[] = {
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		//para la 'sensibilidad'del teclado
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.15f, 0.65f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

		GLint colorLoc = glGetUniformLocation(ourShader.Program, "objectColor");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);
		
		//cuerpo (x, y,z)	
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.8f, 0.9f, 1.1f));// tamaño cuerpo
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	 //   model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(3.0f, 0.1f, 2.0f));//anmcho,grosor,profundidad mesa
		//model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//cuelllo
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.6f, 0.23f, 1.1f));// tamaño cuello
		model = glm::translate(model, glm::vec3(-1.0f, 2.0f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// CABEZA
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.4f, 0.75f, 1.1f));// tamaño cabeza
		model = glm::translate(model, glm::vec3(-0.715f, 1.2f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//nariz
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.35f, 0.55f, 0.65f));// tamaño hocico
		//model = glm::translate(model, glm::vec3(-5.2f, 2.0f, 0.0f));// posicion
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// OREJA izq
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.23f, 0.45f, 0.18f));// tamaño oreja
		model = glm::translate(model, glm::vec3(-2.8f, 3.25f,1.5f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.23f, 0.4f, 0.18f));// tamaño oreja
		model = glm::translate(model, glm::vec3(-2.8f, 3.25f, 2.5f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// OREJA derecha
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.23f, 0.45f, 0.18f));// tamaño oreja
		model = glm::translate(model, glm::vec3(-2.8f, 3.25f, -1.5f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.23f, 0.4f, 0.18f));// tamaño oreja
		model = glm::translate(model, glm::vec3(-2.8f, 3.25f, -2.5f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// CRIN SUPERIOR 1
		glUniform3f(colorLoc, 0.90f, 0.45f, 1.00f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.2f, 0.16f, 0.36f));// tamaño crin
		model = glm::translate(model, glm::vec3(-0.75f, 8.5f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// CRIN SUPERIOR 2
		glUniform3f(colorLoc, 0.75f, 0.35f, 1.00f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 0.16f, 0.36f));// tamaño crin
		model = glm::translate(model, glm::vec3(-0.8f, 9.5f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// cuerno
		glUniform3f(colorLoc, 0.85f, 0.75f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.75f, 0.36f));// tamaño cuerno
		model = glm::translate(model, glm::vec3(-3.0f, 2.6f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.22f, 0.36f));// tamaño cuerno
		model = glm::translate(model, glm::vec3(-4.8f, 7.7f, 0.0f));// posicion
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//nariz
		glUniform3f(colorLoc, 0.83f, 0.20f, 0.20f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.7f, 0.9f, 0.3f));// posicion
		model = glm::scale(model, glm::vec3(0.1f, 0.53f, 0.15f));// tamaño hocico
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		glUniform3f(colorLoc, 1.0f, 0.54f, 0.54f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.7f, 0.9, 0.15f));// posicion
		model = glm::scale(model, glm::vec3(0.1f, 0.53f, 0.15f));// tamaño hocico
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.83f, 0.20f, 0.20f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.7f, 0.9f, -0.3f));// posicion
		model = glm::scale(model, glm::vec3(0.1f, 0.53f, 0.15f));// tamaño hocico
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		glUniform3f(colorLoc, 1.0f, 0.54f, 0.54f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.7f, 0.9, -0.15f));// posicion
		model = glm::scale(model, glm::vec3(0.1f, 0.53f, 0.15f));// tamaño hocico
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ojo der
		glUniform3f(colorLoc, 0.05f, 0.05f, 0.96f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9f, 0.95f, 0.6f));// posicion
		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.2f));// tamaño ojo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9f + 0.066f, 0.95f + 0.066f, 0.6f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.21f));     // tamaño ojo interno
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ojo izq
		glUniform3f(colorLoc, 0.05f, 0.05f, 0.96f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9f, 0.95f, -0.6f));// posicion
		model = glm::scale(model, glm::vec3(0.28f, 0.28f, 0.2f));// tamaño ojo
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.9f + 0.066f, 0.95f + 0.066f, -0.6f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.21f));     // tamaño ojo interno
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// PATA DELANTERA DERECHA
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.68f, -0.6f, 0.33f)); // posicion
		model = glm::scale(model, glm::vec3(0.43, 0.45f, 0.43f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// pezuña delantera derecha
		glUniform3f(colorLoc, 1.0f, 0.54f, 0.54f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.68f, -0.9f, 0.33f));
		model = glm::scale(model, glm::vec3(0.43f, 0.18f, 0.43f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// trasera derecha
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.68f, -0.6f, 0.33f)); // posicion
		model = glm::scale(model, glm::vec3(0.43, 0.45f, 0.43f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// pezuña 
		glUniform3f(colorLoc, 1.0f, 0.54f, 0.54f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.68f, -0.9f, 0.33f));
		model = glm::scale(model, glm::vec3(0.43f, 0.18f, 0.43f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// PATA DELANTERA izq
		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.68f, -0.6f, -0.33f)); // posicion
		model = glm::scale(model, glm::vec3(0.43, 0.45f, 0.43f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// pezuña delantera izq
		glUniform3f(colorLoc, 1.0f, 0.54f, 0.54f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.68f, -0.9f, -0.33f));
		model = glm::scale(model, glm::vec3(0.43f, 0.18f, 0.43f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//trasera izq

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.68f, -0.6f, -0.33f)); // posicion
		model = glm::scale(model, glm::vec3(0.43, 0.45f, 0.43f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// pezuña 
		glUniform3f(colorLoc, 1.0f, 0.54f, 0.54f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.68f, -0.9f, -0.33f));
		model = glm::scale(model, glm::vec3(0.43f, 0.18f, 0.43f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ala 1
		glUniform3f(colorLoc, 0.75f, 1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, 0.7f, 0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.18f, 0.5f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.75f, 1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.56f, 1.0f, 0.5f)); // posicion
		model = glm::scale(model, glm::vec3(1.1f, 0.18f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//ala blanco

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.28f, 0.6f, 0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.18f, 0.4f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.58f, 0.85f, 0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.8f, 0.18f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.46f, 0.67f, 0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ala 2
		glUniform3f(colorLoc, 0.75f, 1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.1f, 0.7f, -0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.18f, 0.5f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.75f, 1.0f, 1.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.56f, 1.0f, -0.5f)); // posicion
		model = glm::scale(model, glm::vec3(1.1f, 0.18f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//ala blanco

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.28f, 0.6f, -0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.18f, 0.4f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.58f, 0.85f, -0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.8f, 0.18f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.95f, 0.95f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.46f, 0.67f, -0.5f)); // posicion
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.18f));       // tamaño pata
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// COLA base morada
		glUniform3f(colorLoc, 0.72f, 0.20f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.1f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.18f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.72f, 0.20f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.98f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.6f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.72f, 0.20f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f, 0.56f, 0.0f));
		model = glm::scale(model, glm::vec3(1.35f, 0.18f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.72f, 0.20f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.2f, 0.83f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.85f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.72f, 0.20f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.73f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.65f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// COLA punta rosa
		glUniform3f(colorLoc, 0.90f, 0.35f, 1.00f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.3f, 0.13f, 0.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.18f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.90f, 0.35f, 1.00f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.16f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.6f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glUniform3f(colorLoc, 0.90f, 0.35f, 1.00f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.18f, 0.77f, 0.0f));
		model = glm::scale(model, glm::vec3(0.18f, 0.25f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.90f, 0.35f, 1.00f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.7f, 0.73f, 0.0f));
		model = glm::scale(model, glm::vec3(0.95f, 0.18f, 0.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// PIXELES 
		glUniform3f(colorLoc, 0.82f, 0.48f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.15f - 0.4f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.16f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.82f, 0.48f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.4f-0.5f, 0.10f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.16f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.82f, 0.48f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f - 0.6f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.16f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.82f, 0.48f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.90f - 0.7f, 0.10f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.16f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glUniform3f(colorLoc, 0.82f, 0.48f, 0.95f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.15f - 0.8f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.16f, 0.16f, 1.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);




		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);// se puede llamar varias veces para diferentes objetos, pero es importante desactivar el VAO despues de usarlo para evitar bugs.
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }
 

 void Inputs(GLFWwindow* window)
 {
	 float speed = 2.5f * deltaTime;   // velocidad en unidades/segundo
	 float rotSpeed = 90.0f * deltaTime; // grados por segundo

	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(window, true);

	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += speed;

	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= speed;

	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += speed;

	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= speed;

	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= speed;

	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += speed;

	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += rotSpeed;

	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= rotSpeed;
 }


