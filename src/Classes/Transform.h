#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Transform
{
private:

public:
    vec3 position;
    vec3 rotation;
    vec3 scale;

    vec3 forward;
    vec3 up;
    vec3 right;

    Transform(vec3 position = vec3(0.0f), vec3 rotation = vec3(0.0f), vec3 scale = vec3(0.0f))
    {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

        this->forward = vec3(0.0f, 0.0f, 1.0f);
        this->up = vec3(0.0f, 1.0f, 0.0f);
        this->right = vec3(1.0f, 0.0f, 0.0f);
    }

    void TransformUpdate()
    {
        right = cross(forward, up);
        up = cross(right, forward);

    }

};



#endif