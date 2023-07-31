#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Transform.h"

class GameObject
{
private:

public:
    Transform* transform;
    GameObject()
    {
        transform = new Transform();
    }

    virtual void Start() = 0;
    
    void GameObjectUpdate()
    {
        transform->TransformUpdate();

        Update();
    }

    virtual void Update() = 0;



};



#endif