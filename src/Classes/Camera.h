#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"

int D_WIDTH = 800;
int D_HEIGHT = 600;

enum{
    ALLEGRO_MOUSE_BUTTON_LEFT = 1,
    ALLEGRO_MOUSE_BUTTON_SCROLL = 2,
    ALLEGRO_MOUSE_BUTTON_RIGHT = 3 
};

class Camera:public GameObject
{
private:
    float camYaw;
    float camPitch;
    float sensitivity;
    float speed;
    bool lock;
    
    void CameraRotatin()
    {
        vec2 delta = Input::GetMouseDeltaPosition();
        camYaw += delta.x * sensitivity;
        camPitch -= delta.y * sensitivity;
        
        camYaw = fmod(camYaw, 360.0f);
        camPitch = fmod(camPitch, 360.0f);

        if(camPitch > 80.0f)
        {
            camPitch = 80.0f;
        }
        else if(camPitch < -80.0f)
        {
            camPitch = -80.0f;
        }
        if(lock) 
        {
            al_set_mouse_xy(al_get_current_display(), D_WIDTH/2, D_HEIGHT/2);
        }
    }

    void SetCameraDirection()//to do -> Adicionar um Lerp
    {
        vec3 direction;
        direction.x = cos(radians(camYaw)) * cos(radians(camPitch));
        direction.y = sin(radians(camPitch));
        direction.z = sin(radians(camYaw)) * cos(radians(camPitch));

        transform->forward = direction;
        
    }

    void LookCamera()
    {
        if(Input::GetMouseButtonDown(ALLEGRO_MOUSE_BUTTON_LEFT))
        {
            lock = true;
            al_grab_mouse(al_get_current_display());
            al_hide_mouse_cursor(al_get_current_display());
        }

        if(Input::GetKeyDown(ALLEGRO_KEY_ESCAPE))
        {
            lock = false;
            al_ungrab_mouse();
            al_show_mouse_cursor(al_get_current_display());
        }
    }

    void Move()
    {
        vec3 move = vec3(0.0f,0.0f,0.0f);

        if(Input::GetKey(ALLEGRO_KEY_W))//Frente
        {
            move += transform->forward;
        }

        if(Input::GetKey(ALLEGRO_KEY_S))//Tras
        {
            move -= transform->forward;
        }

        if(Input::GetKey(ALLEGRO_KEY_D))//Direita
        {
            move += normalize(cross(transform->forward, transform->up));
        }

        if(Input::GetKey(ALLEGRO_KEY_A))//Esquerda
        {
            move -= normalize(cross(transform->forward, transform->up));
        }


        if(Input::GetKey(ALLEGRO_KEY_E))//Cima
        {
            move += transform->up;
        }

        if(Input::GetKey(ALLEGRO_KEY_Q))//Baixo
        {
            move -= transform->up;
        }


        if(Input::GetKey(ALLEGRO_KEY_LSHIFT))// + Speed 
        {
            speed = 0.1f;
        }
        else// - Speed 
        {
            speed = 0.05f;
        }



        if(move == vec3(0.0f))//Seguranca 
        {
            return;
        }

        transform->position += (normalize(move) * speed);
    }


public:
    static Camera *main;

    mat4 projection;
    mat4 view;

    void CameraLookAt()
    {
        view = lookAt(transform->position, transform->position + transform->forward, transform->up);
    }



    void Start()
    {
        Shader::singleton->setMatrix("projection", Camera::main->projection);
    }

    void Update()
    {
        CameraRotatin();
        SetCameraDirection();
        CameraLookAt();
        LookCamera();
        Move();

        Shader::singleton->setMatrix("view", Camera::main->view);

    }

    Camera(vec3 position)
    {
        transform->position = position;
        //Singleton
        if(main == nullptr)
        {
            main = this;
        }

        //Privadas
        camYaw = -90;
        camPitch = 0;
        sensitivity = 0.2f;
        speed = 1.0f;
        lock = true;

        //Publicas
        projection = perspective(radians(45.0f), (float)D_WIDTH/D_HEIGHT, 0.01f, 1000.0f);
    }


};

Camera* Camera::main = nullptr;

#endif