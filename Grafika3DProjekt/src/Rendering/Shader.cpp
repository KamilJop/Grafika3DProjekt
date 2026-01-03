#include "Shader.h"

/**
 * @brief Creates an empty shader object.
 */
Shader::Shader()
{
    shaderId = 0;
}

/**
 * @brief Reads a text file and returns its contents as a string.
 *
 * @param filePath Path to the shader file.
 * @return File contents as a string, or empty string on failure.
 */
std::string Shader::ReadFile(const char* filePath)
{
    std::string fileContent;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cout << "Could not read file " << filePath << "." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        fileContent.append(line + "\n");
    }

    fileStream.close();
    return fileContent;
}

/**
 * @brief Creates a shader program from vertex and fragment shader files.
 *
 * @param vertexPath Path to vertex shader file.
 * @param fragmentPath Path to fragment shader file.
 */
void Shader::CreateShader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode = ReadFile(vertexPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    CompileShader(vShaderCode, fShaderCode);
}

/**
 * @brief Creates a shader program from vertex, geometry, and fragment shader files.
 *
 * @param vertexPath Path to vertex shader file.
 * @param geometryPath Path to geometry shader file.
 * @param fragmentPath Path to fragment shader file.
 */
void Shader::CreateShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
    std::string vertexCode = ReadFile(vertexPath);
    std::string geometryCode = ReadFile(geometryPath);
    std::string fragmentCode = ReadFile(fragmentPath);

    const char* vShaderCode = vertexCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    CompileShader(vShaderCode, gShaderCode, fShaderCode);
}

/**
 * @brief Compiles an individual shader and attaches it to a program.
 *
 * @param theProgram Program to attach to.
 * @param shaderCode Shader source code.
 * @param shaderType GL shader type (vertex, fragment, geometry).
 */
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1] = { shaderCode };
    GLint codeLength[1] = { (GLint)strlen(shaderCode) };

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        std::cout << "Error compiling the " << shaderType << " shader: " << eLog << std::endl;
        return;
    }

    glAttachShader(theProgram, theShader);
}

/**
 * @brief Compiles and links a shader program (vertex + fragment).
 *
 * @param vertexCode Vertex shader source.
 * @param fragmentCode Fragment shader source.
 */
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderId = glCreateProgram();
    if (!shaderId) {
        std::cout << "Error creating shader program!" << std::endl;
        return;
    }

    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: " << eLog << std::endl;
        return;
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program: " << eLog << std::endl;
        return;
    }
}

/**
 * @brief Compiles and links a shader program (vertex + geometry + fragment).
 *
 * @param vertexCode Vertex shader source.
 * @param geometryCode Geometry shader source.
 * @param fragmentCode Fragment shader source.
 */
void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
    shaderId = glCreateProgram();
    if (!shaderId) {
        std::cout << "Error creating shader program!" << std::endl;
        return;
    }

    AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderId, geometryCode, GL_GEOMETRY_SHADER);
    AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: " << eLog << std::endl;
        return;
    }

    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderId, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program: " << eLog << std::endl;
        return;
    }
}

/**
 * @brief Activates the shader program.
 */
void Shader::UseShader()
{
    glUseProgram(shaderId);
}

/**
 * @brief Deletes the shader program.
 */
void Shader::ClearShader()
{
    if (shaderId != 0) {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }
}

/**
 * @brief Destructor clears the shader program.
 */
Shader::~Shader()
{
    ClearShader();
}
