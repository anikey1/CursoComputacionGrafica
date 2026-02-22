//Practica 2
//Anikey Andrea Gomez Guzman
// 319323290
//Fecha de entrega: 20 de Feb, 2026



#include <iostream>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D - Tortuga Mejorada", NULL, NULL);
    glfwSetFramebufferSizeCallback(window, resize);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    
    float vertices[] = {
        // --- Cabeza (0-5) ---
         0.00f,  0.75f, 0.0f,   0,0,0, // 0: Punta arriba
        -0.12f,  0.60f, 0.0f,   0,0,0, // 1: Arriba izq
         0.12f,  0.60f, 0.0f,   0,0,0, // 2: Arriba der
        -0.08f,  0.40f, 0.0f,   0,0,0, // 3: Base izq
         0.08f,  0.40f, 0.0f,   0,0,0, // 4: Base der
         0.00f,  0.48f, 0.0f,   0,0,0, // 5: V interna del cuello

         // --- Caparazón: Hexágono Central (6-11) ---
         -0.20f,  0.15f, 0.0f,   0,0,0, // 6: Centro Arriba Izq
          0.20f,  0.15f, 0.0f,   0,0,0, // 7: Centro Arriba Der
          0.30f, -0.05f, 0.0f,   0,0,0, // 8: Centro Der
          0.20f, -0.25f, 0.0f,   0,0,0, // 9: Centro Abajo Der
         -0.20f, -0.25f, 0.0f,   0,0,0, // 10: Centro Abajo Izq
         -0.30f, -0.05f, 0.0f,   0,0,0, // 11: Centro Izq

         // --- Caparazón: Anillo Interno (12-15) ---
         -0.15f,  0.35f, 0.0f,   0,0,0, // 12: Interno Arriba Izq
          0.15f,  0.35f, 0.0f,   0,0,0, // 13: Interno Arriba Der
         -0.15f, -0.35f, 0.0f,   0,0,0, // 14: Interno Abajo Izq
          0.15f, -0.35f, 0.0f,   0,0,0, // 15: Interno Abajo Der

          // --- Caparazón: Perímetro Exterior (16-21) ---
          -0.35f,  0.35f, 0.0f,   0,0,0, // 16: Exterior Arriba Izq
           0.35f,  0.35f, 0.0f,   0,0,0, // 17: Exterior Arriba Der
          -0.45f, -0.05f, 0.0f,   0,0,0, // 18: Exterior Medio Izq
           0.45f, -0.05f, 0.0f,   0,0,0, // 19: Exterior Medio Der
          -0.30f, -0.40f, 0.0f,   0,0,0, // 20: Exterior Abajo Izq
           0.30f, -0.40f, 0.0f,   0,0,0, // 21: Exterior Abajo Der

           // --- Aleta Izquierda (22-26) ---
           -0.41f,  0.11f, 0.0f,   0,0,0, // 22: Raíz inferior
           -0.65f,  0.30f, 0.0f,   0,0,0, // 23: Codo superior
           -0.80f,  0.10f, 0.0f,   0,0,0, // 24: Codo medio
           -0.90f, -0.15f, 0.0f,   0,0,0, // 25: Punta
           -0.60f,  0.05f, 0.0f,   0,0,0, // 26: Articulación interna

           // --- Aleta Derecha (27-31) ---
            0.41f,  0.11f, 0.0f,   0,0,0, // 27: Raíz inferior
            0.65f,  0.30f, 0.0f,   0,0,0, // 28: Codo superior
            0.80f,  0.10f, 0.0f,   0,0,0, // 29: Codo medio
            0.90f, -0.15f, 0.0f,   0,0,0, // 30: Punta
            0.60f,  0.05f, 0.0f,   0,0,0, // 31: Articulación interna

            // --- Pata Izquierda (32-35) ---
            -0.40f, -0.55f, 0.0f,   0,0,0, // 32: Borde exterior
            -0.35f, -0.70f, 0.0f,   0,0,0, // 33: Punta exterior
            -0.20f, -0.70f, 0.0f,   0,0,0, // 34: Punta interna
            -0.10f, -0.50f, 0.0f,   0,0,0, // 35: Borde interno

            // --- Pata Derecha (36-39) ---
             0.40f, -0.55f, 0.0f,   0,0,0, // 36: Borde exterior
             0.35f, -0.70f, 0.0f,   0,0,0, // 37: Punta exterior
             0.20f, -0.70f, 0.0f,   0,0,0, // 38: Punta interna
             0.10f, -0.50f, 0.0f,   0,0,0, // 39: Borde interno

             // --- Cola (40-42) ---
             -0.08f, -0.35f, 0.0f,   0,0,0, // 40: Base Izq
              0.08f, -0.35f, 0.0f,   0,0,0, // 41: Base Der
              0.00f, -0.50f, 0.0f,   0,0,0, // 42: Punta
    };

    // =========================
    // ÍNDICES PARA GL_LINES
    // =========================
    unsigned int indices[] = {
        // Cabeza
        0,1, 1,3, 3,5, 5,4, 4,2, 2,0,

        // Caparazón - Hexágono Central
        6,7, 7,8, 8,9, 9,10, 10,11, 11,6,

        // Caparazón - Trapecios superior e inferior
        12,13, 12,6, 13,7,
        14,15, 14,10, 15,9,

        // Caparazón - Perímetro Exterior (incorporando la base de las aletas)
        3,16, 16,22, 22,18, 18,20, 20,14, // Lado Izquierdo
        4,17, 17,27, 27,19, 19,21, 21,15, // Lado Derecho

        // Caparazón - Líneas de tensión (conectan el centro con el exterior)
        16,12, 18,11, 20,10, // Izquierda
        17,13, 19,8, 21,9,   // Derecha

        // Aleta Izquierda (Contorno + Geometría interna)
        16,23, 23,24, 24,25, 25,26, 26,22, // Contorno exterior
        16,26, 23,26, 24,26,               // Pliegues internos

        // Aleta Derecha (Contorno + Geometría interna)
        17,28, 28,29, 29,30, 30,31, 31,27, // Contorno exterior
        17,31, 28,31, 29,31,               // Pliegues internos

        // Pata Izquierda (Contorno + Geometría interna)
        20,32, 32,33, 33,34, 34,35, 35,14, // Contorno exterior
        20,35, 32,34,                      // Pliegues internos

        // Pata Derecha (Contorno + Geometría interna)
        21,36, 36,37, 37,38, 38,39, 39,15, // Contorno exterior
        21,39, 36,38,                      // Pliegues internos

        // Cola
        40,42, 42,41
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Habilitar anti-aliasing de líneas para que se vean suaves
    glEnable(GL_LINE_SMOOTH);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);

        // Hacer las líneas ligeramente más gruesas (opcional, ayuda a que resalte)
        glLineWidth(2.5f);

        glDrawElements(GL_LINES, (GLsizei)(sizeof(indices) / sizeof(indices[0])), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}