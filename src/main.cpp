#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define PATH_FRAGMENT_SHADER "src/shaders/fragment.glsl"
#define PATH_VERTEX_SHADER "src/shaders/vertex.glsl"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
static std::string loadFile(const std::string &path);
unsigned int compileShader(unsigned int type, const std::string &source);

// constants
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices_triangle[] = {
     0.0f,  0.5f, 0.0f, // top Vertex 1 - 12 Bytes - 3 bytes each xyz
    -0.5f, -0.5f, 0.0f, // bottom left Vertex 2 - 12 Bytes - 3 bytes each xyz
     0.5f, -0.5f, 0.0f  // bottom right Vertex 3 - 12 Bytes - 3 bytes each xyz
};

int main()
{
    // initialise variables
    unsigned int VBO;

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle Render", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (gladLoadGL(glfwGetProcAddress) == 0)
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // vertex array / buffer setup
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);

    // vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Initialise our shaders using our new custom shader loading and compiling functions
    std::string vertexShaderSource = loadFile(PATH_VERTEX_SHADER);
    std::string fragmentShaderSource = loadFile(PATH_FRAGMENT_SHADER);

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Check if successful
    if (vertexShader == 0 || fragmentShader == 0)
    {
        return -1;
    }

    // Create shader program and link shaders to it
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if linking was successful
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
        return -1;
    }
    
    // Use our shader program when we want to render an object
    glUseProgram(shaderProgram);

    // After rendering, we can delete the shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render here
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // magic
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

static std::string loadFile(const std::string &path)
{
    std::ifstream in(path);

    if (!in.is_open())
    {
        return std::string();
    }

    std::ostringstream ss;
    ss << in.rdbuf();

    return ss.str();
}

unsigned int compileShader(unsigned int type, const std::string &source)
 {
    const char *shaderSource = source.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int shader_success;
    char shader_infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_success);
    if (!shader_success)
    {
        glGetShaderInfoLog(shader, 512, NULL, shader_infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << shader_infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
 }

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
