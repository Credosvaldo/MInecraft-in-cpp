#ifndef TIME_H
#define TIME_H

#include <allegro5/allegro5.h>

class Time
{
public:
    static double time;
    static double deltaTime;

    static void Loop()
    {
        double previousTime = time;
        time = al_get_time();
        deltaTime = time - previousTime;
    }

};

double Time::time = 0;
double Time::deltaTime = 0;

#endif