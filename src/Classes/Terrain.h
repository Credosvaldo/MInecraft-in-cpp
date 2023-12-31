#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>

#include "Cube.h"
#include "FastNoiseLite.h"

using namespace std;

    
/*
    0  Topo
    1  Frente
    2  Embaixo
    3  Atras
    4  Direita
    5  Esquerda
*/


class Terrain : public GameObject
{
private:
    FastNoiseLite noise;
    float minRender = -1.5f;

    const int maxY = 4;
    const int maxX = 40;
    const int maxZ = 40;

    bool antigo = true;
    bool teste = true;

    int amount[6];


    FastNoiseLite::NoiseType noiseTypes[6] = {
        FastNoiseLite::NoiseType_Cellular,//0
        FastNoiseLite::NoiseType_OpenSimplex2,//1
        FastNoiseLite::NoiseType_OpenSimplex2S,//2
        FastNoiseLite::NoiseType_Perlin,//3
        FastNoiseLite::NoiseType_Value,//4
        FastNoiseLite::NoiseType_ValueCubic//5
    };

    bool ThresABlock(float x, float y, float z)
    {
        
        float xOff = x*10;
        float yOff = y*10;
        float zOff = z*10;

        float noiseValue = noise.GetNoise(xOff, yOff, zOff);

        if(noiseValue >= minRender)
        {
            return true;
        }

        return false;

    }

    vector<int> ShouldRender(float x, float y, float z)
    {
        vector<int> resp;
        vec3 sides[] = {
            vec3( 0,  10,  0), //0  Topo
            vec3( 0,  0,  10), //1  Frente
            vec3( 0, -10,  0), //2  Embaixo
            vec3( 0,  0, -10), //3  Atras
            vec3( 10,  0,  0), //4  Direita
            vec3(-10,  0,  0)  //5  Esquerda
        };

        float xOff = x*10;
        float yOff = y*10;
        float zOff = z*10;

        float noiseValue = noise.GetNoise(xOff, yOff, zOff);

        if(noiseValue < minRender)
            return resp;

        if(teste)
        {
            //if(!ThresABlock(x,y+1,z)) //Topo
            resp.push_back(0);
            
            if(!ThresABlock(x,y,z+1))  //Frente
            resp.push_back(1);

            if(!ThresABlock(x,y-1,z))  //Embaixo
            resp.push_back(2);

            //if(!ThresABlock(x,y,z-1)) //Atras
            resp.push_back(3);

            //if(!ThresABlock(x+1,y,z)) //Direita
            resp.push_back(4);

            //if(!ThresABlock(x-1,y,z)) //Esquerda
            resp.push_back(5);

            return resp;
        }


        for(int i = 0; i < 6; i++)
        {

            bool limite = y==maxY-1 && i==0 || y==0 && i==2;
            if(noise.GetNoise((xOff + sides[i].x), (yOff + sides[i].y), (zOff + sides[i].z)) < minRender)
            {
                resp.push_back(i);
            }
        }

        return resp;
    }

    void DesenharTerreno()
    {
        #pragma region Novo
        for(const auto& cube : cubes)
        {
            delete cube;
        }

        cubes.clear();

        noise.SetNoiseType(noiseTypes[indexNoiseType]);
        vector<mat4> model[6];

        for(int i = -maxX; i < maxX; i++)
        {
            for(int j = -maxY; j < maxY; j++)
            {
                for(int k = -maxZ; k < maxZ; k++)
                {
                    vector<int> resp = ShouldRender(i, j, k);

                    if(!resp.empty())
                    {
                        cubes.push_back(new Cube(vec3(i, j, k), BLOCK_GRASS));
                        mat4 aux = mat4(1.0f);
                        aux = translate(aux, cubes.back()->transform->position);

                        for(const auto r : resp)
                        {
                            model[r].push_back(aux);
                        }

                    }

                }
                
            }

        }

        for(int i = 0; i < 6; i++)
        {
            amount[i] = model[i].size();
        }

        CubeMesh::SetVbiData(model);

        #pragma endregion
    }

    ~Terrain()
    {
        for(const auto& cb : cubes)
        {
            delete cb;
        }

        cubes.clear();
    }



public:
    static Terrain* currentTerrain;
    vector<Cube*> cubes;
    int indexNoiseType = 0;

    void Start()
    {
        currentTerrain = this;
        DesenharTerreno();
    }

    void Update()
    {
        if(Input::GetKeyDown(ALLEGRO_KEY_RIGHT))
        {
            indexNoiseType++;
            if(indexNoiseType == 6)
                indexNoiseType = 0;
            DesenharTerreno();
            
        }

        if(Input::GetKeyDown(ALLEGRO_KEY_LEFT))
        {
            indexNoiseType--;
            if(indexNoiseType == -1)
                indexNoiseType = 5;
            DesenharTerreno();
            
        }

    }

    void Draw()
    {
        CubeMesh::Draw(amount, BLOCK_GRASS);
    }


};

Terrain* Terrain::currentTerrain;

#endif