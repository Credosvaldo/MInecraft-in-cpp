#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

struct Vertex
{
    vec3 position;
    vec3 color;
    vec2 texture;

    Vertex(vec3 _position = vec3(0.0f), vec3 _color = vec3(1.0f), vec2 _texture = vec3(0.0f)) : position(_position), color(_color), texture(_texture)
    {

    }

};


#endif