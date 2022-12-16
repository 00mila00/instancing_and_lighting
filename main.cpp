//////////ZAD3////////////
#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include <stdio.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

#include "..\..\src\Mesh.h"
#include "..\..\src\Shader.h"
#include "..\..\src\Model.h"
#include "..\..\src\Camera.h"

#define PI 3.14159265
#define Cos(th) cos(PI/180*(th))
#define Sin(th) sin(PI/180*(th))

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

glm::mat4* transformHouse(glm::mat4* modelMatrices, std::string type);
void lightSetting(Shader sh);
void bufferAttrib(Model m);

// settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(4.0f, 3.0f, 16.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float atime = 0.0f;

// VARIABLES
unsigned int amount = 4000;
int halfAmount = (int)(amount / 2.0);
int octAmount = (int)(amount / 28.0);
int sqrtAmount = (int)(sqrt(amount));

float speed = 0.01f;
float cam_speed = 5.5f;
float x_rotation = 0.2;
float y_rotation = 0.55;
float z_rotation = 0.0;

float offModelx = -octAmount;
float offModely = -0.3f;
float offModelz = 0.0f;

float offHx = 5.0f;
float offHy = 0.0f;
float offHz = octAmount;

float offRx = 0.0f;
float offRy = 0.0f;
float offRz = 0.0f;

int iter = 0;
int counter = 0;

float scaleModel = 1.0f;

//float scaleH = 1.0f;
float scaleR = 1.0f;
float scaleRY = 1.0f;
float rotHouse = 0.0f;

bool changeGui = false;

bool pointLightFlag = true;
bool spotLight1Flag = true;
bool spotLight2Flag = true;
bool dirLightFlag = true;

float DirX = -0.2f;
float DirY = -1.0f;
float DirZ = -0.3f;

//COLORS
float pointAmbient[3] = { 1.f, 1.f, 0.f };
float spotAmbient[3] = { 0.7f, 0.7f, 0.7f };
float spotAmbient1[3] = { 0.9f, 0.9f, 0.9f };
float dirAmbient[3] = { 0.2f, 0.2f, 0.2f };

GLfloat skyColor[3] = { 0.75f, 0.83f, 1.0f };

float constant = 1.0f;
float linear = 0.01;
float quadratic = 0.01;

glm::vec3 spotDirection(1.0f, 0.0f, 0.0f);
glm::vec3 spot2Direction(0.0f, 1.0f, 0.0f);

glm::vec3 PosLight1(10.8f, 3.f, 20.5f);
glm::vec3 PosSpot1(1.9f, 2.f, 2.5f);
glm::vec3 PosSpot2(6.0f, 2.0f, 10.0f);
glm::vec3 PosDir(0.0f, 20.0f, .0f);

// positions of the point lights
glm::vec3 lightPositions[] = {
    glm::vec3(PosLight1),
    glm::vec3(PosSpot1),
    glm::vec3(PosSpot2),
    glm::vec3(PosDir)
};

unsigned int VAO;
unsigned int VBO, VBO2;

bool firstMouse = true;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};



std::vector <glm::vec3> stars;

// METHODS
void generateTexture(GLuint tex, const char* filename);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
     const char* glsl_version = "#version 430";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Housing Estate", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLFWimage images[1];
    images[0].pixels = stbi_load("../../res/textures/burning_house.jpg", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    // tell GLFW to capture our mouse
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGui::StyleColorsClassic();

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    Shader instanceShader("ishader.vert", "lightingShader.frag");
    Shader modelShader("lightingShader.vert", "lightingShader.frag");
    Shader lightCubeShader("shader.vert", "shader.frag");

    // load models
    Model cube("../../res/models/cube.obj");
    Model piramid("../../res/models/piramid.obj");
    Model platform("../../res/models/platform.obj");

    // generate a large list of semi-random model transformation matrices
    //BASE TRANSFORMATIONS
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    modelMatrices = transformHouse(modelMatrices, "house");
    offHz = octAmount;
    offModelx = -octAmount;
    //ROOF TRANSFORMATIONS
    glm::mat4* modelMatrices2;
    modelMatrices2 = new glm::mat4[amount];
    iter = 0;
    counter = 0;
    modelMatrices2 = transformHouse(modelMatrices2, "roof");
    // configure instanced array
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    bufferAttrib(cube);

    unsigned int bufferR;
    glGenBuffers(1, &bufferR);
    glBindBuffer(GL_ARRAY_BUFFER, bufferR);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices2[0], GL_STATIC_DRAW);
    bufferAttrib(piramid);

    GLuint texture[4];
    glGenTextures(4, texture);
    generateTexture(texture[0], "../../res/textures/roof.jpg");
    generateTexture(texture[1], "../../res/textures/ground.jpg");
    generateTexture(texture[2], "../../res/textures/window.png");
    generateTexture(texture[3], "../../res/textures/white.png");

    GLint textureLocation = glGetUniformLocation(instanceShader.ID, "texture");




    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);
        // render
        glClearColor(skyColor[0],skyColor[1], skyColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        //ROTATE POINTLIGHT
        float theta = 0.0;
        theta = theta + 0.0075;

        if (theta >= (2.0 * PI))
            theta = theta - (2.0 * PI);

        PosLight1[0] = PosLight1[0] * glm::cos(theta) - PosLight1[2] * glm::sin(theta);
        PosLight1[2] = PosLight1[0] * glm::sin(theta) + PosLight1[2] * glm::cos(theta);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(textureLocation, 0);
        camera.MovementSpeed = cam_speed;
        if (changeGui) {
            iter = 0;
            counter = 0;
            modelMatrices = transformHouse(modelMatrices, "house");
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
            iter = 0;
            counter = 0;
            modelMatrices2 = transformHouse(modelMatrices2, "roof");
            glBindBuffer(GL_ARRAY_BUFFER, bufferR);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices2[0], GL_STATIC_DRAW);
            changeGui = false;
        }

        // configure transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        
        instanceShader.use();
        instanceShader.setFloat("material.shininess", 50.0f);

        view = glm::rotate(view, x_rotation, glm::vec3(1.0, 0, 0));
        view = glm::rotate(view, y_rotation, glm::vec3(0.0, 1.0, 0));
        view = glm::rotate(view, z_rotation, glm::vec3(0.0, 0.0, 1.0));

        camera.Yaw = 0.9;
        
        view = glm::translate(view, glm::vec3(0.0, -0.95, 0.0));
     
        instanceShader.setMat4("projection", projection);
        instanceShader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glUniform1i(textureLocation, 0);
  
        modelShader.use();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        
        //platform
        int halfTileAmount = 40;
        for (int i = -halfTileAmount; i < halfTileAmount; i++) {
            for (int j = -halfTileAmount; j < halfTileAmount; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(i * 10.0, -0.8f, j * 9.0f));
                model = glm::scale(model, glm::vec3(1.0, 0.4, 0.9));
                modelShader.setMat4("model", model);
                platform.Draw(modelShader);
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glUniform1i(textureLocation, 0);
        instanceShader.use();
        instanceShader.setInt("texture", 0);

        lightSetting(instanceShader);

        model = glm::mat4(1.0f);

        for (unsigned int i = 0; i < cube.meshes.size(); i++)
        { 
            glBindVertexArray(cube.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(cube.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glUniform1i(textureLocation, 0);
     
        model = glm::mat4(1.0f);
        for (unsigned int i = 0; i < piramid.meshes.size(); i++)
        {
            glBindVertexArray(piramid.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(piramid.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }
        modelShader.use();
        
        lightSetting(modelShader);

       
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glUniform1i(textureLocation, 0);
        //POINT LIGHT CUBES TRANSFORM
        if (pointLightFlag) {
            lightCubeShader.setVec4("ourColor", pointAmbient[0], pointAmbient[1], pointAmbient[2], 1.0);

            glBindVertexArray(cubeVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, PosLight1);
            float angle = 20.0f;
            model = glm::scale(model, glm::vec3(0.6));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36); 
            glBindVertexArray(0);
        }

        if (spotLight1Flag) {
            lightCubeShader.setVec4("ourColor", spotAmbient[0], spotAmbient[1], spotAmbient[2], 1.0);
            glBindVertexArray(cubeVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[1]);
            float angle = 20.0f;
            model = glm::scale(model, glm::vec3(0.6));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        if (spotLight2Flag) {
            lightCubeShader.setVec4("ourColor", spotAmbient1[0], spotAmbient1[1], spotAmbient1[2], 1.0);
            glBindVertexArray(cubeVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[2]);
            float angle = 20.0f;
            model = glm::scale(model, glm::vec3(0.6));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        if (dirLightFlag) {
            lightCubeShader.setVec4("ourColor", dirAmbient[0], dirAmbient[1], dirAmbient[2], 1.0);
            glBindVertexArray(cubeVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[3]);
            float angle = 20.0f;
            model = glm::scale(model, glm::vec3(0.6));
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("View settings");
            ImGui::SliderFloat("Speed", &speed, 0.01, 0.02);
            ImGui::SliderFloat("Camera speed", &cam_speed, 2.0, 10.0);
            ImGui::ColorEdit3("Sky Color", skyColor);

            ImGui::SliderFloat("X Rotation", &x_rotation, -PI, PI);
            ImGui::SliderFloat("Y Rotation", &y_rotation, -PI, PI);
            if (ImGui::SliderFloat("Z Rotation", &z_rotation, -PI, PI)) changeGui = true;;
            ImGui::End();
            ImGui::Begin("Transformation");
            ImGui::Text("Offset");
            if (ImGui::SliderFloat("Offset Roof x", &offRx, 0.0, 5.0)) changeGui = true;
            if (ImGui::SliderFloat("Offset Roof y", &offRy, 0.0, 5.0)) changeGui = true;
            if (ImGui::SliderFloat("Offset Roof z", &offRz, 0.0, 5.0)) changeGui = true; 

            if (ImGui::SliderFloat("Offset All x", &offModelx, -180, -120)) changeGui = true;
            if (ImGui::SliderFloat("Offset All y", &offModely, -2, 20)) changeGui = true;
            if (ImGui::SliderFloat("Offset All z", &offModelz, -20, 20)) changeGui = true;
            if (ImGui::SliderFloat("Home interval x", &offHx, 0, 20)) changeGui = true;
            ImGui::Text("Scale");
            if (ImGui::SliderFloat("Home scale", &scaleModel, 0.0, 2.0)) changeGui = true;
            if (ImGui::SliderFloat("Scale Roof (x,z)", &scaleR, 0, 2)) changeGui = true;
            if (ImGui::SliderFloat("Scale Roof y", &scaleRY, 0, 2)) changeGui = true;
            
            ImGui::End();
            ImGui::Begin("Light");

            ImGui::Checkbox("Pointlight", &pointLightFlag);
            ImGui::Checkbox("Spotlight", &spotLight1Flag);
            ImGui::Checkbox("Spotlight2", &spotLight2Flag);
            ImGui::Checkbox("Direct. Light", &dirLightFlag);

            ImGui::ColorEdit3("Pointlight Color", pointAmbient);
            ImGui::ColorEdit3("Direct Light Color", dirAmbient);
            ImGui::SliderFloat("Direct Light xDir", &DirX, -1, 1);
            ImGui::SliderFloat("Direct Light yDir", &DirY, -1, 1);
            ImGui::SliderFloat("Direct Light zDir", &DirZ, -1, 1);

            ImGui::ColorEdit3("Spotlight 1 Color", spotAmbient);

            ImGui::SliderFloat("Spotlight 1 xDir", &spotDirection.x, -1, 1);
            ImGui::SliderFloat("Spotlight 1 yDir", &spotDirection.y, -1, 1);
            ImGui::SliderFloat("Spotlight 1 zDir", &spotDirection.z, -1, 1);

            ImGui::ColorEdit3("Spotlight 2 Color", spotAmbient1);
            ImGui::SliderFloat("Spotlight 2 xDir", &spot2Direction.x, -1, 1);
            ImGui::SliderFloat("Spotlight 2 yDir", &spot2Direction.y, -1, 1);
            ImGui::SliderFloat("Spotlight 2 zDir", &spot2Direction.z, -1, 1);

            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        x_rotation += speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && x_rotation > -PI/2)
        x_rotation -= speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        y_rotation -= speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        y_rotation += speed;
}
glm::mat4* transformHouse(glm::mat4* modelMatrices, std::string type) {
    
    float x, y, z;

    for (int i = -halfAmount; i < halfAmount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        
        if (type == "house") {
            x = offModelx + offHx * iter;
            y = offModely + offHy;
            z = offModelz + offHz;
            model = glm::translate(model, glm::vec3(x, y, z));
            model = glm::scale(model, glm::vec3(scaleModel * 0.9));
            model = glm::scale(model, glm::vec3(1.0, 1.06, 1.0));
            model = glm::rotate(model, z_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else if (type == "roof") {
            x = offModelx + offRx + offHx * iter;
            y = offModely + offRy + offHy;
            z = offModelz + offRz + offHz;

            model = glm::translate(model, glm::vec3(x, y, z));    
            model = glm::scale(model, glm::vec3(scaleR, scaleRY, scaleR) * 0.9f * scaleModel);
            model = glm::scale(model, glm::vec3(1.0, 1.06, 1.0));
            model = glm::rotate(model, z_rotation, glm::vec3(0.0f, 1.0f, 0.0f)); 
        }
        offHz += -5.0;
        counter++;
        if (counter == sqrtAmount) {
            iter++;
            offHz = octAmount;
            counter = 0;
        }
        modelMatrices[i + halfAmount] = model;
    }
    return modelMatrices;
}

void bufferAttrib(Model model) {
    for (unsigned int i = 0; i < model.meshes.size(); i++)
    {
        unsigned int VAO = model.meshes[i].VAO;
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glBindVertexArray(0);
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void generateTexture(GLuint texture, const char* filename) {
    int w, h, n;
    glBindTexture(GL_TEXTURE_2D, texture);
    unsigned char* data = stbi_load(filename, &w, &h, &n, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(data);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
void lightSetting(Shader lightingShader) {
    lightingShader.setFloat("material.shininess", 50.0f);
    // directionalLight
    lightingShader.setBool("dirLight[0].flag", dirLightFlag);
    lightingShader.setVec3("dirLight[0].direction", DirX, DirY, DirZ);
    lightingShader.setVec3("dirLight[0].ambient", dirAmbient[0], dirAmbient[1], dirAmbient[2]);
    lightingShader.setVec3("dirLight[0].diffuse", 0.7f, 0.7f, 0.7f);
    lightingShader.setVec3("dirLight[0].specular", 0.7f, 0.7f, 0.7f);
    // point light 1
    lightingShader.setBool("pointLights[0].flag", pointLightFlag);
    lightingShader.setVec3("pointLights[0].position", PosLight1);
    lightingShader.setVec3("pointLights[0].ambient", pointAmbient[0], pointAmbient[1], pointAmbient[2]);
    lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("pointLights[0].constant", 1.0f);
    lightingShader.setFloat("pointLights[0].linear", 0.09f);
    lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
    // spotLight1
    lightingShader.setBool("spotLight[0].flag", spotLight1Flag);
    lightingShader.setVec3("spotLight[0].position", PosSpot1);
    lightingShader.setVec3("spotLight[0].direction", spotDirection);
    lightingShader.setVec3("spotLight[0].ambient", spotAmbient[0], spotAmbient[1], spotAmbient[2]);
    lightingShader.setVec3("spotLight[0].diffuse", 0.7f, 0.7f, 0.7f);
    lightingShader.setVec3("spotLight[0].specular", 0.7f, 0.7f, 0.7f);
    lightingShader.setFloat("spotLight[0].constant", 1.0f);
    lightingShader.setFloat("spotLight[0].linear", 0.09f);
    lightingShader.setFloat("spotLight[0].quadratic", 0.032f);
    lightingShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
    // spotLight2
    lightingShader.setBool("spotLight[1].flag", spotLight2Flag);
    lightingShader.setVec3("spotLight[1].position", PosSpot2);
    lightingShader.setVec3("spotLight[1].direction", spot2Direction);
    lightingShader.setVec3("spotLight[1].ambient", spotAmbient1[0], spotAmbient1[1], spotAmbient1[2]);
    lightingShader.setVec3("spotLight[1].diffuse", 0.1f, 0.1f, 0.1f);
    lightingShader.setVec3("spotLight[1].specular", 0.1f, 0.1f, 0.1f);
    lightingShader.setFloat("spotLight[1].constant", 1.0f);
    lightingShader.setFloat("spotLight[1].linear", 0.09f);
    lightingShader.setFloat("spotLight[1].quadratic", 0.032f);
    lightingShader.setFloat("spotLight[1].cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("spotLight[1].outerCutOff", glm::cos(glm::radians(15.0f)));
}