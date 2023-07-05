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
    float minRender = -0.5f;

    const int maxY = 80;
    const int maxX = 64;
    const int maxZ = 64;

    bool antigo = true;


    FastNoiseLite::NoiseType noiseTypes[6] = {
        FastNoiseLite::NoiseType_Cellular,//0
        FastNoiseLite::NoiseType_OpenSimplex2,//1
        FastNoiseLite::NoiseType_OpenSimplex2S,//2
        FastNoiseLite::NoiseType_Perlin,//3
        FastNoiseLite::NoiseType_Value,//4
        FastNoiseLite::NoiseType_ValueCubic//5
    };


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
        CubeMesh::Draw(cubes.size(), BLOCK_GRASS);
    }

    bool ShouldRender(int x, int y, int z)
    {
        vector<int> resp;
        vec3 sides[] = {
            vec3( 0,  10,  0),
            vec3( 0,  0,  10),
            vec3( 0, -10,  0),
            vec3( 0,  0, -10),
            vec3( 10,  0,  0),
            vec3(-10,  0,  0)
        };

        float xOff = x*10;
        float yOff = y*10;
        float zOff = z*10;

        float noiseValue = noise.GetNoise(xOff, yOff, zOff);

        if(noiseValue < minRender)
            return false;


        for(int i = 0; i < 6; i++)
        {
            bool limite = y==maxY-1 && i==0 || y==0 && i==2;
            if(noise.GetNoise((xOff + sides[i].x), (yOff + sides[i].y), (zOff + sides[i].z)) < minRender || limite)
            {
                resp.push_back(i);
            }
        }

        return true;
    }

    void DesenharTerreno()
    {
        for(const auto& cube : cubes)
        {
            delete cube;
        }

        cubes.clear();

        noise.SetNoiseType(noiseTypes[indexNoiseType]);
        vector<mat4> model;

        float noiseValue; 

        for(int i = -maxX; i < maxX; i++)
        {
            for(int j = -maxY; j < maxY; j++)
            {
                noiseValue = 2* noise.GetNoise((float)i*10, (float)j*10);

                cubes.push_back(new Cube(vec3(i, (int)noiseValue, j), BLOCK_GRASS));
                
                mat4 aux = mat4(1.0f);
                aux = translate(aux, cubes.back()->transform->position);
                model.push_back(aux);
                
                
            }

        }

        CubeMesh::SetVbiData(model);

        if(antigo)
            return;

        #pragma region Novo
        for(const auto& cube : cubes)
        {
            delete cube;
        }

        cubes.clear();

        noise.SetNoiseType(noiseTypes[indexNoiseType]);
        //vector<mat4> model;

        for(int i = 0; i < maxX; i++)
        {
            for(int j = 0; j < maxY; j++)
            {
                for(int k = 0; k < maxZ; k++)
                {
                    if(ShouldRender(i, j, k))
                    {
                        cubes.push_back(new Cube(vec3(i, j, k), BLOCK_GRASS));
                        mat4 aux = mat4(1.0f);
                        aux = translate(aux, cubes.back()->transform->position);
                        model.push_back(aux);
                    }

                }
                
            }

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

};

Terrain* Terrain::currentTerrain;

#endif