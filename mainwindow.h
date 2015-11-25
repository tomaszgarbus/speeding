#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWindow>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QWidget>
#include <QPushButton>
#include <QPalette>
#include <QPainterPath>
#include <QFontDatabase>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSound>
#include <QFileInfo>
#include <QtTest/QTest>
#include <QOpenGLShader>
#include <QApplication>
#include <QLineEdit>
#include <QQuickView>

#include <QtAndroidExtras/QAndroidJniObject>

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

#include <vector>
#include <list>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

#include "game.h"
#include "utility.h"

class MainWindow : public QWindow, public QOpenGLFunctions
{
    Q_OBJECT

    QAccelerometer* acc;

    QPainter painter;

    QMediaPlaylist* bgPlaylist;
    QMediaPlayer* bgSound;

    QOpenGLContext context;
    QOpenGLPaintDevice *device;

    QTimer timer, fps_timer;

    Game *game;

    int BENCHMARK_FRAMES; //imagine its a const

    int vol();

    int fps_count, last_fps, benchmarkFPSCount, framesElapsed;
    vector<qreal> benchmarkVector;

    bool initialized, showFPS;

    bool blurLines, gradientOn;

    int myPhoneSux;

    list<qreal> tiltHistory, touchHistory;
    qreal tiltSum, touchSum;

    void exposeEvent(QExposeEvent *);
    void resizeEvent(QResizeEvent *);

    QLinearGradient gradientDown, gradientMid1, gradientMid2;
    QRadialGradient gradientUp;

    void initializeGradients();

    QFont font; QPen cnt;

    void drawParticle(Point);

    void drawGreeting();

    void submitScore();
    void submitName();

    int shadow;

    bool unveil, unveilEd;

    int greetingClicks;

    int newsFall(int);

    int appStates[2];

    bool keyLeftDown, keyRightDown;

private slots:
    void sec();
    void paint();

protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);

public:
    QApplication* app;

    MainWindow(Game *, QWindow* = nullptr);
    ~MainWindow();

    void drawWall(QPolygon, QColor, bool = false);
    void drawLine(Point, Point, QPen, qreal = 1);
    void drawGround();
    void drawEvent(Box *, bool = false);
    void drawObstacle(Box *);
    void drawPauseButton();
};

#endif // MAINWINDOW_H
