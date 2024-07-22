#pragma once

#include <gl/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use();
    GLuint getShader() const;

private:

    //unsigned int vertexShaderID;
    //unsigned int fragmentShaderID;
    GLuint shaderProgram;

    //Vertext Shader
    GLuint vertexShaderID;
    std::string vertexFilename;
    std::string vertexFileData;
    const char* vertexSource;

    //Fragment Shader
    GLuint fragmentShaderID;
    std::string fragmentFilename;
    std::string fragmentFileData;
    const char* fragmentSource;


    std::string readFile(const std::string& filePath);
    GLuint compileShader(const char* source, GLuint shaderID);
    GLuint linkShaders(GLuint vertexShaderID, GLuint fragmentShaderID);

    void printShaderLog(GLuint shader);
    void printProgramLog(GLuint program);
};