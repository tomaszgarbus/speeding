#ifndef CAMERA_H
#define CAMERA_H

#include "point.h"

class Camera : public Point
{
public:
    float tilt;
    int playerFall;
    Camera();
    Camera(int, int, int);

};

#endif // CAMERA_H
