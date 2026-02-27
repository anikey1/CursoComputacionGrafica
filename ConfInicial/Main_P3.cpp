//
// Práctica 3
// 
// Anikey Andrea Gomez Guzman
// 319323290
//Fecha de entrega: 27 de Feb, 2026

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad  
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecciones y transformaciones basicas", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificaci�n de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

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


    // use with Perspective Projection
    // El formato es: PosX, PosY, PosZ, BRILLO, Ignorado, Ignorado
    float vertices[] = {
        // Front face (Laterales - Brillo 0.85)
        -0.5f, -0.5f, 0.5f,  0.85f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,  0.85f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f,  0.85f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.5f,  0.85f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.5f,  0.85f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.85f, 0.0f, 0.0f,

        // Back face (Laterales - Brillo 0.85)
        -0.5f, -0.5f,-0.5f,  0.85f, 1.0f, 0.0f,
         0.5f, -0.5f,-0.5f,  0.85f, 1.0f, 0.0f,
         0.5f,  0.5f,-0.5f,  0.85f, 1.0f, 0.0f,
         0.5f,  0.5f,-0.5f,  0.85f, 1.0f, 0.0f,
        -0.5f,  0.5f,-0.5f,  0.85f, 1.0f, 0.0f,
        -0.5f, -0.5f,-0.5f,  0.85f, 1.0f, 0.0f,

        // Left face (Laterales - Brillo 0.85)
        -0.5f,  0.5f,  0.5f, 0.85f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.85f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.85f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.85f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.85f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.85f, 1.0f, 0.0f,

        // Right face (Laterales - Brillo 0.85)
         0.5f,  0.5f,  0.5f, 0.85f, 0.2f, 0.5f,
         0.5f,  0.5f, -0.5f, 0.85f, 0.2f, 0.5f,
         0.5f, -0.5f, -0.5f, 0.85f, 0.2f, 0.5f,
         0.5f, -0.5f, -0.5f, 0.85f, 0.2f, 0.5f,
         0.5f, -0.5f,  0.5f, 0.85f, 0.2f, 0.5f,
         0.5f,  0.5f,  0.5f, 0.85f, 0.2f, 0.5f,

         // Top face (Superior - Brillo 1.0, la más iluminada)
         -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,

         // Bottom face (Inferior - Brillo 0.6, la más oscura/sombra)
         -0.5f, -0.5f, -0.5f, 0.6f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f, 0.6f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 0.6f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 0.6f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f, 0.6f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 0.6f, 1.0f, 1.0f,
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
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	//glm::mat4 projection=glm::mat4(1);

	//projection = glm::perspective(45.0f, (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        // Usar el shader
        ourShader.Use();
        
        //vista-camaraa
        glm::mat4 view = glm::mat4(1.0f);

        //alejamos toda la escena hacia el fondo(-Z) para que quepa en pantalla,
        // y la bajamos un poco (-Y) para centrar la torre.
        view = glm::translate(view, glm::vec3(0.0f, -4.0f, -35.0f));

        // Rotamos en el eje X positivo. Esto inclina la parte superior de la torre 
        // hacia atrás, simulando perfectamente que la estamos mirando desde abajo.
        view = glm::rotate(view, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // Rotamos en el eje Y para girar la torre y verla en un ángulo diagonal, así apreciamos todas las esquinas y sombras.
        view = glm::rotate(view, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint colorLoc = glGetUniformLocation(ourShader.Program, "pastelColor");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        //  Definir escalas y colores
        float base = 1.0f;
        glm::vec3 scales[6] = {
            glm::vec3(base * 1.0f), glm::vec3(base * 1.5f), glm::vec3(base * 2.0f),
            glm::vec3(base * 2.5f), glm::vec3(base * 3.0f), glm::vec3(base * 4.0f)
        };

        glm::vec3 pastelColors[6] = {
            glm::vec3(0.87f, 0.72f, 0.53f), glm::vec3(0.75f, 0.60f, 0.85f),
            glm::vec3(0.60f, 0.80f, 0.95f), glm::vec3(0.95f, 0.70f, 0.80f),
            glm::vec3(1.00f, 0.90f, 0.60f), glm::vec3(0.60f, 0.90f, 0.60f)
        };

        glm::vec3 currentPos = glm::vec3(0.0f, 0.0f, 0.0f);

        //  Dibujar las cajas
        for (int i = 0; i < 6; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            if (i < 5) {
                // Cajas 1 a 5
                if (i > 0) {
                    float offsetY = (scales[i - 1].y / 2.0f) + (scales[i].y / 2.0f);
                    float offsetX = scales[i - 1].x / 2.0f;
                    float offsetZ = scales[i - 1].z / 2.0f;
                    currentPos += glm::vec3(offsetX, offsetY, offsetZ);
                }

                model = glm::translate(model, currentPos);
                model = glm::rotate(model, glm::radians(15.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));

            }
            else {
                // SEXTA CAJA 
                glm::vec3 topCenter5 = currentPos + glm::vec3(0.0f, scales[4].y / 2.0f, 0.0f);

                // Distancia exacta del centro al vértice
                float distToCorner = (scales[5].y * 1.73205f) / 2.0f;

                // Subimos la caja para que el pico toque el centro exacto de la caja 5
                glm::vec3 pos6 = topCenter5 + glm::vec3(0.0f, distToCorner, 0.0f);
                model = glm::translate(model, pos6);

                //esquina abajo
                model = glm::rotate(model, glm::radians(-54.736f), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            model = glm::scale(model, scales[i]);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            if (colorLoc != -1) {
                glUniform3fv(colorLoc, 1, glm::value_ptr(pastelColors[i]));
            }

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
		 
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;

  

}


