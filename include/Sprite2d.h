#pragma once
#include "Common.h"
#include "ShaderTester.h"
#include <string>
#include "NvMath.h"
using MB::vec2i;
using MB::vec2f;

class Shader;
class Sprite2d: public ShaderTester
{
public:
//set all of the properties before call the init() function
//locate the sprite2d in screen view port coordinate
    vec2f screenDim;
    vec2f pixelSize;
    vec2f pixelPosition;


protected:
    std::shared_ptr<Shader> renderShader;
    GLuint VBO;
	GLuint VAO;
	GLuint EBO;

    GLuint writeTexture = 0;
    GLuint ourTexture = 0;


    vec2f ndcSize;
    vec2f ndcPosition;

    vec2f scale;
    vec2f translation;

    //physics model data
    vec2f F; 
    vec2f T; // thrust
    vec2f A;
    float C; // drag coefficient
    vec2f V; // velocity
    float M; // mass
    vec2f S; // displacement


protected:
    void loadMesh();
    void loadTexture();

    void updateUI(int w, int h);
    void drawSprite(int w, int h);
    void stepSimulation(float w, float h,float dt);
    void initSimulation();

    //physics model options
    void step0(float dt);
    void step1(float dt);

public:
    void init();
    void run(float w, float h);

public:
    vec2f ndcToScreen(vec2f ndcPosition);
    vec2f screenToNdc(vec2f screenPosition);


};