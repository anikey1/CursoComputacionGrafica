//Practica 12
//Entrega 8 de mayo, 2026
//Anikey Andrea Gomez Guzman 
//319323290

#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2/SOIL2.h
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,  0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,  0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f, -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f, -0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f, -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f, -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,  0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,  0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,  0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f, -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,  0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f
};

glm::vec3 Light1 = glm::vec3(0);

float rotBall = 0.0f;
float rotDog = 0.0f;  // giro sobre Y (horizontal)
float rollDog = 0.0f;  // echarse sobre Z (muertito)
float head = 0.0f;
float tail = 0.0f;
float FLegL = 0.0f;
float FLegR = 0.0f;
float BLegL = 0.0f;
float BLegR = 0.0f;
float dogPosX, dogPosY, dogPosZ;

// ─── KeyFrames ────────────────────────────────────────────────────────────────
#define MAX_FRAMES 20
int i_max_steps = 1000;
int i_curr_steps = 0;

typedef struct _frame {
    // posición
    float dogPosX, dogPosY, dogPosZ;
    float incX, incY, incZ;
    // rotaciones del cuerpo
    float rotDog, rotDogInc;   // giro Y
    float rollDog, rollDogInc;  // volteo Z (muertito)
    // patas
    float FLegL, FLegR, BLegL, BLegR;
    float incFLegL, incFLegR, incBLegL, incBLegR;
    // cabeza y cola
    float head, tail;
    float incHead, incTail;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int  FrameIndex = 0;
bool play = false;
int  playIndex = 0;

// ─── Helpers ──────────────────────────────────────────────────────────────────
void saveFrame()
{
    printf("frameindex %d\n", FrameIndex);
    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].rollDog = rollDog;
    KeyFrame[FrameIndex].FLegL = FLegL;
    KeyFrame[FrameIndex].FLegR = FLegR;
    KeyFrame[FrameIndex].BLegL = BLegL;
    KeyFrame[FrameIndex].BLegR = BLegR;
    KeyFrame[FrameIndex].head = head;
    KeyFrame[FrameIndex].tail = tail;
    FrameIndex++;
}

void resetElements()
{
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    rotDog = KeyFrame[0].rotDog;
    rollDog = KeyFrame[0].rollDog;
    FLegL = KeyFrame[0].FLegL;
    FLegR = KeyFrame[0].FLegR;
    BLegL = KeyFrame[0].BLegL;
    BLegR = KeyFrame[0].BLegR;
    head = KeyFrame[0].head;
    tail = KeyFrame[0].tail;
}

void interpolation()
{
    int p = playIndex;
    KeyFrame[p].incX = (KeyFrame[p + 1].dogPosX - KeyFrame[p].dogPosX) / i_max_steps;
    KeyFrame[p].incY = (KeyFrame[p + 1].dogPosY - KeyFrame[p].dogPosY) / i_max_steps;
    KeyFrame[p].incZ = (KeyFrame[p + 1].dogPosZ - KeyFrame[p].dogPosZ) / i_max_steps;
    KeyFrame[p].rotDogInc = (KeyFrame[p + 1].rotDog - KeyFrame[p].rotDog) / i_max_steps;
    KeyFrame[p].rollDogInc = (KeyFrame[p + 1].rollDog - KeyFrame[p].rollDog) / i_max_steps;
    KeyFrame[p].incFLegL = (KeyFrame[p + 1].FLegL - KeyFrame[p].FLegL) / i_max_steps;
    KeyFrame[p].incFLegR = (KeyFrame[p + 1].FLegR - KeyFrame[p].FLegR) / i_max_steps;
    KeyFrame[p].incBLegL = (KeyFrame[p + 1].BLegL - KeyFrame[p].BLegL) / i_max_steps;
    KeyFrame[p].incBLegR = (KeyFrame[p + 1].BLegR - KeyFrame[p].BLegR) / i_max_steps;
    KeyFrame[p].incHead = (KeyFrame[p + 1].head - KeyFrame[p].head) / i_max_steps;
    KeyFrame[p].incTail = (KeyFrame[p + 1].tail - KeyFrame[p].tail) / i_max_steps;
}

// ─── Macro: posX,posY,posZ, rotDog,rollDog, FLegL,FLegR,BLegL,BLegR, head,tail
#define SET_KF(idx, px,py,pz, rd,rl, fll,flr,bll,blr, hd,tl) \
    KeyFrame[idx].dogPosX=px; KeyFrame[idx].dogPosY=py; KeyFrame[idx].dogPosZ=pz; \
    KeyFrame[idx].rotDog=rd;  KeyFrame[idx].rollDog=rl; \
    KeyFrame[idx].FLegL=fll;  KeyFrame[idx].FLegR=flr; \
    KeyFrame[idx].BLegL=bll;  KeyFrame[idx].BLegR=blr; \
    KeyFrame[idx].head=hd;    KeyFrame[idx].tail=tl;

void loadAnimationKeyframes()
{
    //         idx  posX  posY   posZ  rotY  rollZ  FLegL  FLegR  BLegL  BLegR  head   tail

    // FASE 1 – DE PIE (0-3)
    SET_KF(0, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
        SET_KF(1, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 20.0f)
        SET_KF(2, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, -20.0f)
        SET_KF(3, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 0.0f)

        // FASE 2 – DAR LA PATITA (4-8)
        SET_KF(4, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 3.0f, 5.0f)
        SET_KF(5, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10.0f)
        SET_KF(6, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, -5.0f, 15.0f)
        SET_KF(7, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, -5.0f, 20.0f)
        SET_KF(8, 0.0f, 0.000f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f)

        // FASE 3 – SENTARSE (9-13)
        // BLegL/BLegR negativos = patas traseras van hacia adelante/abajo
        // posY baja para que el cuerpo se hunda al sentarse
        SET_KF(9, 0.0f, -0.010f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -15.0f, 15.0f, 5.0f, 0.0f)
        SET_KF(10, 0.0f, -0.025f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -35.0f, 35.0f, 8.0f, 0.0f)
        SET_KF(11, 0.0f, -0.040f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -55.0f, 55.0f, 10.0f, 0.0f)
        SET_KF(12, 0.0f, -0.055f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -70.0f, 70.0f, 12.0f, 0.0f)
        SET_KF(13, 0.0f, -0.065f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -80.0f, 80.0f, 15.0f, 0.0f)

        // FASE 4 – MUERTITO (14-19)
        // rollDog llega a 90° (echarse de lado), posY sigue bajando
        SET_KF(14, 0.0f, -0.065f, 0.0f, 0.0f, 15.0f, -5.0f, 5.0f, -70.0f, 70.0f, 10.0f, 10.0f)
        SET_KF(15, 0.0f, -0.070f, 0.0f, 0.0f, 35.0f, -15.0f, 15.0f, -50.0f, 50.0f, 5.0f, 15.0f)
        SET_KF(16, 0.0f, -0.075f, 0.0f, 0.0f, 55.0f, -25.0f, 25.0f, -30.0f, 30.0f, 0.0f, 20.0f)
        SET_KF(17, 0.0f, -0.080f, 0.0f, 0.0f, 70.0f, -35.0f, 35.0f, -15.0f, 15.0f, -5.0f, 15.0f)
        SET_KF(18, 0.0f, -0.085f, 0.0f, 0.0f, 82.0f, -40.0f, 40.0f, -5.0f, 5.0f, -10.0f, 5.0f)
        SET_KF(19, 0.0f, -0.090f, 0.0f, 0.0f, 90.0f, -40.0f, 40.0f, 0.0f, 0.0f, -15.0f, 0.0f)

        FrameIndex = 20;
}

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

    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Init keyframe array
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        KeyFrame[i] = { 0 };  // todo en cero
    }

    loadAnimationKeyframes();

    // Estado inicial = frame 0
    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    rotDog = KeyFrame[0].rotDog;
    rollDog = KeyFrame[0].rollDog;
    FLegL = KeyFrame[0].FLegL;  FLegR = KeyFrame[0].FLegR;
    BLegL = KeyFrame[0].BLegL;  BLegR = KeyFrame[0].BLegR;
    head = KeyFrame[0].head;   tail = KeyFrame[0].tail;

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
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();
        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1), modelTemp(1);

        // Piso
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // ── PERRO ─────────────────────────────────────────────────────────────
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // Body: primero giro Y normal, luego volteo Z (muertito)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f)); // giro Y
        model = glm::rotate(model, glm::radians(rollDog), glm::vec3(0.0f, 0.0f, 1.0f)); // echarse Z
        modelTemp = model;
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Head
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Tail
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Front Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegL), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Front Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegR), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Back Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(BLegL), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Back Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(BLegR), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Ball
        model = glm::mat4(1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::translate(model, glm::vec3(dogPosX, dogPosY + 0.3f, dogPosZ));
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);
        glBindVertexArray(0);

        // Lamp cube
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// ─── DoMovement ───────────────────────────────────────────────────────────────
void DoMovement()
{
    if (keys[GLFW_KEY_2]) rotDog += 0.1f;
    if (keys[GLFW_KEY_3]) rotDog -= 0.1f;

    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;

    if (keys[GLFW_KEY_Q]) FLegL += 0.1f;
    if (keys[GLFW_KEY_E]) FLegL -= 0.1f;
    if (keys[GLFW_KEY_B]) FLegR += 0.1f;
    if (keys[GLFW_KEY_N]) FLegR -= 0.1f;
    if (keys[GLFW_KEY_Z]) BLegL += 0.1f;
    if (keys[GLFW_KEY_X]) BLegL -= 0.1f;
    if (keys[GLFW_KEY_C]) BLegR += 0.1f;
    if (keys[GLFW_KEY_V]) BLegR -= 0.1f;

    if (keys[GLFW_KEY_O]) dogPosY += 0.001f;
    if (keys[GLFW_KEY_P]) dogPosY -= 0.001f;

    if (keys[GLFW_KEY_F]) tail += 0.1f;
    if (keys[GLFW_KEY_R]) tail -= 0.1f;
    if (keys[GLFW_KEY_I]) head += 0.1f;
    if (keys[GLFW_KEY_K]) head -= 0.1f;
}

// ─── KeyCallback ──────────────────────────────────────────────────────────────
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)        keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (!play && FrameIndex > 1)
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
        }
        else play = false;
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
        if (FrameIndex < MAX_FRAMES) saveFrame();

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
}

// ─── Animation ────────────────────────────────────────────────────────────────
void Animation()
{
    if (!play) return;

    if (i_curr_steps >= i_max_steps)
    {
        playIndex++;
        if (playIndex > FrameIndex - 2)
        {
            printf("termina anim\n");
            playIndex = 0;
            play = false;
        }
        else
        {
            i_curr_steps = 0;
            interpolation();
        }
    }
    else
    {
        dogPosX += KeyFrame[playIndex].incX;
        dogPosY += KeyFrame[playIndex].incY;
        dogPosZ += KeyFrame[playIndex].incZ;
        rotDog += KeyFrame[playIndex].rotDogInc;
        rollDog += KeyFrame[playIndex].rollDogInc;  // <-- echarse de lado
        FLegL += KeyFrame[playIndex].incFLegL;
        FLegR += KeyFrame[playIndex].incFLegR;
        BLegL += KeyFrame[playIndex].incBLegL;
        BLegR += KeyFrame[playIndex].incBLegR;
        head += KeyFrame[playIndex].incHead;
        tail += KeyFrame[playIndex].incTail;
        i_curr_steps++;
    }
}

// ─── MouseCallback ────────────────────────────────────────────────────────────
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) { lastX = xPos; lastY = yPos; firstMouse = false; }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}