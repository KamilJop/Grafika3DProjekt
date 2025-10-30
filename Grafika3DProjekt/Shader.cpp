#include "Shader.h"

// Constructor
Shader::Shader()
{
	shaderId = 0;
}

// Method used to read shader files
std::string Shader::ReadFile(const char* filePath) {

	// Setup variable for content
	std::string fileContent;

	// Open file stream
	std::ifstream fileStream(filePath, std::ios::in);

	// Check if file is open
	if (!fileStream.is_open()) {
		std::cout << "Could not read file " << filePath << "." << std::endl;
		return "";
	}

	// Read file line by line
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		fileContent.append(line + "\n");
	}

	// Close file stream
	fileStream.close();

	// Return content
	return fileContent;
}


// Method used to add shader to program
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {

	// Create shader object
	GLuint theShader = glCreateShader(shaderType);

	// Send GLSL code to shader object
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);

	// Compile shader
	glCompileShader(theShader);

	// Check for errors
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the " << shaderType << " shader: " << eLog << std::endl;
		return;
	}

	// Attach shader to program
	glAttachShader(theProgram, theShader);
}

// Method used to compile shaders
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	// Create shader program
	shaderId = glCreateProgram();

	// Check for errors
	if (!shaderId) {
		std::cout << "Error creating shader program!" << std::endl;
		return;
	}

	// Add shaders
	AddShader(shaderId, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderId, fragmentCode, GL_FRAGMENT_SHADER);

	// Check for errors
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

// Method used to create shader from shader paths
void Shader::CreateShader(const char* vertexPath, const char* fragmentPath)
{
	// Read shader files
	std::string vertexCode = ReadFile(vertexPath);
	std::string fragmentCode = ReadFile(fragmentPath);

	// Convert to char pointers
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Compile shaders
	CompileShader(vShaderCode, fShaderCode);
}

// Method used to use shader
void Shader::UseShader() {
	glUseProgram(shaderId);
}

// Method used while destructing the object
void Shader::ClearShader() {
	if (shaderId != 0) {
		glDeleteProgram(shaderId);
		shaderId = 0;
	}
}

// Destructor
Shader::~Shader()
{
	ClearShader();
}
