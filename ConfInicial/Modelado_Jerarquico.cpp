//
// Practica 5
// Anikey Andrea Gomez Guzman
// 319323290
// Fecha de entrega: 13 de Marzo, 2026
// Modelado Jerarquico
//
#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);
float deltaTime = 0.0f; // Tiempo entre frame actual y anterior
float lastFrame = 0.0f; // Tiempo del frame anterior

const GLint WIDTH = 1200, HEIGHT = 800;

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rot = 0.0f;

//For model
float	hombro = 0.0f,
codo = 0.0f, muneca = 0.0f,
dedo1 = 0.0f, dedo2 = 0.0f,
dedo3 = 0.0f, dedo4 = 0.0f,
dedo5 = 0.0f, dedo6 = 0.0f,
dedo7 = 0.0f, dedo8 = 0.0f,
pulgar1 = 0.0f, pulgar2 = 0.0f;


int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Anikey Andrea Gomez Guzman", nullptr, nullptr);

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
	// use with Orthographic Projection




	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,

		-0.5f, -0.5f,-0.5f,
		 0.5f, -0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		 0.5f,  0.5f,-0.5f,
		-0.5f,  0.5f,-0.5f,
		-0.5f, -0.5f,-0.5f,

		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  -0.5f, 0.5f,

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
		-0.5f,  0.5f, -0.5f,
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


	glm::mat4 projection = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	glm::vec3 color = glm::vec3(0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);
		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp
		glm::mat4 modelTemp2 = glm::mat4(1.0f); //Temp



		//View set up 
		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint uniformColor = ourShader.uniformColor;

		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		glBindVertexArray(VAO);

		//Model 
		model = glm::rotate(model, glm::radians(hombro), glm::vec3(0.0f, 0.0, 1.0f)); //hombro
		modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));//
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f);//color diferente para cada caja 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);//A

		//antebrazo 
		//primero se obtiene la posicion del nuevo modelo a partir del modelo anterior, para que se mantenga la jerarquia
		model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f)); 
		model = glm::rotate(model, glm::radians(-codo),glm::vec3(0.0f,1.0f,0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f); 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Palma
		model = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(muneca), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp2 =modelTemp= model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Dedos

		// Indice
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.375f));
		model = glm::rotate(model, glm::radians(-dedo1), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.50f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.00f, 0.20f, 0.18f));
		color = glm::vec3(0.0f, 0.8f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(modelTemp, glm::vec3(0.50f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-dedo2), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.70f, 0.18f, 0.16f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Medio
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.125f));
		model = glm::rotate(model, glm::radians(-dedo3), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.58f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.15f, 0.20f, 0.18f));
		color = glm::vec3(0.0f, 0.8f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(modelTemp, glm::vec3(0.58f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-dedo4), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.40f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.80f, 0.18f, 0.16f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Anular
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.125f));
		model = glm::rotate(model, glm::radians(-dedo5), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.52f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.05f, 0.20f, 0.18f));
		color = glm::vec3(0.0f, 0.8f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(modelTemp, glm::vec3(0.52f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-dedo6), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.36f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.72f, 0.18f, 0.16f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Meñique
		model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.375f));
		model = glm::rotate(model, glm::radians(-dedo7), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.42f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.85f, 0.20f, 0.18f));
		color = glm::vec3(0.0f, 0.8f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(modelTemp, glm::vec3(0.42f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-dedo8), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.28f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.56f, 0.18f, 0.16f));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Pulgar
		model = glm::translate(modelTemp2, glm::vec3(0.10f, -0.42f, 0.42f));
		model = glm::rotate(model, glm::radians(-35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(pulgar1), glm::vec3(0.0f, 1.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.30f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.60f, 0.22f, 0.20f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::translate(modelTemp, glm::vec3(0.30f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(pulgar2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.50f, 0.20f, 0.18f));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


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
	float speed = 2.5f * deltaTime;
	float rotSpeed = 90.0f * deltaTime;

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

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && hombro < 90.0f)
		hombro += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && hombro > -90.0f)
		hombro -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && codo < 120.0f)
		codo += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && codo > 0.0f)
		codo -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && muneca < 60.0f)
		muneca += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && muneca > -60.0f)
		muneca -= rotSpeed;

	// dedo 1
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && dedo1 < 90.0f)
		dedo1 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && dedo1 > 0.0f)
		dedo1 -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && dedo2 < 90.0f)
		dedo2 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && dedo2 > 0.0f)
		dedo2 -= rotSpeed;

	// dedo 2
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && dedo3 < 90.0f)
		dedo3 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && dedo3 > 0.0f)
		dedo3 -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && dedo4 < 90.0f)
		dedo4 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && dedo4 > 0.0f)
		dedo4 -= rotSpeed;

	// dedo 3
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && dedo5 < 90.0f)
		dedo5 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && dedo5 > 0.0f)
		dedo5 -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && dedo6 < 90.0f)
		dedo6 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && dedo6 > 0.0f)
		dedo6 -= rotSpeed;

	// dedo 4
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && dedo7 < 90.0f)
		dedo7 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && dedo7 > 0.0f)
		dedo7 -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && dedo8 < 90.0f)
		dedo8 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && dedo8 > 0.0f)
		dedo8 -= rotSpeed;

	// pulgar
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && pulgar1 < 60.0f)
		pulgar1 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && pulgar1 > -20.0f)
		pulgar1 -= rotSpeed;

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && pulgar2 < 80.0f)
		pulgar2 += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && pulgar2 > 0.0f)
		pulgar2 -= rotSpeed;
}

