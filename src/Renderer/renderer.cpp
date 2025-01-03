#include "Renderer.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Calculate/calculate.h"
#include <cmath>
#include <random>

// Vertex shader source
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in float aSize;  // Новый атрибут для размера точки
    uniform mat4 projection;

    void main() {
        gl_Position = projection * vec4(aPos, 0.0, 1.0);
        gl_PointSize = aSize;  // Используем размер точки
    }
)glsl";

// Fragment shader source
const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;

    void main() {
        // Расчёт расстояния от центра точки
        float dist = length(gl_PointCoord - vec2(0.5));
        if (dist < 0.5) {
            FragColor = vec4(0.0, 0.5, 1.0, 1.0); // Синий цвет внутри круга
        } else {
            discard;  // Отбрасываем пиксели вне круга
        }
    }
)glsl";

Renderer::Renderer() : window(nullptr), VBO(0), VAO(0), shaderProgram(0) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::init(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);
    init_shaders();
    return true;
}

void Renderer::init_shaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Error: Vertex shader compilation failed\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Error: Fragment shader compilation failed\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error: Shader program linking failed\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Renderer::init_points(const std::vector<std::array<float, 2>>& points) {
    std::vector<float> vertices;
    std::vector<float> sizes;

    auto random_double = [](double min, double max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    };

    for (const auto& point : points) {
        vertices.push_back(point[0]);
        vertices.push_back(point[1]);
        sizes.push_back(static_cast<float>(random_double(20.0, 40.0)));  // Более широкий диапазон размеров
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint sizeBuffer;
    glGenBuffers(1, &sizeBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sizeBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizes.size() * sizeof(float), sizes.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::render(const std::vector<std::array<float, 2>>& points) {
    init_shaders();
    init_points(points);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);  // Включение поддержки изменения размера точки

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, -1.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        process_input();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, points.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Renderer::process_input() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Renderer::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
}
