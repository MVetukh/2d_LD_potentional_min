//
// Created by MaxV on 04.11.2024.
//

#ifndef INC_2D_LD_POTENTIONAL_MIN_RENDERER_H
#define INC_2D_LD_POTENTIONAL_MIN_RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include <GLFW/glfw3.h>


class Renderer {
public:
    Renderer();
    ~Renderer();

    // Инициализация OpenGL и создание окна
    bool init(int width, int height, const char* title);

    // Главный цикл отрисовки
    void render(const std::vector<std::array<float, 2>>& points);

    // Очистка ресурсов
    void cleanup();
private:
    GLFWwindow* window;
    GLuint VBO, VAO, shaderProgram;
    glm::mat4 projection;

    // Инициализация шейдеров
    void init_shaders();

    // Инициализация точек для отрисовки
    void init_points(const std::vector<std::array<float, 2>>& points);

    // Обработка ввода
    void process_input();


};



#endif //INC_2D_LD_POTENTIONAL_MIN_RENDERER_H
