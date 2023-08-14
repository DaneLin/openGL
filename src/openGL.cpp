#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "myShader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 编辑着色器
const char *vertexShaderSource1 = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "layout (location = 1) in vec3 aColor;\n"
                                  "out vec3 ourColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                  "   ourColor = aColor;\n"
                                  "}\0";

const char *vertexShaderSource2 = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "out vec4 vertexColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                  "   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
                                  "}\0";

const char *fragmentShaderSource1 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "in vec3 ourColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(ourColor, 1.f);\n"
                                    "}\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "uniform vec4 ourColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = ourColor;\n"
                                    "}\0";

// float vertices[] = {
//     0.5f, 0.5f, 0.0f,
//     -0.5f, 0.5f, 0.0f,
//     0.0f, 0.0f, 0.0f,
//     0.5f, -0.5f, 0.0f,
//     -0.5f, -0.5f, 0.0f,
// };

// unsigned int indices[] = {
//     // 注意索引从0开始!
//     // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
//     // 这样可以由下标代表顶点组合成矩形

//     0, 1, 2, // 第一个三角形
//     2, 3, 4  // 第二个三角形
// };

// 直接用教程中給出的
float firstTriangle[] = {
    // 位置              // 颜色
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // 顶部
};
float secondTriangle[] = {
    0.0f, -0.5f, 0.0f, // left
    0.9f, -0.5f, 0.0f, // right
    0.45f, 0.5f, 0.0f  // top
};

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

    // 为了使用着色器，必须在运行时动态编译其源代码
    // 首先创建一个着色器对象，通过ID来引用
    unsigned int vertexShader1;
    vertexShader1 = glCreateShader(GL_VERTEX_SHADER);
    // 将着色器源码附加到着色器对象上，然后编译他
    // 将要编译的着色器对象作为第一个参数，第二参数指定了传递源码的字符串数量
    // 第三个参数时顶点着色器的真正源码
    glShaderSource(vertexShader1, 1, &vertexShaderSource1, NULL);
    glCompileShader(vertexShader1);

    unsigned int vertexShader2;
    vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader2);

    // check for shader compile error
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader1, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAIED\n"
                  << std::endl;
    }

    // // 编译片段着色器
    // unsigned int fragmentShader;
    // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
    // glCompileShader(fragmentShader);
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAIED\n"
    //               << std::endl;
    // }

    // // 链接这些着色器
    // // 创建一个程序对象
    // unsigned int shaderProgram;
    // // glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用
    // shaderProgram = glCreateProgram();

    // // 将之前编译的着色器附加到程序对象上
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    // // check for linking error
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success)
    // {
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAIED\n"
    //               << std::endl;
    // }
    // // 将着色器对象链接到程序对象后，就不再需要着色器对象，可以直接删除
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgramOrange = glCreateProgram();
    unsigned int shaderProgramYellow = glCreateProgram();

    glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShaderOrange);
    glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShaderYellow);
    // link the first program object
    glAttachShader(shaderProgramOrange, vertexShader1);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);
    // second program object
    glAttachShader(shaderProgramYellow, vertexShader2);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // // 使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    // unsigned int VBO, VAO, EBO;
    // glGenBuffers(1, &VBO);
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &EBO);
    // // bind the Vertex Array Object first, then bind and set vertex buffers
    // // and then configure vertex attributes
    // glBindVertexArray(VAO);
    // // OpenGL中有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
    // // 使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // // 绑定完成后，我们使用的任何(在GL_ARRAY_BUFFER目标上的)缓冲调用都会用来配置当前绑定的缓冲VBO
    // // 然后我们可以调用glBufferData函数，其会把之前定义的顶点数据复制到缓冲的内存中
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // // 顶点着色器允许我们指定任何以顶点属性作为形式的输入
    // // 意味这我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性
    // // 必须在渲染前指定OpenGL该如何解释顶点数据
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 练习2
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    // first triangle setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second triangle setup
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    Shader ourShader("../src/shader.vs", "../src/shader.fs");

    // render loop
    // glfwWindowShouldClose检查GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {
        // 是否按下退出
        processInput(window);

        // 渲染
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        float timeValue = glfwGetTime();
        float offset =  (std::sin(timeValue) / 2.f);
        ourShader.setFloat("horizontal_offset", offset);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /*
        // 绘制三角形
        glUseProgram(shaderProgramOrange);
        // glBindVertexArray(VAO);
        // // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 着色器中的uniform还是空的，需要给它添加数据
        // 首先找到着色器中uniform属性的索引/位置值
        // 得到uniform的索引/位置值之后，就可以更新

        float timeValue = glfwGetTime();
        float greenValue = (std::sin(timeValue) / 2.f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
        glUseProgram(shaderProgramYellow);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        */

        // 交换颜色缓冲，其在本次迭代中用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);

        // 检查有没有触发什么事件，更新窗口状态，并且调用对应的回调函数
        glfwPollEvents();
    }
    // //在开始渲染前，必须告诉OpenGL渲染窗口的尺寸大小，即视口viewport
    // glViewport(0, 0, 800, 600);

    // optional:
    // de-allocate all resources once they're outlived their purpose
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);

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