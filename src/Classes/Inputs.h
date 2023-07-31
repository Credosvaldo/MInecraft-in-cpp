#ifndef INPUTS_H
#define INPUTS_H

#include <allegro5/allegro5.h>
#include "../Struct/Mouse.h"

using namespace glm;


#define SEEN 1
#define RELEASE 2

typedef int KeyCode;
typedef int MouseButton;


class Input
{
private:
    static unsigned char key[ALLEGRO_KEY_MAX];
    static Mouse mouse;

    #pragma region Set Values
    static void UpdateKey()
    {
        for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= SEEN;

        for(int i = 0; i < 3; i++)
            mouse.button[i] &= SEEN;

         mouse.deltaPosition = vec2(0.0f);
    }


    static void SetKeyDown(KeyCode keyCode)
    {
        key[keyCode] = SEEN | RELEASE;
    }

    static void SetKeyUp(KeyCode keyCode)
    {
        key[keyCode] &= RELEASE;
    }


    static void SetMouseClickDown(MouseButton button)
    {
        mouse.button[button] = SEEN | RELEASE;
    };

    static void SetMouseClickUp(MouseButton button)
    {
        mouse.button[button] &= RELEASE;
    };

    static void SetMousePosition(vec2 position, vec2 deltaPosition)
    {
        mouse.position = position;
        mouse.deltaPosition = deltaPosition;
    }
    #pragma endregion

public:
    static void Init()
    {
        memset(key, 0, sizeof(key));
    }

    #pragma region Get Values
    static bool GetKey(KeyCode keyCode)
    {
        return key[keyCode];
    }

    static bool GetKeyDown(KeyCode keyCode)
    {
        return key[keyCode] == 3;
    }

    static vec2 GetMousePosition()
    {
        return mouse.position;
    }

    static vec2 GetMouseDeltaPosition()
    {
        return mouse.deltaPosition;
    }

    static bool GetMouseButton(MouseButton button)
    {
        return mouse.button[button];
    }

    static bool GetMouseButtonDown(MouseButton button)
    {
        return mouse.button[button] == 3;
    }
    #pragma endregion

    static void WaitForEvent(const ALLEGRO_EVENT *event)
    {
        switch (event->type)
        {
            case ALLEGRO_EVENT_TIMER:
                UpdateKey();
                break;

            case ALLEGRO_EVENT_KEY_UP:
                SetKeyUp(event->keyboard.keycode);
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                SetKeyDown(event->keyboard.keycode);
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                SetMousePosition(vec2(event->mouse.x, event->mouse.y), vec2(event->mouse.dx, event->mouse.dy));
                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                SetMouseClickDown(event->mouse.button);

                break;

            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                SetMouseClickUp(event->mouse.button);
                break;
        }
    }

};





unsigned char Input::key[] = "";
Mouse Input::mouse;


#endif