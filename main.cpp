#pragma region Bibliotecas e Afins
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>

#include <GL/glew.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/threads.h>

#include <GL/gl.h>


#include "Classes/shader.h"
#include "Classes/Inputs.h"
#include "Classes/FastNoiseLite.h"
#include "Classes/CubeMesh.h"
#include "Classes/Camera.h"
#include "Classes/Cube.h"
#include "Classes/Time.h"

#include "Enums/BlockType.h"

#include "Struct/Vertex.h"

#define STB_IMAGE_IMPLEMENTATION
#include "OthersHeaders/stb_image.h"

#include "OthersHeaders/base.h"



#pragma endregion


FastNoiseLite::NoiseType noiseTypes[] = {
    FastNoiseLite::NoiseType_Cellular,//0
    FastNoiseLite::NoiseType_OpenSimplex2,//1
    FastNoiseLite::NoiseType_OpenSimplex2S,//2
    FastNoiseLite::NoiseType_Perlin,//3
    FastNoiseLite::NoiseType_Value,//4
    FastNoiseLite::NoiseType_ValueCubic//5
};


#pragma region Variaveis Globais
ALLEGRO_EVENT event;
bool done = false;

Shader *shader;

vector<GameObject*> gameObjects;
vector<Cube*> cubes;

//Main
ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_TIMER *timer;

//Threads
thread* threadUpdateKey;
thread *threadEvents;

#pragma endregion

void must_init(bool test, string msg)
{
    if(test)
        return;

    cout<< "Nao foi possivel inicializar " << msg << endl;
    exit(1);

}

int indexNoiseType = 0;

void InitGlew()
{
    if(glewInit() != GLEW_OK)
    {
        must_init(false, "glew");
    }
}

void InitShaders()
{
    shader = new Shader("Shader/vertexShader.vert", "Shader/fragmentShader.frag");
    shader->use();
}

void InitTextures()
{
    #pragma region New Textures

    const int maxTextures = 1;
    const char* imagePath[maxTextures] = {
        "Images/GrassBlock.png"
    };

    unsigned int textures[maxTextures];
    int width, height, chanels;
    unsigned char *image;

    for(int i = 0; i < maxTextures; i++)//ATUALIZE OS VALORES INTERNOS PARA SE ADEQUAR AO FOR
    {
        glGenTextures(maxTextures, textures);

        image = stbi_load(imagePath[i], &width, &height, &chanels, 0);
        must_init(image, "texture load");

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]); //Binda a textura

        //Quando muito pequena ou grande
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //Registrando imagem
        glTexImage2D(GL_TEXTURE_2D, 0, chanels == 3 ? GL_RGB : GL_RGBA, width, height, 0, chanels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        stbi_image_free(image);

    }


    #pragma endregion

}

void DesenharTerreno()
{
    cubes.clear();
    FastNoiseLite noise;

    noise.SetNoiseType(noiseTypes[indexNoiseType]);

    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            for(int k = 0; k < 32; k++)
            {
                float noiseValue = noise.GetNoise( (float)i*10, (float)j*10, (float)k*10);

                if(noiseValue < -0.5f)
                    continue;

                cubes.push_back(new Cube(shader, vec3(i, j, k), BLOCK_GRASS));

            }
            
        }

    }



}

void InitCamera()
{
    gameObjects.push_back(new Camera(vec3(0,32,0)));
    shader->setMatrix("projection", Camera::main->projection);

}

void InitDepthTest()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void InitCulling()
{
    glEnable(GL_CULL_FACE);  // habilita o culling
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    for(const auto& cb : cubes)
    {
        cb->Culling(cubes);
    }

}

void initOpenGL()
{
    InitGlew();
    InitShaders();
    InitTextures();

    thread terrainThread(DesenharTerreno);

    InitCamera();
    InitCulling();
    InitDepthTest();

    terrainThread.join();

    InitCulling();

}



void updateKey()
{
    if(Input::GetKeyDown(ALLEGRO_KEY_RIGHT))
    {
        
        indexNoiseType++;
        if(indexNoiseType == 6)
            indexNoiseType = 0;
        DesenharTerreno();
        
        //cout<< "Index: " << indexNoiseType << "\n";
    }

    if(Input::GetKeyDown(ALLEGRO_KEY_LEFT))
    {
        
        indexNoiseType--;
        if(indexNoiseType == -1)
            indexNoiseType = 5;
        DesenharTerreno();
        
        //cout<< "Index: " << indexNoiseType << "\n";
    }


}



void draw()
{
    al_clear_to_color(al_map_rgb(135, 206, 235));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(const auto& cb : cubes)
    {
        cb->Draw();
    }

    al_flip_display();
}

void logic()
{
    for(const auto& go : gameObjects)
    {
        go->Update();
    }

    

}

void threadTest()
{

}

void RegisterEvents()
{
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
}

void InitAllegro()
{
    //INICIALIZANDO CONTEXTO
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "teclado");
    must_init(al_install_mouse(), "mouse");

    //SET DISPLAY FLAGS
    al_set_new_display_flags(ALLEGRO_OPENGL);
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 24, ALLEGRO_REQUIRE);

    //VARIAVEIS ALLEGRO
    display = al_create_display(D_WIDTH, D_HEIGHT);
    must_init(display, "display");
    queue = al_create_event_queue();
    must_init(queue, "event queue");
    timer = al_create_timer(1.0f/60.0f);
    must_init(timer, "timer");

    //SET MOUSE CURSOR CONFIGS
    al_grab_mouse(display);
    al_hide_mouse_cursor(display);

}

void DeleteAll()
{
    al_uninstall_keyboard();
    al_uninstall_mouse();

    al_destroy_display(display),
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);

    threadUpdateKey->join();
    threadEvents->join();

    delete threadUpdateKey;
    delete threadEvents;

    for(const auto& go : gameObjects)
    {
        delete go;
    }

    for(const auto& cb : cubes)
    {
        delete cb;
    }

    delete shader;

    CubeMesh::Delete();
}

int main()
{
    InitAllegro();
    initOpenGL();
    
    threadUpdateKey = new thread(updateKey);
    threadEvents = new thread(RegisterEvents);

    Input::Init();
    CubeMesh::Init();

    for(const auto& go : gameObjects)
    {
        go->Start();
    }

    al_start_timer(timer);
    while(!done)
    {
        al_wait_for_event(queue, &event);
        
        switch (event.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            
            case ALLEGRO_EVENT_TIMER:
                threadUpdateKey->join();
                delete threadUpdateKey;
                threadUpdateKey = new thread(updateKey);
                logic();
                draw();
                break;

        }

        Input::WaitForEvent(&event);
        Time::Loop();

    }

    DeleteAll();

    return 0;
}