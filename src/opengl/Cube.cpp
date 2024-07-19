#include "../Header/Cube.h"

Cube::Cube() :
    vboID(0),
    iboID(0),
    position(0.0f),
    scale(1.0f),
    rotationAngle(0.0f),
    axis(1.0f),
    model(glm::mat4(1.0f))
    // Initialize vertex and index data
    {
    // Color definitions
    GLfloat redColor[] = { 1.0f, 0.0f, 0.0f };
    GLfloat greenColor[] = { 0.0f, 1.0f, 0.0f };
    GLfloat blueColor[] = { 0.0f, 0.0f, 1.0f };
    GLfloat yellowColor[] = { 1.0f, 1.0f, 0.0f };
    GLfloat cyanColor[] = { 0.0f, 1.0f, 1.0f };
    GLfloat magentaColor[] = { 1.0f, 0.0f, 1.0f };
    // VBO data for a cube
    vertexData = {
        // Front face (Blue)
        -0.5f, -0.5f,  0.5f, blueColor[0], blueColor[1], blueColor[2],
         0.5f, -0.5f,  0.5f, blueColor[0], blueColor[1], blueColor[2],
         0.5f,  0.5f,  0.5f, blueColor[0], blueColor[1], blueColor[2],
        -0.5f,  0.5f,  0.5f, blueColor[0], blueColor[1], blueColor[2],

        // Back face (Red)
        -0.5f, -0.5f, -0.5f, redColor[0], redColor[1], redColor[2],
         0.5f, -0.5f, -0.5f, redColor[0], redColor[1], redColor[2],
         0.5f,  0.5f, -0.5f, redColor[0], redColor[1], redColor[2],
        -0.5f,  0.5f, -0.5f, redColor[0], redColor[1], redColor[2],

        // Left face (Green)
        -0.5f, -0.5f, -0.5f, greenColor[0], greenColor[1], greenColor[2],
        -0.5f, -0.5f,  0.5f, greenColor[0], greenColor[1], greenColor[2],
        -0.5f,  0.5f,  0.5f, greenColor[0], greenColor[1], greenColor[2],
        -0.5f,  0.5f, -0.5f, greenColor[0], greenColor[1], greenColor[2],

        // Right face (Cyan)
        0.5f, -0.5f, -0.5f, cyanColor[0], cyanColor[1], cyanColor[2],
        0.5f, -0.5f,  0.5f, cyanColor[0], cyanColor[1], cyanColor[2],
        0.5f,  0.5f,  0.5f, cyanColor[0], cyanColor[1], cyanColor[2],
        0.5f,  0.5f, -0.5f, cyanColor[0], cyanColor[1], cyanColor[2],

        // Top face (Yellow)
        -0.5f,  0.5f, -0.5f, yellowColor[0], yellowColor[1], yellowColor[2],
         0.5f,  0.5f, -0.5f, yellowColor[0], yellowColor[1], yellowColor[2],
         0.5f,  0.5f,  0.5f, yellowColor[0], yellowColor[1], yellowColor[2],
        -0.5f,  0.5f,  0.5f, yellowColor[0], yellowColor[1], yellowColor[2],

        // Bottom face (Magenta)
        -0.5f, -0.5f, -0.5f, magentaColor[0], magentaColor[1], magentaColor[2],
         0.5f, -0.5f, -0.5f, magentaColor[0], magentaColor[1], magentaColor[2],
         0.5f, -0.5f,  0.5f, magentaColor[0], magentaColor[1], magentaColor[2],
        -0.5f, -0.5f,  0.5f, magentaColor[0], magentaColor[1], magentaColor[2],
    };

    // IBO data for a cube
    indexData = {
        0, 1, 2, // first triangle
        2, 3, 0, // second triangle

        // top
        4, 5, 6, // first triangle
        6, 7, 4, // second triangle

        // left
        8, 9, 10,  // first triangle
        10, 11, 8, // second triangle

        // right
        14, 13, 12, // 12, 13, 14, // first triangle
        12, 15, 14, // 14, 15, 12, // second triangle

        // back
        18, 17, 16, // 16, 17, 18, // first triangle
        16, 19, 18, // 18, 19, 16, // second triangle

        // bottom
        20, 21, 22, // first triangle
        22, 23, 20  // second triangle
    };
    }
Cube::~Cube()
{
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &iboID);
}

void Cube::initialize()
{

    // Generate and bind VBO
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

    // Generate and bind IBO
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    
}

void Cube::update(float deltaTime)
{
    accumulatedTime += deltaTime;
    //float timeElapsed = accumulatedTime / 1000.0f;

    //Does scaling, translation, rotation, & axis change lerp based on time
    applySimpleTransformation(deltaTime);

    model = glm::translate(glm::mat4(0.1f), position) * glm::scale(glm::mat4(0.1f), scale) * glm::rotate(glm::mat4(0.1f), rotationAngle, axis);
}

void Cube::render(GLuint gProgramID)
{
    glUseProgram(gProgramID);

    // Bind VBO and IBO
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);

    // Set vertex attribute pointers
    gVertexPos3DLocation = glGetAttribLocation(gProgramID, "LVertexPos3D");
    vertexColorLocation = glGetAttribLocation(gProgramID, "LVertexColor");

    glEnableVertexAttribArray(gVertexPos3DLocation);
    glVertexAttribPointer(gVertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

    glEnableVertexAttribArray(vertexColorLocation);
    glVertexAttribPointer(vertexColorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    
    // Set the model matrix uniform
    glUniformMatrix4fv(glGetUniformLocation(gProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexData.size()), GL_UNSIGNED_INT, 0);


    glDisableVertexAttribArray(gVertexPos3DLocation);
    glDisableVertexAttribArray(vertexColorLocation);
}



void Cube::setPosition(const glm::vec3 pos)
{
    position = pos;

}

void Cube::setScale(const glm::vec3 scale)
{
    this->scale = scale;

}

void Cube::setRotationAngle(float rotationAngle)
{
    this->rotationAngle = rotationAngle;
}

void Cube::setAxis(const glm::vec3 axis)
{
    this->axis = axis;

}

void Cube::applySimpleTransformation(float deltaTime)
{
    float time = accumulatedTime / 1000.0f;
    setAxis(glm::vec3(sin(time), cos(time), 0.0f));
    setPosition(glm::vec3(2.0f * cos(time), sin(2.0f * time), cos(2.0f * time)));
    setScale(glm::vec3(sin((time)+1.0f)));
    setRotationAngle(time);
}
