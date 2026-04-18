//Practica 9 - Fuentes de Luz
//Anikey Andrea Gomez Guzman 
//319323290

#include <iostream>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active = false;
bool reflectorActive = false;
float fogataIntensidad = 1.0f; // L = leve, K = intensa
bool linternaActive = true; // empieza encendida

glm::vec3 pointLightPositions[] = {
    glm::vec3(1.0f, 0.0f, 1.0f),   // fogata
    glm::vec3(-0.8f, 0.8f, -1.4f), // ventana izq
    glm::vec3(0.9f, 0.8f, -1.4f),  // ventana der
    glm::vec3(1.6f, 1.0f, -5.5f)   // ventana lateral
};

float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f
};

glm::vec3 Light1 = glm::vec3(0);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Anikey Andrea Gomez Guzman", nullptr, nullptr);
    if (!window) { glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) return EXIT_FAILURE;

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    std::cout << "Cargando modelos..." << std::endl;
    Model Perro((char*)"Models/Perro/RedDog.obj");   std::cout << "Perro OK" << std::endl;
    Model Piso((char*)"Models/piso/piso.obj");        std::cout << "Piso OK" << std::endl;
    Model house((char*)"Models/House/model.obj");     std::cout << "House OK" << std::endl;
    Model Fogata((char*)"Models/Fogata/fogata.obj");  std::cout << "Fogata OK" << std::endl;
    Model lunaModel((char*)"Models/Luna/Luna.obj");   std::cout << "Luna OK" << std::endl;
    Model lamp((char*)"Models/lamp/lamp.obj");        std::cout << "Lamp OK" << std::endl;
    Model stuff((char*)"Models/stuff/model.obj");     std::cout << "Stuff OK" << std::endl;
    Model arbol((char*)"Models/arb/model.obj");       std::cout << "Arbol OK" << std::endl;
    std::cout << "Todos cargados!" << std::endl;

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // fondo noche azul muy oscuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        //  Luna azul fría 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.15f, 0.15f, 0.25f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.3f);

        // ─── POINT LIGHT [0] — Fogata (ESPACIO toggle, K intensa, L leve) ─
        glm::vec3 lightColor(0.0f);
        if (active)
        {
            float flicker = 0.7f + 0.15f * sin(glfwGetTime() * 7.3f)
                + 0.10f * sin(glfwGetTime() * 13.7f);
            if (fogataIntensidad > 1.0f)
            {
                // Intensa — naranja rojizo
                lightColor.x = 0.9f * flicker * fogataIntensidad;  // rojo fuerte
                lightColor.y = 0.25f * flicker * fogataIntensidad;  // poco verde
                lightColor.z = 0.02f * flicker;                      // casi nada azul
            }
            else
            {
                // Leve — amarillo cálido
                lightColor.x = 0.8f * flicker * fogataIntensidad;
                lightColor.y = 0.6f * flicker * fogataIntensidad;   // más verde = más amarillo
                lightColor.z = 0.05f * flicker;
            }
        }
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            lightColor.x * 0.3f, lightColor.y * 0.3f, lightColor.z * 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            lightColor.x * 0.5f, lightColor.y * 0.2f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.05f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.02f);

        //Ventana frente izquierda
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), -0.8f, 0.8f, -1.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.1f, 0.08f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.7f, 0.5f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.3f, 0.2f, 0.05f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.4f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.3f);

        // Ventana frente derecha 
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), 0.9f, 0.8f, -1.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.1f, 0.08f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.8f, 0.5f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.3f, 0.2f, 0.05f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.4f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.3f);

        //  Ventana lateral
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), 1.6f, 1.0f, -5.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.1f, 0.08f, 0.01f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.7f, 0.5f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.3f, 0.2f, 0.05f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.4f);

         
        // Linterna cámara azul (O toggle)
        glm::vec3 lintAmb(0.0f), lintDiff(0.0f);
        if (linternaActive) { lintAmb = glm::vec3(0.2f, 0.2f, 0.8f); lintDiff = glm::vec3(0.2f, 0.2f, 0.8f); }
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), lintAmb.x, lintAmb.y, lintAmb.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), lintDiff.x, lintDiff.y, lintDiff.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        // reflector de perrito
        glm::vec3 refAmb(0.0f), refDiff(0.0f), refSpec(0.0f);
        if (reflectorActive)
        {
            refAmb = glm::vec3(0.1f, 0.1f, 0.1f);
            refDiff = glm::vec3(0.9f, 0.85f, 0.7f);
            refSpec = glm::vec3(1.0f, 0.95f, 0.8f);
        }
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight2.position"), 0.0f, 3.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight2.direction"), 0.0f, -1.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight2.ambient"), refAmb.x, refAmb.y, refAmb.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight2.diffuse"), refDiff.x, refDiff.y, refDiff.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight2.specular"), refSpec.x, refSpec.y, refSpec.z);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight2.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight2.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight2.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight2.cutOff"), glm::cos(glm::radians(8.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight2.outerCutOff"), glm::cos(glm::radians(14.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // lampara

        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight3.position"), 5.0f, 1.8f, 2.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight3.direction"), 0.0f, -1.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight3.ambient"), 0.3f, 0.25f, 0.05f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight3.diffuse"), 0.8f, 0.4f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight3.specular"), 0.4f, 0.3f, 0.1f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight3.constant"), 0.8f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight3.linear"), 0.1f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight3.quadratic"), 0.05f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight3.cutOff"), glm::cos(glm::radians(25.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight3.outerCutOff"), glm::cos(glm::radians(35.0f)));

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);

        // Piso
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 3.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Perrito
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        Perro.Draw(lightingShader);
        glDisable(GL_BLEND);

        // Fogata
        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Fogata.Draw(lightingShader);

        //// banca
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.0f, -0.4f, 1.0f));
        model = glm::scale(model, glm::vec3(0.035f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        stuff.Draw(lightingShader);

        // Cabaña
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.25f, -4.0f));
        model = glm::scale(model, glm::vec3(0.004f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        house.Draw(lightingShader);

        // Árbol
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.5f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.09f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        // Lámpara 
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.0f, -0.4f, 2.5f));
        model = glm::scale(model, glm::vec3(0.15f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        lamp.Draw(lightingShader);

        // Luna
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, 8.0f, -10.0f));
        model = glm::scale(model, glm::vec3(0.8f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        lunaModel.Draw(lightingShader);

        // Lamp cubes — solo [1],[2],[3], NO dibuja cubo en fogata
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        for (GLuint i = 1; i < 1; i++) // empieza en 1 — fogata no necesita cubo
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.05f)); // muy pequeño, casi invisible
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.01f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)   keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // ESPACIO — toggle fogata
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        Light1 = active ? glm::vec3(1.0f, 1.0f, 0.0f) : glm::vec3(0.0f);
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)//linterna 
        linternaActive = !linternaActive;
    // F — toggle reflector perrito
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        reflectorActive = !reflectorActive;

    // K — fogata intensa / L — fogata leve
    if (key == GLFW_KEY_K && action == GLFW_PRESS) fogataIntensidad = 3.0f;
    if (key == GLFW_KEY_L && action == GLFW_PRESS) fogataIntensidad = 0.8f;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}