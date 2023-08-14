#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "myShader.h"
// #include "image.cpp"
#include "stb_image.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 注册一个回调函数，当窗口大小发生变化时被调用
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// 输入控制函数
void processInput(GLFWwindow *window);

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

    // 注册这个函数，告诉GLFW我们希望每次窗口调整大小的时候，调用这个callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD是用来管理OpenGL的函数指针的
    // 所以在调用任何OpenGL函数之间，都需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("../src/shader.vs", "../src/shader.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f   // top left
    };
    // 用两个三角形组合成为一个矩形
    unsigned int indices[] = {
        // 顺时针顺序
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 纹理部分
    unsigned int texture;
    glGenTextures(1, &texture);            // 第一个参数，生成纹理的数量，第二个参数，保存的位置
    glBindTexture(GL_TEXTURE_2D, texture); // 绑定纹理
    // set the texture wrapping paprameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // 读取图像信息
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("D:\\Github\\openGL-1\\src\\container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // 生成了纹理和相对应的mipmap后，释放图像的内存
    stbi_image_free(data);

    // texture2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    // set the texture wrapping paprameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 读取图片信息
    data = stbi_load("D:\\Github\\openGL-1\\src\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel
        // make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    float offset = 0.5f;
    // render loop
    // glfwWindowShouldClose检查GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        // 是否按下退出
        processInput(window);

        // 渲染
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        
        static double time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (glfwGetTime() - time >= .01f)
            {
                time = glfwGetTime();
                offset += 0.01f;
                std::cout << (offset) << std::endl;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (glfwGetTime() - time >= .01f)
            {
                time = glfwGetTime();
                offset -= 0.01f;
                std::cout << (offset) << std::endl;
            }
        }

        ourShader.use();
        ourShader.setFloat("mix_offset", offset);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 交换颜色缓冲，其在本次迭代中用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);

        // 检查有没有触发什么事件，更新窗口状态，并且调用对应的回调函数
        glfwPollEvents();
    }
    // //在开始渲染前，必须告诉OpenGL渲染窗口的尺寸大小，即视口viewport
    // glViewport(0, 0, 800, 600);

    // optional:
    // de-allocate all resources once they're outlived their purpose

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
}