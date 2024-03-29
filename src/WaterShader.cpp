#include "WaterShader.h"
#include "Shader.h"
#include "ModelObj.h"
#include "ModelGL.h"
#include "FileConfig.h"
#include <random>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"

void WaterShader::loadShader() {
    /*create shaders
    */
    string vertexShaderFile = resourceFolder + std::string("shaders/water.vert");
    string fragShaderFile = resourceFolder + std::string("shaders/water2.frag");

    renderShader = std::make_shared<Shader>();
    renderShader->setShader(vertexShaderFile.c_str(), fragShaderFile.c_str());
    if (!renderShader->isValid()) {
        printf("failed to create shader: %s\n", vertexShaderFile.c_str());
    }
    else {
        printf("succeeded to create shader: %s  programid = %d\n", 
        vertexShaderFile.c_str(), renderShader->program);
    }
}

void WaterShader::loadMesh() {
    string waterModelFile = resourceFolder + "models/plane300x300.obj";
    waterModel.reset((ModelObj*)ModelObj::CreateFromObjFile(waterModelFile.c_str(), 1.0f, false, false, false));
    if (!waterModel) {
        printf("failed to load water model object: %s \n", waterModelFile.c_str());
    }
    else {
        printf("succeeded to load water model object: %s\n", waterModelFile.c_str());
        waterModelGl.reset(ModelGL::CreateFromModel(waterModel.get()));
        if(waterModelGl){
            printf("succeeded to load water model object : %s as GL memory OBject \n", waterModelFile.c_str());
        }
    }
}

void WaterShader::loadTexture() {
    Sprite2d::loadTexture();
}


#define CONFIG_ADD(var) fileConfig.Add((#var), var)
#define CONFIG_READ_VEC3F(var) var = fileConfig.Readvec3f(#var)
#define CONFIG_READ(var) var = fileConfig.Read(#var, var)


void WaterShader::drawSprite(int w, int h, vec2f offset) {
    vec3f eye = vec3f(this->eye.x, this->eye.y, this->eye.z);
    vec3f at(lookat.x, lookat.y, lookat.z);
    vec3f up(0, 1, 0);

    float w_view = 1.0f, h_view = 1.0f;
    float near = 1;      
    float far = 10000.0f;
    float aspect = w_view / h_view;
    float fovy = PI / 2.0;

    matrix4f model, view, projection;
    matrix4f r, rx, ry, rz, t, s;
    r = MB::rotationY(r, 0.0f);
    t.set_translate(model_translation);
    s.set_scale(model_scale);
    rx = MB::rotationX(rx, model_rot.x);
    ry = MB::rotationY(ry, model_rot.y);
    rz = MB::rotationZ(rz, model_rot.z);
    r = rx * ry * rz;
    model = t * r * s;

    MB::lookAt(view, eye, at, up);
    MB::perspective(projection, fovy,
        aspect,
        near, far);
    float specularPower = model_specularPower;
    float diffusePower = model_diffusePower;
    
    renderShader->Use(12);
    renderShader->setUniformMatrix4fv("model", model._array, 1, GL_FALSE);
    renderShader->setUniformMatrix4fv("view", view._array, 1, GL_FALSE);
    renderShader->setUniformMatrix4fv("projection", projection._array, 1, GL_FALSE);
    /*renderShader->setUniform1f("specularPower", specularPower);
    renderShader->setUniform1f("diffusePower", diffusePower);
    renderShader->setUniform1i("useTexture", useTexture);
    renderShader->setUniform1i("lightNum", lightNum);
    renderShader->setUniform3fv("lights", (const float*)lightPositions, lightNum);
    renderShader->setUniform3fv("lights_Color", (const float*)lightColors, lightNum);
    renderShader->setUniform1f("alphaColor", alphaColor);*/

    glBindTexture(GL_TEXTURE_2D, ourTexture);
    waterModelGl->drawElements(0, 2, 1, usePoints, useLines);
}

void WaterShader::stepSimulation(float w, float h, float dt) {
    static float time = 0;
    static int cycle = 0;
    time += dt;
    ++cycle;
    const int num = 1;
    const int cycleCount = 300;
    if (transitionWave) {
        if (cycle % cycleCount == 0) {
            updateRandWavesData(num);
        }
        else if (curPivotWave >= 0) {
            int start = (curPivotWave - num + waveCount) % waveCount;
            float t = 1.0 * (cycle % cycleCount) / cycleCount;
            transitionWavesValue(start, num, t);
        }
    }


    renderShader->Use(12);
    renderShader->setUniform1f("time", time);
    renderShader->setUniform1f("waves_Power", wavePower);
    renderShader->setUniform1i("waveCount", waveCount);
    renderShader->setUniform2fv("waves_D", (const float*)waves_D, waveCount);
    renderShader->setUniform3fv("waves_AWP", (const float*)waves_AWP, waveCount);
}

void WaterShader::saveWavesData() {
    string path = resourceFolder + "watershader/waves_data.txt";
    Config fileConfig;
    fileConfig.LoadConfig(path);

    for (size_t i = 0; i < MAX_WAVES; i++)
    {
        std::string key = std::string("waves_D") + std::to_string(i);
        fileConfig.Add(key, waves_D[i]);
    }

    for (size_t i = 0; i < MAX_WAVES; i++)
    {
        std::string key = std::string("waves_AWP") + std::to_string(i);
        fileConfig.Add(key, waves_AWP[i]);
    }
    CONFIG_ADD(waveCount);
    fileConfig.Save();
}

void WaterShader::loadWavesData() {
    string path = resourceFolder + "watershader/waves_data.txt";
    Config fileConfig;
    fileConfig.LoadConfig(path);

    for (size_t i = 0; i < MAX_WAVES; i++)
    {
        std::string key = std::string("waves_D") + std::to_string(i);
        waves_D[i] = fileConfig.Readvec2f(key);
    }

    for (size_t i = 0; i < MAX_WAVES; i++)
    {
        std::string key = std::string("waves_AWP") + std::to_string(i);
        waves_AWP[i] = fileConfig.Readvec3f(key);
    }
    CONFIG_READ(waveCount);
}

void WaterShader::saveModelViewData(){
    string path = resourceFolder + "watershader/model_view_data.txt";
    Config fileConfig;
    fileConfig.LoadConfig(path);

    CONFIG_ADD(model_scale);
    CONFIG_ADD(model_translation);
    CONFIG_ADD(model_rot);
    CONFIG_ADD(lookat);
    CONFIG_ADD(eye);

    fileConfig.Save();
}

void WaterShader::loadModelViewData(){
    string path = resourceFolder + "watershader/model_view_data.txt";
    Config fileConfig;
    fileConfig.LoadConfig(path);
    CONFIG_READ_VEC3F(model_scale);
    CONFIG_READ_VEC3F(model_translation);
    CONFIG_READ_VEC3F(model_rot);
    CONFIG_READ_VEC3F(lookat);
    CONFIG_READ_VEC3F(eye);
}


