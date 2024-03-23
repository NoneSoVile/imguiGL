#include "Mesh3d.h"
#include "Shader.h"
#include "ModelObj.h"
#include "ModelGL.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"

void Mesh3d::loadShader() {
    /*create shaders
    */
    string vertexShaderFile = resourceFolder + std::string("shaders/basic3d.vert");
    string fragShaderFile = resourceFolder + std::string("shaders/basic3d.frag");

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

void Mesh3d::loadMesh() {
    string waterModelFile = resourceFolder + "models/plane100x100.obj";
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

void Mesh3d::loadTexture() {
    Sprite2d::loadTexture();
}

void Mesh3d::updateUI(int w, int h) {
// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::SetNextWindowBgAlpha(0.5f);
    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Helloxxx, world!", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
        ImGui::Text("3D Settings");              

        ImGui::SliderFloat3("modle scale", (float*)&model_scale, .0f, 12.0f);
        ImGui::SliderFloat3("model rotation", (float*)&model_rot, -PI * 2, PI * 2);
        ImGui::SliderFloat3("modle translation", (float*)&model_translation, -30.f, 30.0f);
        ImGui::SliderFloat3("lookAt vector", (float*)&lookat, -32.0f, 32.0f);
        ImGui::SliderFloat3("eye vector", (float*) &eye, -12.0f, 12.0f);

        ImGui::SliderFloat("diffusePower", (float*)&car_model_diffusePower, .0f, 12.0f);
        ImGui::SliderFloat("specularPower", (float*)&car_model_specularPower, .0f, 1300.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void Mesh3d::drawSprite(int w, int h, vec2f offset) {
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
    float specularPower = car_model_specularPower;
    float diffusePower = car_model_diffusePower;
    
    vec3f lightPositions[] = {
        vec3f(200, 200, 200),
        vec3f(-200, 200, 100),
        vec3f(0, 100, -100),
    };
    vec3f lightColors[] = {
        vec3f(0.5, 0.5, 0.5),
        vec3f(0.6, 0.6, 0.6),
        vec3f(1, 1, 1),
    };
    float lightNum = sizeof(lightPositions) / sizeof(lightPositions[0]);
    renderShader->Use(12);
    renderShader->setUniformMatrix4fv("model", model._array, 1, GL_FALSE);
    renderShader->setUniformMatrix4fv("view", view._array, 1, GL_FALSE);
    renderShader->setUniformMatrix4fv("projection", projection._array, 1, GL_FALSE);
    renderShader->setUniform1f("specularPower", specularPower);
    renderShader->setUniform1f("diffusePower", diffusePower);
    renderShader->setUniform1i("lightNum", lightNum);
    renderShader->setUniform3fv("lights", (const float *)lightPositions, lightNum);
    renderShader->setUniform3fv("lights_Color", (const float *)lightColors, lightNum);

    // int tex_surround = m_GLSurround->GetTexture(DATA_FRONT, false);
    glBindTexture(GL_TEXTURE_2D, ourTexture);
    waterModelGl->drawElements(0, 2, 1);
}

void Mesh3d::stepSimulation(float w, float h, float dt) {
    
}

void Mesh3d::initSimulation(int w, int h) {
    
}

void Mesh3d::run(float w, float h)
{
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    static float dt = 0;
	stepSimulation(w, h, dt);
    glViewport(0, 0, w, h);
	drawSprite(w, h, vec2f(0,0));
	
    updateUI(w, h);
}
