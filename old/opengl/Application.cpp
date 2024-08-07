#include "../Header/Application.h"
#include <filesystem>

Application::Application()
    : window(nullptr)
    , gContext(nullptr)
    , shader(nullptr)
    , startTime(0.0f)
    , endTime(0.0f)
    , model(glm::mat4(1.0f))  // Identity matrix
{
    cube = new Cube();
}

Application::~Application()
{
    close();
}

bool Application::initSDL()
{
    // Initialization flag

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        sdlSuccess = false;
    }
    else
    {
        // Use OpenGL 3.1 core
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        window = SDL_CreateWindow("Visualizer-SDL2OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if (window == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            sdlSuccess = false;
        }
        else
        {
            // Create context
            gContext = SDL_GL_CreateContext(window);
            if (gContext == NULL)
            {
                printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
                sdlSuccess = false;
            }
            else
            {
                // Initialize GLEW
                glewExperimental = GL_TRUE;
                glewError        = glewInit();
                if (glewError != GLEW_OK)
                {
                    printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
                }

                // Use Vsync
                if (SDL_GL_SetSwapInterval(1) < 0)
                {
                    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
                }

                // Initialize OpenGL
                shader = new Shader("Resources/Shader.vert", "Resources/Shader.frag");
                if (!shader || shader->getShader() == 0)
                {
                    printf("Failed to create Shader!\n");
                    sdlSuccess = false;
                }
                if (!initGL())
                {
                    printf("Unable to initialize OpenGL!\n");
                }
                // Initialize ImGui
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO();
                (void)io;
                ImGui_ImplSDL2_InitForOpenGL(window, gContext);
                ImGui_ImplOpenGL3_Init("#version 330");
            }
        }
    }
    return sdlSuccess;
}

bool Application::initGL()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    // glCullFace(GL_BACK); // Cull back faces
    glFrontFace(GL_CCW);  // Counter-clockwise winding considered front

    // Initialize the shader
    shader->use();
    gProgramID = shader->getShader();

    // Get attribute and uniform locations
    modelLoc      = glGetUniformLocation(gProgramID, "model");
    viewLoc       = glGetUniformLocation(gProgramID, "view");
    projectionLoc = glGetUniformLocation(gProgramID, "projection");

    // Initialize clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Set the projection and view matrices
    projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    shader->use();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    shader->use();

    cube->initialize();

    return true;
}

void Application::run()
{
    if (!initSDL())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Deltatime calci
        startTime = static_cast<float>(SDL_GetTicks());

        // Enable text input
        SDL_StartTextInput();

        // While application is running
        while (isWindowOpen)
        {
            endTime        = static_cast<float>(SDL_GetTicks());
            float deltTime = endTime - startTime;
            startTime      = endTime;

            // Handle events on queue
            handleEvents();

            // Updates
            update(deltTime);

            // Render cube
            render();

            // Update screen
            SDL_GL_SwapWindow(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        // Disable text input
        SDL_StopTextInput();
    }

    // Free resources and close SDL
    close();
}

void Application::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&e);

        // User requests quit
        if (e.type == SDL_QUIT)
        {
            isWindowOpen = false;
        }
        // Handle keypress
        else if (e.type == SDL_TEXTINPUT)
        {
            if (e.text.text[0] == 'q')
            {
                gRenderCube = !gRenderCube;
            }
        }
    }
}

void Application::update(float deltaTime)
{
    // Calculate time elapsed
    accumulatedTime += deltaTime;
    float time = accumulatedTime / 1000.0f;

    // Cube update
    cube->update(deltaTime);

    // Update the shader uniform
    shader->use();
}

void Application::render()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render cube
    if (gRenderCube)
    {
        shader->use();

        cube->render(gProgramID);

        shader->use();
    }
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    handleImGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::close()
{
    // Deallocate program
    glDeleteProgram(gProgramID);

    if (shader)
    {
        delete shader;
        shader = nullptr;
    }
    if (cube)
    {
        delete cube;
        cube = nullptr;
    }

    // Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

void Application::initImGui()
{
}

void Application::handleImGui()
{
    if (ImGui::Begin("Shader Loader"))
    {
        if (ImGui::Button("Load Vertex Shader"))
        {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseVertexShader", "Choose Vertex Shader", ".vert,.glsl,.txt");
        }

        if (ImGui::Button("Load Fragment Shader"))
        {
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFragmentShader", "Choose Fragment Shader",
                                                    ".frag,.glsl,.txt");
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseVertexShader"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                strcpy_s(vertexPathAdd, 256, filePathName.c_str());
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseFragmentShader"))
        {
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                strcpy_s(fragmentPathAdd, 256, filePathName.c_str());
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGui::Button("Load Shaders"))
        {
            if (strlen(vertexPathAdd) > 0 && strlen(fragmentPathAdd) > 0)
            {
                fileSelectSucceed = true;

                shader = new Shader(std::filesystem::relative(vertexPathAdd).string(),
                                    std::filesystem::relative(fragmentPathAdd).string());

                if (!shader || shader->getShader() == 0)
                {
                    printf("Failed to create Shader!\n");
                    sdlSuccess = false;
                }
                else
                {
                    printf("Shader created SuccessFully!\n");
                }
                if (!initGL())
                {
                    printf("Unable to initialize OpenGL!\n");
                }
            }
            else
            {
                std::cerr << "Please select both vertex and fragment shader files." << std::endl;
            }
        }

        ImGui::End();
    }
}
