#version 310 es
precision highp float;
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
out highp vec2 vTexCoord;
out highp vec4 vPosition;
out highp vec4 vPositionMV;
out highp vec3 vNormal;
out highp vec3 vNormalMV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mediump mat4 uProjection;
uniform mediump mat4 u_inv_MV;
uniform mediump float u_time;

#define MAX_WAVES 20
uniform int waveCount;
uniform vec2 waves_D[MAX_WAVES];
uniform vec3 waves_AWP[MAX_WAVES];
uniform float time;
const float PI = 3.1415926;

void main() {
    mat4 MVP = projection*view*model;
    mat4 MV = view*model;
    

    vec4 position4 = vec4(position, 1.0f);
    //water height of vertices calculation

    float dpdx = 0.0;
    float dpdz = 0.0;
    for(int i = 0; i < waveCount; i++){
        vec3 AWP = waves_AWP[i];
        vec2 D = waves_D[i];
        float A = AWP.x*1.0;
        float W = AWP.y*130.0;
        float P = AWP.z*10.0;
        vec2 posXZ = position4.xz;
        position4.y += A * sin(dot(D, posXZ) * W + time * P);
        dpdx += W * D.x* A* cos(dot(D, posXZ) * W + time * P);
        dpdz += W * D.y* A* cos(dot(D, posXZ) * W + time * P);
    }

    
    
        //
    //
    vPosition = position4;
    vNormal = vec3(-dpdx, 1.0, -dpdz);//normal;
    vTexCoord = texCoord;

    vec4 p = MV*position4;
    mat4 inverse_mv = transpose(inverse(MV));
    vec3 n = (inverse_mv*vec4(vNormal, 0.0)).xyz;//vec3(0.0, 0.0, 1.0);
    vPositionMV = p;
    vNormalMV = n = normalize(n);
    gl_Position = MVP*position4;
}