#ifndef POINT_H
#define POINT_H

class Point
{
public:
    int X, Y, Z;

    Point();
    Point(int, int, int);

    Point reflection();

};

bool collinear(Point *, Point *);

#endif // POINT_H
