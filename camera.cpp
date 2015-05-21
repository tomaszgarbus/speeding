#include "camera.h"

Camera::Camera()
{
    X = Y = Z = 0;
}

Camera::Camera(int x, int y, int z)
{
    X = x;
    Y = y;
    Z = z;
    playerFall = 0;
}
