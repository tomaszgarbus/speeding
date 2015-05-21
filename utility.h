#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <QBrush>
#include <QPen>
#include <QImage>
#include <algorithm>
#include <QPainterPath>
#include <QDebug>

using namespace std;

string toString(int);

string iHeartString(int);

void tiltWall(QPolygon*);

qreal ariMean(vector<qreal>*);

int ariMean(vector<int>*);

#endif // UTILITY_H
