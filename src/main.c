#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "shader.h"
#include "buffer.h"

// Прототипы функций
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        fprintf(stderr, "cant initialize GLFW\n");
        return -1;
    }

    // Установка версии OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
    if (!window) {
        fprintf(stderr, "cant create window GLFW\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "cant initialize GLAD\n");
        return -1;
    }

    int screenWidth, screenHeight;

    // Компиляция шейдеров
    GLuint shaderProgram = createShaderProgram("src\\shaders\\vertex_shader.glsl", "src\\shaders\\fragment_shader.glsl");

    // Определение данных треугольника
    float triangleVertices[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f
    };

    // Создание VAO для треугольника
    GLuint VAO = createVAO(triangleVertices, sizeof(triangleVertices) / sizeof(float));

    // Основной цикл рендера
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

        // Передаём размеры в шейдер
        int iResolutionLocation = glGetUniformLocation(shaderProgram, "iResolution");
        if (iResolutionLocation != -1) {
            glUniform2f(iResolutionLocation, (float)screenWidth, (float)screenHeight);
        }

        int timeLocation = glGetUniformLocation(shaderProgram, "iTime");
        if (timeLocation != -1) {
            glUniform1f(timeLocation, (float)glfwGetTime());
        }

        // Очистка экрана
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Использование шейдерной программы и отрисовка треугольника
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // События и буферы
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
