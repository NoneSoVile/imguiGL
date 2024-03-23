#pragma once
#include "Sprite2d.h"
class ModelObj;
class ModelGL;
#define MAX_LIGHTS 15
class Mesh3d : public Sprite2d {
protected:
    std::shared_ptr<ModelObj> waterModel;
	std::shared_ptr<ModelGL> waterModelGl;

    //gui data
    bool show_lighting_settings = 0;
    bool show_water_settings = 0;
    bool useTexture = 0;
    vec3f eye = vec3f(0.f, 1.f, 3.f);
    vec3f lookat;
    vec3f model_rot;
    vec3f model_scale= vec3f(3, 1.f, 3.f);
    vec3f model_translation;

    //light
    vec3f lightColors[MAX_LIGHTS];
    vec3f lightPositions[MAX_LIGHTS];
    int lightNum = 3;

    //simualation
#define MAX_WAVES 20
    vec2f waves_D[MAX_WAVES];
    vec3f waves_AWP[MAX_WAVES];
    int waveCount = 3;
    float timestep = 0.01;
protected:
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);
    virtual void stepSimulation(float w, float h,float dt);
    virtual void initSimulation(int w, int h);

    void saveArrayData();
    void loadArrayData();
    void saveWavesData();
    void loadWavesData();

public:
    virtual void run(float w, float h);

};