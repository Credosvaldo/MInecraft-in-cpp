#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCor;
layout (location = 2) in vec2 aTextCords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 Cor;
out vec2 TextCords;

void main()
{
    gl_Position =  projection * view *  model *  vec4(aPos, 1.0f);

    Cor = aCor;
    TextCords = aTextCords;

}