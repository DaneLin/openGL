#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "myShader.h"
// #include "image.cpp"
#include "stb_image.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Camera camera(cameraPos);

// 鼠标位置
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
// 鼠标是否第一次进入窗口
bool is_first_in = true;
// time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 注册一个回调函数，当窗口大小发生变化时被调用
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// 输入控制函数
void processInput(GLFWwindow *window);

// 监听鼠标移动事件
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

/// @brief 监听鼠标滚轮事件
/// @param window 监听窗口
/// @param xoffset 输入的x轴数据
/// @param yoffset 输入的y轴数据
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

/// @brief utility function for loading a 2D texture from file
/// @param path 对应纹理文件的路劲
/// @return 返回读取后的纹理ID
unsigned int loadTexture(const char *path);

int main()
{
    // 初始化GLFW
    glfwInit();
    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建一个窗口对象
    // 存放所有和窗口相关的数据，并且会被GLFW的其他函数频繁用到
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 告诉GLFW隐藏光标，并且捕捉它
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 注册这个函数，告诉GLFW我们希望每次窗口调整大小的时候，调用这个callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 注册鼠标回调函数，当鼠标移动时，调用该回调函数
    glfwSetCursorPosCallback(window, mouse_callback);
    // 注册滚轮回调函数，当鼠标移动时，调用该回调函数
    glfwSetScrollCallback(window, scroll_callback);

    // GLAD是用来管理OpenGL的函数指针的
    // 所以在调用任何OpenGL函数之间，都需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shader ourShader("../src/shader.vs", "../src/shader.fs");
    Shader GouraudShader("../src/shader.vs", "../src/shader.fs");

    Shader lightingShader("../src/lightShader.vs", "../src/lightShader.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 为灯创建一个新的VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 由于此时VBO中已经包含了正确的立方体的顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);

    unsigned int diffuseMap = loadTexture("../src/container2.png");
    unsigned int specularMap = loadTexture("../src/container2_specular.png");
    //unsigned int emissionMap = loadTexture("../src/matrix.jpg");

    GouraudShader.use();
    GouraudShader.setInt("material.diffuse", 0);
    GouraudShader.setInt("material.specular", 1);
    //GouraudShader.setInt("material.emission", 2);

    // render loop
    // glfwWindowShouldClose检查GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 是否按下退出
        processInput(window);

        // 渲染
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightColor(1.f, 1.f, 1.f);

        // be sure to activate shader when setting uniforms/drawing objects
        GouraudShader.use();
        GouraudShader.setVec3("viewPos", camera.Position);
        GouraudShader.setFloat("material.shininess", 32.0f);

        //directional light
        GouraudShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        GouraudShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        GouraudShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        GouraudShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
         // point light 1
        GouraudShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        GouraudShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        GouraudShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        GouraudShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        GouraudShader.setFloat("pointLights[0].constant", 1.0f);
        GouraudShader.setFloat("pointLights[0].linear", 0.09f);
        GouraudShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        GouraudShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        GouraudShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        GouraudShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        GouraudShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        GouraudShader.setFloat("pointLights[1].constant", 1.0f);
        GouraudShader.setFloat("pointLights[1].linear", 0.09f);
        GouraudShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        GouraudShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        GouraudShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        GouraudShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        GouraudShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        GouraudShader.setFloat("pointLights[2].constant", 1.0f);
        GouraudShader.setFloat("pointLights[2].linear", 0.09f);
        GouraudShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        GouraudShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        GouraudShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        GouraudShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        GouraudShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        GouraudShader.setFloat("pointLights[3].constant", 1.0f);
        GouraudShader.setFloat("pointLights[3].linear", 0.09f);
        GouraudShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        GouraudShader.setVec3("spotLight.position", camera.Position);
        GouraudShader.setVec3("spotLight.direction", camera.Front);
        GouraudShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        GouraudShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        GouraudShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        GouraudShader.setFloat("spotLight.constant", 1.0f);
        GouraudShader.setFloat("spotLight.linear", 0.09f);
        GouraudShader.setFloat("spotLight.quadratic", 0.032f);
        GouraudShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        GouraudShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f))); 
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glm::mat4 model;
        // model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 0.0f ,0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        GouraudShader.setMat4("projection", projection);
        GouraudShader.setMat4("view", view);

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            GouraudShader.setMat4("model", model);

            // render the cube

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //draw the lamp objects
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("LightColor", lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 交换颜色缓冲，其在本次迭代中用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);

        // 检查有没有触发什么事件，更新窗口状态，并且调用对应的回调函数
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // 渲染循环结束后，释放/删除之前分配的所有资源
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // 使用glfwGetKey函数
    // 需要一个窗口以及一个按键作为输入
    // 这个函数将会返回这个按键是否正在被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
        // cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        // cameraPos -= cameraSpeed * cameraFront;
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
        // cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (is_first_in)
    {
        lastX = xpos;
        lastY = ypos;
        is_first_in = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // y坐标是从底部往顶部依次增大的
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }

    stbi_image_free(data);
    return textureID;
}
