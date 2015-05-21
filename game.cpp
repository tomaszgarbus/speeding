#include "game.h"

void Game::loadSettings()
{
    QString settingsDir = QApplication::applicationDirPath() + "\ssettings.ini";
    QSettings settings(settingsDir);

    highScore = settings.value("highScore").toInt();
    schemeNo = settings.value("scheme").toInt();
    lastVersion = settings.value("version").toInt();
    if (lastVersion != currentVersion) firstLaunch = true, highScore = 0;
    tiltOn = !settings.value("tilt").toBool();
    myPhoneSux = settings.value("phonesux").toInt();
    playerID = settings.value("id").toInt();

    QString tmpName = settings.value("name").toString();
    //playerName = settings.value("name").toString();
}

void Game::saveSettings()
{
    QString settingsDir = QApplication::applicationDirPath() + "\ssettings.ini";
    QSettings settings(settingsDir);

    settings.setValue("highScore", highScore);
    settings.setValue("scheme", schemeNo);
    settings.setValue("version", (!firstLaunch) * currentVersion);
    settings.setValue("tilt", !tiltOn);
    settings.setValue("phonesux", myPhoneSux);
    settings.setValue("id", playerID);
    settings.setValue("name", QString(playerName.c_str()));
}

void Game::updateMenuBoxes()
{
    if (firstLaunch) return;

    resumeBox = Box(
                Point(player.X-120, player.Y - 200, player.Z+300),
                50,
                400,
                &scheme,
                resume
                );

    restartBox = Box(
                Point(player.X+120, player.Y - 200, player.Z+300),
                50,
                400,
                &scheme,
                restart
                );

    snowBox = Box(
                Point(player.X+120, player.Y + 300, player.Z+300),
                50,
                400,
                &scheme,
                snow
                );

    schemeBox = Box(
                Point(player.X-120, player.Y + 300, player.Z+300),
                50,
                400,
                &scheme,
                colorscheme
                );

    closeBox = Box(
                Point(player.X-120, player.Y + 800, player.Z+300),
                50,
                400,
                &scheme,
                done
                );

    steeringBox = Box(
                Point(player.X+120, player.Y + 800, player.Z+300),
                50,
                400,
                &scheme,
                steering
                );


    for (int i = 0; i < int(obstacles.size()); i++) if (obstacles[i].A != crash && obstacles[i].A != jump && obstacles[i].A != heal)
    {
        swap(obstacles[i], obstacles.back());
        obstacles.pop_back();
        i--;
    }

    obstacles.push_back(steeringBox);
    obstacles.push_back(closeBox);
    obstacles.push_back(snowBox);
    obstacles.push_back(schemeBox);
    obstacles.push_back(resumeBox);
    obstacles.push_back(restartBox);
}

pair<bool, bool> Game::dead()
{
    for (int i = 0; i < int(obstacles.size()); i++)
        //if (obstacles[i].A == crash && intersect(Box(player, 1, 1, &scheme), obstacles[i]))
        if (obstacles[i].A == crash && intersect(obstacles[i], Point(player.X, player.Y, player.Z)))
        {
            player.Z = obstacles[i].root.Z - obstacles[i].W;
            return make_pair(true, player.X > obstacles[i].root.X);
        }

    return make_pair(false, false);
}


void Game::addParticle()
{
    if (!particlesOn) return;
    if (snowOn && rand()%3 != 2) return;
    if (paused || speed <= 0 || resurrecting) return;

    particles.push_back(Point(
                            nextRandom(player.X-CUBE_SIZE*2, player.X+CUBE_SIZE*2),
                            nextRandom(0, CUBE_SIZE*12),
                            nextRandom(player.Z+CUBE_SIZE*3, player.Z+CUBE_SIZE*5))
                        );

}

const int levelRange[] = { 0, 10000, 30000, 80000, 160000, 250000 };

int Game::maxSpeed()
{
    int ret = 0;
    for (int i = 1; i < 7; i++)
    {
        if (player.Z >= levelRange[i-1])
        {
            ret = (i+1) * 10 + 10 * (player.Z - levelRange[i-1]) / (levelRange[i] - levelRange[i-1]);
        }
    }
    ret = min(ret, 65);
    if (player.Z > 250000) ret = 65 + (5*(player.Z-250000))/150000;
    ret = min(ret, 70);
    return ret;

}

void Game::checkTimers()
{
    obstacleTimer.start(60 * 100 / maxSpeed());
    particleTimer.start(15 * 100 / maxSpeed());
}

void Game::autoSteer()
{
    direction = 0;
    for (int i = 0; i < int(obstacles.size()); i++)
    {
        if (obstacles[i].root.X-obstacles[i].W <= player.X+3 && obstacles[i].root.X+obstacles[i].W >= player.X-3)
        {
            direction = 9 * (1 - 2 * (i%2));
        }
    }

}

#define score player.Z

void Game::update(int lag)
{
    if (resurrecting && player.Z >= resurrectionTarget)
    {
        resurrecting = false;
    }
    if (!benchmarkOn)
    {
        pair<bool, bool> tmpDead = dead();
        if (tmpDead.first) {

            fixObstacles();

            lives = max(lives-1, 0);
            tilt += speed*6 -12*speed*tmpDead.second;
            speed = -speed;

            resurrecting = true;
            resurrectionTarget = player.Z;

            if (!paused) vibe = true;
        }
    }

    if (lives == 0)
    {
        if (highScore == 0 && score > 0)
        {
            if (playerID == 0) playerID = rand()%100000000;
        }
        if (score > highScore)
        {
            highScore = score;
            saveSettings();
        }
        if (!paused) scoreToSubmit = highScore/25;
        paused = true;
    }

    if (paused)
    {
        direction = 0;
        updateMenuBoxes();
        return;
    }

    if (maxSpeed() != lastMaxSpeed) checkTimers();


    if (tilt > 0) tilt--;
    if (tilt < 0) tilt++;
    if ((1 ^ benchmarkOn ^ firstLaunch) && speed < maxSpeed()) for (int i = 0; i < lag; i++) ++speed;

    if (shake)
    {
        if (int(tilts.size()) == 8) tilts.erase(tilts.begin());
        tilts.push_back(-2.f+float(rand()%2000)/500.f);
        player.tilt += ariMean(&tilts);
        if (int(xsh.size()) == 8) xsh.erase(xsh.begin());
        xsh.push_back(-quakeIntensity+rand()%(2*quakeIntensity));
        if (int(ysh.size()) == 8) ysh.erase(ysh.begin());
        ysh.push_back(-quakeIntensity+rand()%(2*quakeIntensity));
        xShift = ariMean(&xsh), yShift = ariMean(&ysh);
        quakeIntensity = 10 + ((score/10000)%9-3)*3;
    }
    else {
        xShift = yShift = 0;
    }

    if (autoSteering) autoSteer();

    player.Z+=speed*lag;
    player.X+=direction*lag-tilt/20;

    player.playerFall++;
    player.Y = max(CUBE_SIZE/5, player.Y - player.playerFall);

    for (int i = 0; i < int(obstacles.size()); i++)
    {
        if (obstacles[i].A == heal && intersect(Box(player, 1, 1, &scheme), obstacles[i]))
        {
            lives = min(lives+1, 3);
            swap(obstacles[i], obstacles.back());
            obstacles.pop_back();
            i--;
        }
        if (obstacles[i].A == jump && intersect(Box(player, 1, 1, &scheme), obstacles[i]))
        {
            player.playerFall = max(-100, (-80 * speed)/20);
            qDebug() << "lel";
        }
        if (obstacles[i].A == crash && !obstacles[i].falling)
            obstacles[i].H = min(obstacles[i].H+55, CUBE_SIZE*10);

        if (obstacles[i].falling || shake)
        {
            obstacles[i].root.Y = max(0, obstacles[i].root.Y-obstacles[i].fallSpeed);
            obstacles[i].fallSpeed += abs(speed);


            if (obstacles[i].root.Y == 0 && obstacles[i].A == crash)
            {
                if (obstacles[i].angle == 0) obstacles[i].angle = 1000 - 2000 * (rand()%2);
                if (abs(obstacles[i].angle) > 55) obstacles[i].angle = obstacles[i].angle - (1 + shake) * 10 + 20 * (obstacles[i].angle < 0);
            }
        }
    }

    for (int i = 0; i < int(particles.size()); i++)
    {
        particles[i].Y-=35, particles[i].Y = max(particles[i].Y, 0);

        if (particles[i].Z <= player.Z)
        {
                swap(particles[i], particles.back());
                particles.pop_back();
                i--;
        }
    }

    lastMaxSpeed = maxSpeed();
}

void Game::fixObstacles()
{
    for(int i=0;i<int(obstacles.size());i++) if(obstacles[i].root.Z+obstacles[i].W <= player.Z)
    {
        swap(obstacles[i], obstacles.back());
        i--;
        obstacles.pop_back();
    }
}

void Game::addObstacle()
{
    if (speed <= 0 || paused || resurrecting) return;

    fixObstacles();

    Box O;

    bool check = false;
    while (!check)
    {
        O = Box(
                    Point(
                        nextRandom(0, 4) == 0 ? player.X :
                        nextRandom(player.X-CUBE_SIZE*2, player.X+CUBE_SIZE*2),
                        0,
                        nextRandom(player.Z+CUBE_SIZE*2, player.Z+CUBE_SIZE*3)),
                    nextRandom(30, 100),
                    0,
                    &scheme
                    );

        if (O.root.X == player.X)
        {
            O.root.X += ( direction * (O.root.Z - player.Z) ) / speed;
        }

        O.angle = 55 + rand()%110;
        if (rand()%2 == 0) O.angle = - O.angle;

        if ((score/10000)%9 > 3)
        {
            if (!shake) startShake();
            O.angle = 0;
            O.falling = true;
            O.root.Y = player.Y + 25000;
            O.H = 1000 + rand()%2000;
            O.root.Z = player.Z + CUBE_SIZE*4;
        }
        else {
            shake = false;
            quakeIntensity--;
        }

        check = true;
        for (int i = 0; i < int(obstacles.size()); i++) if (intersect(O, obstacles[i])) check = false;
    }

    //qDebug() << O.root.X << " " << O.root.Y << " " << O.root.Z - player.Z << " " << O.W << " " << O.H;

    if (rand()%11 == 0)
    {
        O.A = jump;
        O.W = 40;
        O.H = 1000;
        O.angle = 0;
    }

    if (lives < 3 && rand()%19 == 0)
    {
        O.A = heal;
        O.W = 40;
        O.H = 1000;
        O.angle = 0;
    }

    obstacles.push_back(O);

    sort(obstacles.begin(), obstacles.end());
}

void Game::startShake()
{
    shake = true;
    quakeIntensity = 10;
    player.playerFall = -15;
    for (int i = 0; i < int(obstacles.size()); i++)
    {
        obstacles[i].falling = true;
        obstacles[i].fallSpeed -= speed*speed/2;
    }
}

void Game::start()
{
    obstacles.clear();
    particles.clear();

    shake = false;

    player = Camera(50, CUBE_SIZE/5, 0);

    speed = direction = 0;

    lastMaxSpeed = -10000;

    speed = 0;

    lives = 3;

    tilt = 0;

    resurrecting = false;

    obstacleTimer.start(1000);
    particleTimer.start(1000);

    //obstacles.push_back(Box(Point(player.X + 1000000, player.Y, player.Z + 1000000), 1000000-500, 1000000, &scheme));
}

Game::Game() : paused(true), speed(0), direction(0), autoSteering(0), highScore(0), snowOn(false), tiltOn(true), particlesOn(true), myPhoneSux(0), vibe(0), playerID(0), scoreToSubmit(0)
{
    reflectionsOn = true;
    coresOn = true;
    comicOn = false;
    schemeNo = 4;
    lastVersion = 0;

    firstLaunch = false;
    connect(&obstacleTimer, SIGNAL(timeout()), this, SLOT(addObstacle()));
    connect(&particleTimer, SIGNAL(timeout()), this, SLOT(addParticle()));

    loadSettings();
    saveSettings();

    //firstLaunch = true;

    if (firstLaunch) paused = false;

    scheme = ColorScheme(schemeNo);
}
