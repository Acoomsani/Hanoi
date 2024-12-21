#version 330 core

in vec2 uv;
in vec4 Light;

out vec4 FragColor;
uniform sampler2D pictureSampler;

void main(){
    vec4 color = texture(pictureSampler, uv);
    FragColor = color + Light;
}