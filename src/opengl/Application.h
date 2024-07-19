#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//IMGUI
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "ImGuiFileDialog.h"

//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include <SDL.h>
#undef main
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Cube.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

class Application {
public:
    Application();
    ~Application();
    void run();
    char* vertexPathAdd = new char[256] {""};
    char* fragmentPathAdd = new char[256] {""};
private:
    bool initSDL();
    bool initGL();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void close();

    //IMGUI
    void initImGui();
    void handleImGui();


    float accumulatedTime = 0.0f;

    bool sdlSuccess = true;
    bool openGLSuccess = true;
    bool isWindowOpen = true;
    bool fileSelectSucceed = false;
    SDL_Window* window;
    SDL_GLContext gContext;
   
    GLenum glewError;

    //Graphics program
    GLuint gProgramID = 0;

    //SHADER
    Shader* shader;

    Cube* cube;

    //uniform locations
    GLint modelLoc;
    GLint viewLoc;
    GLint projectionLoc;


    //projection matrix
    glm::mat4 projection;

    // Set the view matrix
    glm::mat4 view;

    // Set model matrix (rotation)
    glm::mat4 model;

    bool gRenderCube = true;

    float startTime;
    float endTime;

};