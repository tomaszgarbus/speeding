#ifndef MATHS_H
#define MATHS_H

#include <QPoint>
#include <algorithm>
#include <QDebug>
#include <QSize>
#include <QPolygon>

#include <cstdlib>
#include "camera.h"
#include <cmath>

using namespace std;

const int CUBE_SIZE = 1000;

Point transformPoint(Camera *, Point);

QPoint fixPoint(QSize, Point);

QPolygon lyingParticle(QSize, Camera *, Point);

#endif // MATHS_H
