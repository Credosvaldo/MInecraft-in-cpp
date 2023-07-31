#ifndef CUBE_H
#define CUBE_H  

#include <vector>

#include "GameObject.h"
#include "CubeMesh.h"
#include "shader.h"

#include "../Enums/BlockType.h"


class Cube : public GameObject
{
private:

public:
    BlockType type;

    Cube(vec3 position, BlockType type)
    {
        this->transform->position = position;
        this->type = type;

    }

    void Start()
    {

    }

    void Update()
    {

    }

};



#endif