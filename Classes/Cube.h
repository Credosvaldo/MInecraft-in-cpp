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

    BlockType type;

public:
    Cube(Shader *myMaterial, vec3 position, BlockType type)
    {
        this->myMaterial = myMaterial;
        this->transform->position = position;
        this->type = type;
        //cout<< "contruiu um na pos: " << position.x << " " << position.y << " " << position.z << "\n";
    }

    void Draw()
    {
        mat4 model(1.0f);
        model = translate(model, transform->position);  

        myMaterial->setMatrix("model", model);
        CubeMesh::Draw(tem, type);
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