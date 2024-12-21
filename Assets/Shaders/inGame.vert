#version 460 core
layout(location = 0)in vec4 pos;
layout(location = 2)in vec2 auv;

out vec2 uv;
out vec4 Light;

uniform mat4 model;
uniform vec3 light;
uniform mat4 viewMatrix;
uniform mat4 projection;

void main(){
    uv = auv;
    vec4 position = pos;
    Light = vec4(light, 0.0f);
    gl_Position = projection * viewMatrix * model * position;
}