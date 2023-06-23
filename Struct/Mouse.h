#ifndef MOUSE_H
#define MOUSE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


struct Mouse
{
    unsigned char button[3];
    vec2 position;
    vec2 deltaPosition;

    Mouse()
    {
        memset(button, 0, sizeof(button));
        position = vec2(0.0f);
        deltaPosition = vec2(0.0f);
    }
    
};




#endif