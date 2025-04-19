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

#include <vector>
#include <array>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(int width, int height, const char* title);
    void render(const std::vector<std::array<float, 2>>& points, float L, const std::vector<float>& radii);

    void cleanup();

private:
    GLFWwindow* window;
    GLuint VAO, VBO, sizeVBO;
    GLuint shaderProgram;

    void processInput();
    bool compileShaders();
    void initBuffers(const std::vector<float>& vertices, const std::vector<float>& sizes);
};



#endif //INC_2D_LD_POTENTIONAL_MIN_RENDERER_H
