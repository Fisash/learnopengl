#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

char* read_shader_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* shader_code = (char*)malloc(file_size + 1); // +1 для null-терминатора
    if (!shader_code) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    size_t read_bytes = fread(shader_code, 1, file_size, file);
    if (read_bytes < file_size) {
        if (ferror(file)) {
            perror("Failed to read file");
        }
        fprintf(stderr, "Warning: expected to read %ld bytes, but got %zu bytes\n", file_size, read_bytes);
    }
    shader_code[read_bytes] = '\0'; 

    fclose(file);
    return shader_code;
}


GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath) {
    char* vertexCode = read_shader_file(vertexPath);
    char* fragmentCode = read_shader_file(fragmentPath);

    printf("vertex shader source:\n%s\n", vertexCode);
    printf("fragment shader source:\n%s\n", fragmentCode);

    if (!vertexCode || !fragmentCode) {
        return 0;
    }

    // Vertex shader compile
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char**)&vertexCode, NULL);
    glCompileShader(vertexShader);
    free(vertexCode);
    
    int success;
    char infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        fprintf(stderr, "compile vertex shader error: %s\n", infoLog);
        return 0;
    }

    //Fragment shader compile
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char**)&fragmentCode, NULL);
    glCompileShader(fragmentShader);
    free(fragmentCode);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        fprintf(stderr, "compile fragment shader error: %s\n", infoLog);
        return 0;
    }

    //Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        fprintf(stderr, "unite shader program error: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
