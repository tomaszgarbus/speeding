#include "maths.h"

Point transformPoint(Camera *pov, Point pt)
{
    const int near = 80;
    int X = pt.X - pov->X;
    int Y = pt.Y - pov->Y;
    int Z = pt.Z - pov->Z - near;

    if (10 * Z <= -CUBE_SIZE)
    {
        //qDebug() << X << " " << Y << " " << Z;
        return Point(0, 0, 0);
    }

    int nX = CUBE_SIZE + 8 * X * (CUBE_SIZE) / (CUBE_SIZE + 10 * Z);
    int nY = CUBE_SIZE + Y * (CUBE_SIZE) / (CUBE_SIZE + 10 * Z);

    return Point(nX, nY, pt.Z);
}

QPoint fixPoint(QSize size, Point pt)
{
    int a = min(size.width(), size.height());

    return QPoint(

                size.width()/2 + (pt.X - CUBE_SIZE) * (a) / (CUBE_SIZE),
                size.height()/2 - (pt.Y - CUBE_SIZE) * (a) / (CUBE_SIZE)
                );
}

#define PREPP(X) fixPoint(size, transformPoint(pov, (X)))

QPolygon lyingParticle(QSize size, Camera *pov, Point O)
{
    QPolygon ret;
    ret.push_back(PREPP(Point(O.X, O.Y, O.Z)));
    ret.push_back(PREPP(Point(O.X, O.Y, O.Z+10)));
    ret.push_back(PREPP(Point(O.X+10, O.Y, O.Z+10)));
    ret.push_back(PREPP(Point(O.X+10, O.Y, O.Z)));
    return ret;
}
