#ifndef CUBEMESH_H
#define CUBEMESH_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "../Struct/Vertex.h"
#include "../Enums/BlockType.h"

#include "Cube.h"


/*
 0  Topo
 1  Frente
 2  Embaixo
 3  Atras
 4  Direita
 5  Esquerda
*/

const float tc0 = (1.0f/3.0f) * 0;
const float tc1 = (1.0f/3.0f) * 1;
const float tc2 = (1.0f/3.0f) * 2;
const float tc3 = (1.0f/3.0f) * 3;

class CubeMesh
{
private:
    static Vertex vertex[24];
    static unsigned int elements[6][6];
    static BlockType lastBlockType;
    static bool criado;

    static int TextureID(BlockType type)
    {
        
        switch (type)
        {
            case BLOCK_GRASS:
                return 0;
                break;
            
            case BLOCK_COBLESTONE:
                return 1;
                break;

            default:
                return -1;
                break;
        }
        
    }

public:
    static unsigned int VAO;
    static unsigned int VBO;
    static unsigned int VBI;
    static unsigned int EBOs[6];
    static int test;

    static void Init()
    {   
        //Gerar Buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(6, EBOs);

        //BIND VAO
        glBindVertexArray(VAO);

        #pragma region VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        #pragma endregion

        #pragma region  EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements[0]), elements[0], GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements[1]), elements[1], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements[2]), elements[2], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements[3]), elements[3], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[4]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements[4]), elements[4], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[5]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements[5]), elements[5], GL_STATIC_DRAW);
        #pragma endregion

        #pragma region ORGANIZANDO ATTRIB POINTERS
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(3*sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(6*sizeof(float)));

        //Instancing
        glBindBuffer(GL_ARRAY_BUFFER, VBI);

        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3); 
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4); 
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5); 
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6); 
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        #pragma endregion

        #pragma region ATIVANDO ATTRIB POINTERSS
        glEnableVertexAttribArray(0); 
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        #pragma endregion
        
        glBindVertexArray(VAO);

    }

    static void SetVbiData(const vector<mat4>& model)
    {
        glDeleteBuffers(1, &VBI);
        glGenBuffers(1, &VBI);

        glBindBuffer(GL_ARRAY_BUFFER, VBI);
        glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(mat4), model.data(), GL_STATIC_DRAW);
    }

    static void Draw(int amount, BlockType type)
    {
        if(lastBlockType != type)
        {
            Shader::singleton->setInt("texture1", TextureID(type));
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBI);
        
        for(int face = 0; face < 6; face++)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[face]);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, amount);
        }

        lastBlockType = type;
    }

    static void Delete()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(6, EBOs);
    }

};

Vertex CubeMesh::vertex[24] = {
    //Position                    //Colors                  //Texture 

    //Top
    {vec3(-0.5f,  0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc2)}, //0 botton left
    {vec3( 0.5f,  0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc2)}, //1 botton right
    {vec3( 0.5f,  0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc1)}, //2 top right
    {vec3(-0.5f,  0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc1)}, //3 top left

    //Front
    {vec3(-0.5f, -0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc3)}, //0 botton left
    {vec3( 0.5f, -0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc3)}, //1 botton right
    {vec3( 0.5f,  0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc2)}, //2 top right
    {vec3(-0.5f,  0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc2)}, //3 top left

    //Down
    {vec3(-0.5f, -0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc0, tc3)}, //0 botton left
    {vec3( 0.5f, -0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc3)}, //1 botton right
    {vec3( 0.5f, -0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc2)}, //2 top right
    {vec3(-0.5f, -0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc0, tc2)}, //3 top left

    //Back
    {vec3( 0.5f, -0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc1)}, //0 botton left
    {vec3(-0.5f, -0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc1)}, //1 botton right
    {vec3(-0.5f,  0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc0)}, //2 top right
    {vec3( 0.5f,  0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc0)}, //3 top left

    //Right
    {vec3( 0.5f, -0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc2)}, //0 botton left
    {vec3( 0.5f, -0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc3, tc2)}, //1 botton right
    {vec3( 0.5f,  0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc3, tc1)}, //2 top right
    {vec3( 0.5f,  0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc2, tc1)}, //3 top left

    //Left
    {vec3(-0.5f, -0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc0, tc2)}, //0 botton left
    {vec3(-0.5f, -0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc2)}, //1 botton right
    {vec3(-0.5f,  0.5f,  0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc1, tc1)}, //2 top right
    {vec3(-0.5f,  0.5f, -0.5f),   vec3(1.0f, 1.0f, 1.0f),   vec2(tc0, tc1)}  //3 top left


};

unsigned int CubeMesh::elements[6][6] = {
    //Topo 0
    {
        0, 1, 2,
        2, 3, 0
    },

    //Frente 1
    {
        4, 5, 6,
        6, 7, 4
    },

    //Embaixo 2
    {
        8, 9, 10,
        10, 11, 8
    },
    
    //Atras 3
    {
        12, 13, 14,
        14, 15, 12
    },

    //Direita 4
    {
        16, 17, 18,
        18, 19, 16
    },

    //Esquerda 5
    {
        20, 21, 22,
        22, 23, 20
    },

};

unsigned int CubeMesh::VAO;
unsigned int CubeMesh::VBO;
unsigned int CubeMesh::VBI;
unsigned int CubeMesh::EBOs[6];
bool CubeMesh::criado = false;
BlockType CubeMesh::lastBlockType = -1;

#endif 