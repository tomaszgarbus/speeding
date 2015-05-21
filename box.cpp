#include "box.h"

Box::Box()
{

}

QColor randomColor()
{
    QColor ret;
    int c = rand()%7;
    if (c == 0) ret = Qt::red;
    if (c == 1) ret = Qt::yellow;
    if (c == 2) ret = Qt::blue;
    if (c == 3) ret = QColor(141, 56, 201);
    if (c == 4) ret = QColor(255, 165, 0);
    if (c == 5) ret = Qt::green;
    if (c == 6) ret = QColor(75, 0, 130);
    return ret;
}

Box::Box(Point r, int w, int h, ColorScheme *scheme, action a) : root(r), W(w), H(h), A(a)
{
    //color = QColor(0, 0, 100+(W)%95);

    if (A != crash) color = scheme->menuBox;
    else if ((W+H)%2 == 0) color = scheme->crashBox1;
    else color = scheme->crashBox2;

    if (scheme->isRainbow && a != crash)
    {
        if (a == done) color = Qt::red;
        if (a == colorscheme) color = Qt::yellow;
        if (a == resume) color = Qt::blue;
        if (a == restart) color = QColor(141, 56, 201);
        if (a == snow) color = QColor(255, 165, 0);
        if (a == steering) color = Qt::green;
        color.setAlpha(170);
    }

    if (scheme->isRainbow && a == crash)
    {
        color = randomColor();
        color.setAlpha(170);
    }

    angle = fallSpeed = 0;

    isCore = falling = nearMissed = false;
}

bool Box::operator <(Box B) const
{
    if (root.Z == B.root.Z) return W > B.W;
    return root.Z > B.root.Z;
}

#define PREPP(X) fixPoint(size, transformPoint(pov, (X)))

Point Box::ANG(Point A)
{
    return Point(
                A.X - (angle!= 0 ? abs(A.Y)/angle : 0),
                A.Y + 5 * (angle != 0 ? A.Y/abs(angle) : 0) * (angle < 0 ? A.X < root.X : A.X > root.X),
                A.Z
                );
}

QPolygon Box::rearWall(QSize size, Camera *pov)
{

    vector<Point> t;
    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X-W, root.Y+H, max(pov->Z, root.Z+W))));

    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z+W))));

    QPolygon ret;

    for (int i = 0; i < 5; i++)
    {
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::frontWall(QSize size, Camera *pov)
{
    vector<Point> t;
    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y+H, max(pov->Z, root.Z-W))));

    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < 5; i++)
    {
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::leftWall(QSize size, Camera *pov)
{
    vector<Point> t;
    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y+H, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y+H, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z+W))));

    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < 5; i++)
    {
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::rightWall(QSize size, Camera *pov)
{
    vector<Point> t;
    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z+W))));

    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < 5; i++)
    {
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::upperWall(QSize size, Camera *pov)
{
    vector<Point> t;
    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y+H, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y+H, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z+W))));

    t.push_back((Point(root.X+W, root.Y+H, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < 5; i++)
    {
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::lowerWall(QSize size, Camera *pov)
{
    vector<Point> t;
    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-W, root.Y, max(pov->Z, root.Z+W))));
    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z+W))));

    t.push_back((Point(root.X+W, root.Y, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < 5; i++)
    {
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::jumpSymbol(QSize size, Camera *pov, int _h, bool reflection)
{
    vector<Point> t;
    t.push_back((Point(root.X-40, root.Y+100, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-40, root.Y+200, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X, root.Y+400, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+40, root.Y+200, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+40, root.Y+100, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X, root.Y+300, max(pov->Z, root.Z-W))));

    t.push_back((Point(root.X-40, root.Y+100, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < 7; i++)
    {
        t[i].Y += _h;
        if (reflection) t[i].Y *= -1;
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

QPolygon Box::healSymbol(QSize size, Camera *pov, int _h, bool reflection)
{
    vector<Point> t;

    t.push_back((Point(root.X-40, root.Y+140, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-40, root.Y+380, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-15, root.Y+380, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-15, root.Y+560, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+15, root.Y+560, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+15, root.Y+380, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+40, root.Y+380, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+40, root.Y+140, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+15, root.Y+140, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X+15, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-15, root.Y, max(pov->Z, root.Z-W))));
    t.push_back((Point(root.X-15, root.Y+140, max(pov->Z, root.Z-W))));

    t.push_back((Point(root.X-40, root.Y+140, max(pov->Z, root.Z-W))));

    QPolygon ret;

    for (int i = 0; i < int(t.size()); i++)
    {
        t[i].Y += _h;
        if (reflection) t[i].Y *= -1;
        ret.push_back(PREPP(ANG(t[i])));
    }

    return ret;
}

bool intersect(Box A, Point B)
{
    if (B.Z > A.root.Z + A.W || B.Z < A.root.Z - A.W) return false;
    if (B.Y > A.root.Y + A.H || B.Y < A.root.Y) return false;

    // A.X - (angle!= 0 ? abs(A.Y)/angle : 0),

    int x1 = A.root.X - A.W - (A.angle != 0 ? abs(B.Y)/A.angle : 0);
    int x2 = A.root.X + A.W - (A.angle != 0 ? abs(B.Y)/A.angle : 0);
    return B.X >= x1 && B.X <= x2;
}

bool intersect(Box A, Box B)
{
    int minx = max(A.root.X - A.W, B.root.X - B.W);
    int maxx = min(A.root.X + A.W, B.root.X + B.W);

    int miny = max(A.root.Y - A.H, B.root.Y - B.H);
    int maxy = min(A.root.Y + A.H, B.root.Y + B.H);

    int minz = max(A.root.Z - A.W, B.root.Z - B.W);
    int maxz = min(A.root.Z + A.W, B.root.Z + B.W);

    return maxy >= miny && maxx >= minx && maxz >= minz;
}

bool Box::visible(QSize size, Camera *pov)
{
    QPolygon A = this->rearWall(size, pov);
    for (int i = 0; i < 4; i++)
        if (A[i].x() >= 0 && A[i].x() <= size.width() && A[i].y() >= 0 && A[i].y() <= size.height())
            return true;

    return false;
}

bool Box::clicked(QSize size, Camera *pov, QPoint P)
{
    QPoint A = PREPP(Point(root.X-W, root.Y+H, max(pov->Z, root.Z-W)));
    QPoint Z = PREPP(Point(root.X+W, root.Y, max(pov->Z, root.Z-W)));

    //qDebug() << A.x() << " " << A.y() << " " << P.x() << " " << P.y() << " " << Z.x() << " " << Z.y();

    return P.x() >= A.x() && P.x() <= Z.x() && P.y() >= A.y() && P.y() <= Z.y();
}

bool Box::frontWallVisible(Camera *pov)
{
    return pov->Z <= root.Z-W;
}

bool Box::leftWallVisible(Camera *pov)
{
    return pov->X <= root.X-W;
}

bool Box::rightWallVisible(Camera *pov)
{
    return pov->X >= root.X-W;
}

bool Box::upperWallVisible(Camera *pov)
{
    return pov->Y >= root.Y+H;
}

bool Box::lowerWallVisible(Camera *pov)
{
    return pov->Y < root.Y;
}

Box Box::core()
{
    Box ret = *this;
    ret.W -= 15;
    if (ret.H < 0) ret.H = min(ret.H+15, 0);
    else ret.H = max(ret.H-15, 0);
    ret.isCore = true;
    ret.color.setAlpha(min(255, color.alpha()*180/130));
    return ret;
}

Box Box::reflection()
{
    Box ret = *this;
    ret.root.Y = -ret.root.Y - ret.H;
    //ret.H *= -1;
    ret.color.setAlpha(20);
    return ret;
}

QRect Box::frontRect(QSize size, Camera *pov, int margin)
{
    QRect ret =
        QRect(PREPP(Point(root.X-W, root.Y+H, max(pov->Z, root.Z-W))), PREPP(Point(root.X+W, root.Y, max(pov->Z, root.Z-W))));
    ret.setX(ret.x() + margin); ret.setY(ret.y() + margin);
    ret.setWidth(ret.width() - 2 * margin); ret.setHeight(ret.height() - 2 * margin);
    return ret;
}
