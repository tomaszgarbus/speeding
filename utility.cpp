#include "utility.h"

string toString(int n)
{
    if (n == 0) return "0";

    bool neg = false;

    if (n < 0) n = -n, neg = true;

    string r = "";
    while (n)
    {
        r = char(n%10+'0') + r;
        n /= 10;
    }

    if (neg) r = "-" + r;

    return r;
}

string iHeartString(int lives)
{
    string ret;
    while (lives--) ret = ret + "<3 ";
    return ret;
}

void tiltWall(QPolygon *wall)
{
    qDebug() << wall->size();
    for (int i = 0; i < int(wall->size()); i++)
    {
        qDebug() << i;
        qDebug() << wall->point(i).x() << " " << wall->point(i).y();
        wall->setPoint(i, wall->point(i).x()-1+rand()%3, wall->point(i).y()-1+rand()%3);
    }
    wall->setPoint(int(wall->size())-1, wall->point(0));
}

qreal ariMean(vector<qreal> *V)
{
    qreal ret = 0.f;
    for (int i = 0; i < int(V->size()); i++) ret = ret + V->at(i);
    return ret / qreal(V->size());
}

int ariMean(vector<int> *V)
{
    int ret = 0;
    for (int i = 0; i < int(V->size()); i++) ret = ret + V->at(i);
    return ret / int(V->size());
}
