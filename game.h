#ifndef GAME_H
#define GAME_H

#include <QString>
#include <QThread>
#include <QTimer>
#include <QPushButton>
#include <QSettings>
#include <QApplication>
#include <QAccelerometer>

#include <vector>
#include <set>

#include "box.h"
#include "random.h"
#include "colorscheme.h"
#include "utility.h"

using namespace std;

const int currentVersion = 13;

class Game : public QThread
{
    Q_OBJECT

    QTimer ticktock;
    QTimer obstacleTimer, particleTimer;

    int lastMaxSpeed;
    int maxSpeed();
    void checkTimers();
    void autoSteer();
    void fixObstacles();

private slots:

    void addObstacle();
    void addParticle();

public slots:
    void update(int);

public:
    int xShift, yShift;
    vector<int> xsh, ysh;

    void saveSettings();
    void loadSettings();

    bool reflectionsOn, coresOn, comicOn, autoSteering, snowOn, tiltOn, firstLaunch, benchmarkOn, particlesOn;

    int quakeIntensity;
    bool shake;
    vector<double> tilts;
    void startShake();

    Game();

    void start();

    int playerID, scoreToSubmit;
    string playerName;

    Camera player;
    Box resumeBox, restartBox, schemeBox, snowBox, closeBox, steeringBox;
    vector<Box> obstacles;
    int direction, speed, lives, resurrectionTarget, tilt, lastVersion, myPhoneSux;
    qreal qdirection;
    bool paused, resurrecting;

    bool vibe;

    void updateMenuBoxes();

    vector<Point> particles;

    pair<bool, bool> dead();

    ColorScheme scheme;
    int schemeNo;

    int highScore;
};

#endif // GAME_H
