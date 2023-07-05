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
            float noiseTestTOP = noise.GetNoise(x*10, (y+1)*10, z*10); //0  Topo
            float noiseTestFOR = noise.GetNoise(x*10, (y+1)*10, z*10); //1  Frente
            float noiseTestDOW = noise.GetNoise(x*10, (y+1)*10, z*10); //2  Embaixo
            float noiseTestBAC = noise.GetNoise(x*10, (y+1)*10, z*10); //3  Atras
            float noiseTestRIG = noise.GetNoise(x*10, (y+1)*10, z*10); //4  Direita
            float noiseTestLEF = noise.GetNoise(x*10, (y+1)*10, z*10); //5  Esquerda

            if(noiseTestTOP < minRender)
            {
                resp.push_back(0);
                // cout<< "executo\n";
            }

            if(noiseTestFOR < minRender)
            {
                resp.push_back(1);
                // cout<< "executo\n";
            } 

            if(noiseTestDOW < minRender)
            {
                resp.push_back(2);
                // cout<< "executo\n";
            }

            if(noiseTestBAC < minRender)
            {
                resp.push_back(3);
                // cout<< "executo\n";
            } 

            if(noiseTestRIG < minRender)
            {
                resp.push_back(4);
                // cout<< "executo\n";
            }

            if(noiseTest < minRender)
            {
                resp.push_back(5);
                // cout<< "executo\n";
            } 

            // resp.push_back(1);
            // resp.push_back(2);
            // resp.push_back(3);
            // resp.push_back(4);
            // resp.push_back(5);

            return resp;
        }


        for(int i = 0; i < 6; i++)
        {
            // cout<< "I: " << i << endl;
            // cout<< "x: " << x << " y: " << y << " z: " << z << endl;
            // cout<< "xOff: " << xOff + sides[i].x << " yOff: " << yOff + sides[i].y << " zOff: " << zOff + sides[i].z << endl;
            // cout<< "noise: " << noise.GetNoise((xOff + sides[i].x), (yOff + sides[i].y), (zOff + sides[i].z)) << endl;
            // cout<< "MinRender: " << minRender << endl;
            
            // cout<< endl;

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