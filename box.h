#ifndef BOX_H
#define BOX_H

#include <QPolygon>
#include <vector>
#include <QColor>
#include <QTimer>

#include "maths.h"
#include "colorscheme.h"

using namespace std;

QColor randomColor();

enum action
{
    crash = 0,
    resume = 1,
    restart = 2,
    reflection = 3,
    core = 4,
    colorscheme = 5,
    comicstyle = 6,
    snow = 7,
    jump = 8,
    done = 9,
    steering = 10,
    heal = 11
};

class Box
{

    Point ANG(Point);

public:

    Point root;
    int W, H, angle, fallSpeed;

    bool isCore, falling, nearMissed;

    action A;

    QColor color;

    Box();
    Box(Point, int, int, ColorScheme *, action = crash);

    bool operator<(Box B) const;
    bool clicked(QSize, Camera *, QPoint);

    bool frontWallVisible(Camera *);
    bool leftWallVisible(Camera *);
    bool rightWallVisible(Camera *);
    bool upperWallVisible(Camera *);
    bool lowerWallVisible(Camera *);

    QPolygon rearWall(QSize, Camera *);
    QPolygon frontWall(QSize, Camera *);
    QPolygon leftWall(QSize, Camera *);
    QPolygon rightWall(QSize, Camera *);
    QPolygon upperWall(QSize, Camera *);
    QPolygon lowerWall(QSize, Camera *);
    QPolygon jumpSymbol(QSize, Camera *, int = 0, bool = false);
    QPolygon healSymbol(QSize, Camera *, int = 0, bool = false);

    Box core();

    Box reflection();

    QRect frontRect(QSize, Camera *, int = 0);

    bool visible(QSize, Camera *);
};

bool intersect(Box, Point);
bool intersect(Box, Box);

#endif // BOX_H
