#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class Shader
 * @brief Handles loading, compiling, linking, and using GLSL shader programs.
 *
 * Provides:
 * - File reading for shader source
 * - Compilation of vertex, fragment, and geometry shaders
 * - Program linking and validation
 * - Uniform helper functions (matrices, vectors, floats, ints)
 */
class Shader
{
public:

    /** @brief Creates an empty shader object. */
    Shader();

    /**
     * @brief Creates a shader program from vertex and fragment shader files.
     *
     * @param vertexPath Path to vertex shader file.
     * @param fragmentPath Path to fragment shader file.
     */
    void CreateShader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Creates a shader program from vertex, geometry, and fragment shader files.
     *
     * @param vertexPath Path to vertex shader file.
     * @param geometryPath Path to geometry shader file.
     * @param fragmentPath Path to fragment shader file.
     */
    void CreateShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

    /**
     * @brief Reads a text file and returns its contents.
     *
     * @param filePath Path to the file.
     * @return File contents as a string.
     */
    std::string ReadFile(const char* filePath);

    /** @brief Activates the shader program. */
    void UseShader();

    /** @brief Deletes the shader program. */
    void ClearShader();

    /** @brief Returns the OpenGL shader program ID. */
    int getShaderID() const { return shaderId; }

    /** @brief Sets a mat4 uniform. */
    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    /** @brief Sets a vec3 uniform. */
    void setVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, glm::value_ptr(value));
    }

    /** @brief Sets a vec4 uniform. */
    void setVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(shaderId, name.c_str()), 1, glm::value_ptr(value));
    }

    /** @brief Sets a float uniform. */
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
    }

    /** @brief Sets an int uniform. */
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
    }

    /** @brief Destructor clears the shader program. */
    ~Shader();

private:

    GLuint shaderId; ///< OpenGL shader program ID.

    /** @brief Compiles and links a vertex + fragment shader program. */
    void CompileShader(const char* vertexCode, const char* fragmentCode);

    /** @brief Compiles and links a vertex + geometry + fragment shader program. */
    void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);

    /**
     * @brief Compiles an individual shader and attaches it to a program.
     *
     * @param theProgram Program to attach to.
     * @param shaderCode GLSL source code.
     * @param shaderType Shader type (vertex, fragment, geometry).
     */
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
