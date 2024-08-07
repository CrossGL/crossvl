#include "../Header/Shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
  // Get vertex source
  vertexFilename = vertexPath;
  vertexFileData = readFile(vertexFilename);
  vertexSource = vertexFileData.c_str();

  // Get fragment source
  fragmentFilename = fragmentPath;
  fragmentFileData = readFile(fragmentFilename);
  fragmentSource = fragmentFileData.c_str();

  vertexShaderID = compileShader(vertexSource, GL_VERTEX_SHADER);
  fragmentShaderID = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

  shaderProgram = linkShaders(vertexShaderID, fragmentShaderID);
}

Shader::~Shader() {
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
  glDeleteProgram(shaderProgram);
}

void Shader::use() { glUseProgram(shaderProgram); }

GLuint Shader::getShader() const { return shaderProgram; }

std::string Shader::readFile(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filePath << std::endl;
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint Shader::compileShader(const char *source, GLuint shaderID) {
  GLint shader;
  shader = glCreateShader(shaderID);

  const char *shaderSource = source;
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  GLint success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  switch (shaderID) {
  case GL_VERTEX_SHADER:
    if (!success) {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
    break;
  case GL_FRAGMENT_SHADER:
    if (!success) {
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                << infoLog << std::endl;
    }
    break;
  }

  return shader;
}

GLuint Shader::linkShaders(GLuint vertexShaderID, GLuint fragmentShaderID) {
  GLuint shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShaderID);
  glAttachShader(shaderProgram, fragmentShaderID);
  glLinkProgram(shaderProgram);

  GLint success;
  char infoLog[512];

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  return shaderProgram;
}

void Shader::printShaderLog(GLuint shader) {
  if (glIsShader(shader)) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    char *infoLog = new char[maxLength];
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0) {
      std::cout << infoLog << std::endl;
    }
    delete[] infoLog;
  } else {
    std::cout << "Name " << shader << " is not a shader" << std::endl;
  }
}

void Shader::printProgramLog(GLuint program) {
  if (glIsProgram(program)) {
    int infoLogLength = 0;
    int maxLength = infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    char *infoLog = new char[maxLength];
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0) {
      std::cout << infoLog << std::endl;
    }
    delete[] infoLog;
  } else {
    std::cout << "Name " << program << " is not a program" << std::endl;
  }
}
