#pragma once
#include "Sprite2d.h"
class ModelObj;
class ModelGL;
class Mesh3d : public Sprite2d {
protected:
    std::shared_ptr<ModelObj> waterModel;
	std::shared_ptr<ModelGL> waterModelGl;

    //gui data
    vec3f eye = vec3f(0.f, 1.f, 3.f);
    vec3f lookat;
    vec3f model_rot;
    vec3f model_scale=1;
    vec3f model_translation;
protected:
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();

    virtual void updateUI(int w, int h);
    virtual void drawSprite(int w, int h, vec2f offset);
    virtual void stepSimulation(float w, float h,float dt);
    virtual void initSimulation(int w, int h);

public:
    virtual void run(float w, float h);

};