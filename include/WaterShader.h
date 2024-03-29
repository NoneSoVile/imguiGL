#pragma once
#include "Mesh3d.h"
class ModelObj;
class ModelGL;
#define MAX_LIGHTS 15
class WaterShader : public Mesh3d {
protected:
    virtual void loadShader();
    virtual void loadMesh();
    virtual void loadTexture();

    virtual void drawSprite(int w, int h, vec2f offset);
    virtual void stepSimulation(float w, float h,float dt);

    virtual void saveWavesData();
    virtual void loadWavesData();

    virtual void saveModelViewData();
    virtual void loadModelViewData();

};