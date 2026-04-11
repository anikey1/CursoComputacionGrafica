// Practica 8
// Anikey Andrea Gomez Guzman
// 319323290
// Fecha de entrega: 10 de abril, 2026
// Iluminacion: Modo Dia/Noche con trayectoria orbital de Sol y Luna

// Std. Includes
#include <string>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(2.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


bool mododia = true;
float anguloOrbita = 90.0f;       // Sol empieza arriba
const float RADIO_ORBITA = 4.0f;

// Segunda fuente de luz (desplazable con J/K)
glm::vec3 lightPos2(-1.5f, 2.0f, -1.5f);
float movelightPos2 = 0.0f;


glm::vec3 posEnOrbita(float grados)
{
    float rad = glm::radians(grados);
    return glm::vec3(RADIO_ORBITA * cos(rad),
        RADIO_ORBITA * sin(rad),
        0.0f);
}

int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT,
        "Practica 8 - Anikey Andrea Gomez Guzman", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // -- Shaders --------------------------------------------------
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");

    // -- Modelos de escena ----------------------------------------
    Model dog((char*)"Models/Perro/RedDog.obj");
    Model sofa((char*)"Models/Sofa/Sofa.obj");
    Model pizza((char*)"Models/Pizza/Pizza.obj");
    Model control((char*)"Models/Control/Control.obj");
    Model glass((char*)"Models/Glass/Glass.obj");
    Model lentes((char*)"Models/Lentes/Lentes.obj");
    Model mesa((char*)"Models/Mesa/Mesa.obj");


    Model solModel((char*)"Models/Sol/Sol.obj");
    Model lunaModel((char*)"Models/Luna/Luna.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // -- Posiciones orbitales ---------------------------------
        glm::vec3 posSol = posEnOrbita(anguloOrbita);
        glm::vec3 posLuna = posEnOrbita(anguloOrbita + 180.0f);

        // t=1 dia pleno / t=0 noche plena — transicion gradual con Q/E
        float t = glm::clamp((posSol.y + RADIO_ORBITA) / (2.0f * RADIO_ORBITA), 0.0f, 1.0f);
        float tLuna = 1.0f - t;

        // -- Fondo: interpola azul cielo <-> azul noche -----------
        glm::vec3 bg = glm::mix(glm::vec3(0.02f, 0.02f, 0.10f),
            glm::vec3(0.53f, 0.81f, 0.98f), t);
        glClearColor(bg.r, bg.g, bg.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       
        lightingShader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z);

        // Luz 1: Sol — calida, amarilla, se atenua al bajar
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"),
            posSol.x, posSol.y, posSol.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"),
            0.40f * t, 0.35f * t, 0.20f * t);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"),
            0.90f * t, 0.85f * t, 0.55f * t);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"),
            1.00f * t, 0.95f * t, 0.80f * t);

        // Luz 2: Luna — blanca fria, se activa cuando sube la luna
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"),
            posLuna.x, posLuna.y, posLuna.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"),
            0.10f * tLuna, 0.10f * tLuna, 0.20f * tLuna);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"),
            0.45f * tLuna, 0.47f * tLuna, 0.70f * tLuna);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"),
            0.60f * tLuna, 0.62f * tLuna, 0.90f * tLuna);

        // Material de los modelos de escena
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.8f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // -- Dibuja modelos de escena -----------------------------

        // PERRO
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, 0.0f, -1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // SOFA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.0f));
        model = glm::scale(model, glm::vec3(1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        sofa.Draw(lightingShader);

        // PIZZA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.35f, -0.37f, -0.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(3.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 0.1f));
        model = glm::scale(model, glm::vec3(0.025f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        pizza.Draw(lightingShader);

        // CONTROL
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -0.37f, -0.52f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(3.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 0.1f));
        model = glm::scale(model, glm::vec3(0.04f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        control.Draw(lightingShader);

        // VASO
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.35f, -0.3f, -0.8f));
        model = glm::scale(model, glm::vec3(0.01f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glass.Draw(lightingShader);

        // LENTES
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.8f, 0.15f, -0.69f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(3.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.01f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.015f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        lentes.Draw(lightingShader);

        // MESA
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, -0.9f, 4.0f));
        model = glm::scale(model, glm::vec3(0.022f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        mesa.Draw(lightingShader);

  
        lightingShader.Use();  
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"),
            1, GL_FALSE, glm::value_ptr(view));

        // SOL
        model = glm::mat4(1.0f);
        model = glm::translate(model, posSol);
        model = glm::scale(model, glm::vec3(1.1f));
        model = glm::translate(model, glm::vec3(0.0f, -0.72f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        solModel.Draw(lightingShader);  // <-- lightingShader

        // LUNA
        model = glm::mat4(1.0f);
        model = glm::translate(model, posLuna);
        model = glm::scale(model, glm::vec3(0.18f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        lunaModel.Draw(lightingShader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)        keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // N: salto rapido dia/noche
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        mododia = !mododia;
        anguloOrbita = mododia ? 90.0f : 270.0f;
        std::cout << (mododia ? "[DIA]" : "[NOCHE]") << std::endl;
    }

    // E/Q: rotan la orbita gradualmente (la luz cambia sola)
    if (keys[GLFW_KEY_E])
        anguloOrbita += 2.0f;
    if (keys[GLFW_KEY_Q])
        anguloOrbita -= 2.0f;

    if (anguloOrbita >= 360.0f) anguloOrbita -= 360.0f;
    if (anguloOrbita < 0.0f) anguloOrbita += 360.0f;

    // J/K: desplaza la posicion base de luz2
    //if (keys[GLFW_KEY_K])
    //    movelightPos2 += 0.1f;
    //if (keys[GLFW_KEY_J])
    //    movelightPos2 -= 0.1f;
}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}