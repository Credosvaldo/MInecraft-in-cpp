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
    Shader *myMaterial;
    bool tem[6];
    mat4 model;

    BlockType type;

public:
    vector<int>faces;

    Cube(Shader *myMaterial, vec3 position, BlockType type, vector<int>& faces)
    {
        this->myMaterial = myMaterial;
        this->transform->position = position;
        this->type = type;
        this->faces = faces;

        this->model = mat4(1.0);
        this->model = translate(model, transform->position);  

    }

    void Draw()
    {
        myMaterial->setMatrix("model", model);
        CubeMesh::Draw(faces, type);
    }

    void Culling(const vector<Cube*>& cubes)
    {
        memset(tem, 0, sizeof(tem));
        for(const auto& cubo : cubes)
        {
            if(cubo->transform->position.x == transform->position.x)
            {
                if(cubo->transform->position.y == transform->position.y)
                {
                    if(cubo->transform->position.z - 1 == transform->position.z)
                    {
                        tem[1] = true;//Frente
                    }
                    else if(cubo->transform->position.z + 1 == transform->position.z)
                    {
                        tem[3] = true;//Tras
                    }
                }
                else if(cubo->transform->position.z == transform->position.z)
                {
                    if(cubo->transform->position.y - 1 == transform->position.y)
                    {
                        tem[0] = true;//Cima
                    }
                    else if(cubo->transform->position.y + 1 == transform->position.y)
                    {
                        tem[2] = true;//Baixo
                    }
                }
            }
            else if(cubo->transform->position.y == transform->position.y)
            {
                if(cubo->transform->position.z == transform->position.z)
                {
                    if(cubo->transform->position.x - 1 == transform->position.x)
                    {
                        tem[4] = true;//Direita
                    }
                    else if(cubo->transform->position.x + 1 == transform->position.x)
                    {
                        tem[5] = true;//Esquerda
                    }
                }
            }

        }
    }

    void Start()
    {

    }

    void Update()
    {

    }

};



#endif