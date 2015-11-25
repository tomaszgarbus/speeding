#include "point.h"

Point::Point()
{
    X = Y = Z = 0;
}

Point::Point(int x, int y, int z) : X(x), Y(y), Z(z)
{

}

bool collinear(Point *P, Point *Q)
{
    return (P->X == Q->X) + (P->Y == Q->Y) + (P->Z == Q->Z) >= 2;
}

Point Point::reflection()
{
    Point ret = *this;
    ret.Y *= -1;
    return ret;
}
