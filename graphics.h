#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QPainter>
#include <QGraphicsScene>

#include "mainwindow.h"
#include "camera.h"
#include "maths.h"

void drawGround(MainWindow *, Camera *);

void drawFrame(MainWindow *, Camera *);

#endif // GRAPHICS_H
