#pragma once
#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    Cube();
    ~Cube();

    void initialize();
    void update(float deltaTime);
    void render(GLuint ProgramID);

    void setPosition(const glm::vec3 pos);
    void setScale(const glm::vec3 scale);
    void setRotationAngle(float rotationAngle);
    void setAxis(const glm::vec3 axis);

    //Does scaling, translation, rotation, & axis change lerp based on time
    void applySimpleTransformation(float deltaTime);

private:
    GLuint vboID;
    GLuint iboID;
    GLint vertexColorLocation;
    GLint gVertexPos3DLocation = -1;

    std::vector<GLfloat> vertexData;
    std::vector<GLuint> indexData;

    glm::mat4 model;
    glm::vec3 position;
    glm::vec3 scale;
    float rotationAngle;
    glm::vec3 axis;

    float accumulatedTime = 0.0f;
};