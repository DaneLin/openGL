#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//编辑着色器
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

//注册一个回调函数，当窗口大小发生变化时被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//输入控制函数
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

    //注册这个函数，告诉GLFW我们希望每次窗口调整大小的时候，调用这个callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //GLAD是用来管理OpenGL的函数指针的
    //所以在调用任何OpenGL函数之间，都需要初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    //OpenGL中有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
    //使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //绑定完成后，我们使用的任何(在GL_ARRAY_BUFFER目标上的)缓冲调用都会用来配置当前绑定的缓冲VBO
    //然后我们可以调用glBufferData函数，其会把之前定义的顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //为了使用着色器，必须在运行时动态编译其源代码
    //首先创建一个着色器对象，通过ID来引用
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //将着色器源码附加到着色器对象上，然后编译他
    //将要编译的着色器对象作为第一个参数，第二参数指定了传递源码的字符串数量
    //第三个参数时顶点着色器的真正源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //编译片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //链接这些着色器
    //创建一个程序对象
    unsigned int shaderProgram;
    //glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用
    shaderProgram = glCreateProgram();

    //将之前编译的着色器附加到程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //这个函数调用后，每个着色器调用和渲染调用都会使用这个程序对象
    glUseProgram(shaderProgram);
    //将着色器对象链接到程序对象后，就不再需要着色器对象，可以直接删除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //顶点着色器允许我们指定任何以顶点属性作为形式的输入
    //意味这我们必须手动指定输入数据的哪一个部分对应顶点着色器的哪一个顶点属性
    //必须在渲染前指定OpenGL该如何解释顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //render loop
    //glfwWindowShouldClose检查GLFW是否被要求退出
    while (!glfwWindowShouldClose(window))
    {   
        //是否按下退出
        processInput(window);

        //渲染
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        //交换颜色缓冲，其在本次迭代中用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);

        //检查有没有触发什么事件，更新窗口状态，并且调用对应的回调函数
        glfwPollEvents();
    }
    // //在开始渲染前，必须告诉OpenGL渲染窗口的尺寸大小，即视口viewport
    // glViewport(0, 0, 800, 600);

    //渲染循环结束后，释放/删除之前分配的所有资源
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    //使用glfwGetKey函数
    //需要一个窗口以及一个按键作为输入
    //这个函数将会返回这个按键是否正在被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}