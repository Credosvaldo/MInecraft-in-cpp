#version 330 core

in vec3 Cor;
in vec2 TextCords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float amount = 0.2f;

void main()
{
    gl_FragColor = vec4(Cor, 1.0f) * mix(texture(texture1, TextCords), texture(texture2, TextCords), 0.2f);
}