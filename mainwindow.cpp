#include "mainwindow.h"

#define EPS 2000

#define SIZE min(size().height(), size().width())

void MainWindow::initializeGradients()
{
    gradientDown = QLinearGradient(0, size().height()/2, 0, size().height());
    gradientDown.setColorAt(0, game->scheme.ground1);
    gradientDown.setColorAt(1, game->scheme.ground2);

    gradientUp = QRadialGradient(size().width()/2, size().height()/2, SIZE);
    gradientUp.setColorAt(0, game->scheme.center);
    gradientUp.setColorAt(1, game->scheme.sky);

    gradientMid1 = QLinearGradient(0, size().height()/2, 0, (size().height()*2)/3);
    gradientMid1.setColorAt(0, game->scheme.ground1);
    gradientMid1.setColorAt(1, Qt::transparent);

    gradientMid2 = QLinearGradient(0, size().height()/2, 0, 0);
    gradientMid2.setColorAt(0, game->scheme.center);
    gradientMid2.setColorAt(1, Qt::transparent);
}

void MainWindow::exposeEvent(QExposeEvent *e)
{
    if (!isExposed()) {
        //qDebug() << "jest super\n";
        initialized = false;
        //bgSound->stop();
        return;
    }

    //bgSound->play();

    setSurfaceType(OpenGLSurface);
    context.create();
    context.makeCurrent(this);
    initializeOpenGLFunctions();

    device = new QOpenGLPaintDevice(size());

    initializeGradients();

    font.setPixelSize(50 * size().height() / 1080);

    if (initialized) return;

    initialized = true;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if (!initialized) return;

    device->setSize(size());

    //qDebug() << size().width() << " " << size().height();

    initializeGradients();

    font.setPixelSize(50 * size().height() / 1080);
}

void MainWindow::sec()
{
    last_fps = fps_count;
    fps_count = 0;
}

MainWindow::MainWindow(Game *g, QWindow* parent) : device(), initialized(false), showFPS(false), shadow(0), unveil(false), unveilEd(false), blurLines(true), myPhoneSux(0), gradientOn(true), BENCHMARK_FRAMES(600) //pozdro 600
{
    setSurfaceType(OpenGLSurface);
    //setSource(QUrl("qrc:/main.qml"));
    //setClearBeforeRendering(false);
    //setPersistentOpenGLContext(false);
    //setPersistentSceneGraph(false);
    //setResizeMode(SizeRootObjectToView);

    last_fps = 60;

    framesElapsed = 0;

    fps_count = 0;

    timer.start(1);
    connect(&timer, SIGNAL(timeout()), this, SLOT(paint()));

    fps_timer.start(1000);
    connect(&fps_timer, SIGNAL(timeout()), this, SLOT(sec()));

    game = g;

    QFontDatabase::addApplicationFont(":/fonts/Caitlin.ttf");
    QFontDatabase::addApplicationFont(":/fonts/FaceYourFears.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Origami.ttf");
    QFontDatabase::addApplicationFont(":/fonts/SecretCode.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Maxi.ttf");
    QFontDatabase::addApplicationFont(":/fonts/New-Republic.ttf");

    font.setFamily(game->scheme.fonttype);
    font.setPixelSize(50 * size().height() / 1080);

    painter.setFont(font);

    //this->setAttribute(Qt::WA_OpaquePaintEvent, true);

    game->updateMenuBoxes();

    myPhoneSux = game->myPhoneSux;

    acc = new QAccelerometer(this);
    acc->start();

    greetingClicks = 0;

    benchmarkFPSCount = (game->firstLaunch ? 0 : BENCHMARK_FRAMES + 1);

    if (myPhoneSux > 0) game->coresOn = false;
    if (myPhoneSux > 1) blurLines = false;
    if (myPhoneSux > 2) game->particlesOn = false;
    if (myPhoneSux > 3) game->reflectionsOn = false;
    if (myPhoneSux > 4) gradientOn = false;

    appStates[0] = appStates[1] = 0;

    keyLeftDown = keyRightDown = false;
}

#define PREPP(X) fixPoint(this->size(), transformPoint(&game->player, (X)))

void MainWindow::drawLine(Point P, Point Q, QPen pen, qreal width)
{
    if (blurLines) pen.setWidthF(width);

    QPoint T = PREPP(P), R = PREPP(Q);

    painter.setPen(pen);

    painter.drawLine(T.x(), T.y(), R.x(), R.y());
}

void MainWindow::drawGround()
{

    for(int x = ((game->player.X-CUBE_SIZE*2)/100)*99; x <= game->player.X+CUBE_SIZE*2; x += 100)
    {
        drawLine(Point(x, 0, game->player.Z), Point(x, 0, game->player.Z+CUBE_SIZE+game->player.Y*10), QPen(game->scheme.groundLine), 1.f -  qreal(abs(x - game->player.X)) / qreal(CUBE_SIZE*2));
    }

    painter.setOpacity(1);

}

void MainWindow::drawWall(QPolygon wall, QColor color, bool isFront)
{
    QPainterPath path;
    path.addPolygon(wall);
    if (!isFront) color.setRgb(max(0, color.red()-30), max(0, color.green()-30), max(0, color.blue()-30), color.alpha());
    //color.setAlpha(130 + 50 * isCore);
    painter.fillPath(path, color);
    if (game->comicOn) for (int i = 0; i < 4; i++) {
        QPainterPath path;
        tiltWall(&wall);
        path.addPolygon(wall);
        //cnt.setColor(game->scheme.contour);
        //cnt.setWidth(4);
        painter.setPen(game->scheme.contour);
        painter.drawPath(path);
    }
}

void MainWindow::drawEvent(Box *O, bool reflection)
{
    if (O->A == jump)
    {
        QPainterPath path;
        path.addPolygon(O->jumpSymbol(size(), &game->player, 0, reflection));
        painter.fillPath(path, (reflection ? game->scheme.jumpSymbolReflection : game->scheme.jumpSymbol));

        path = QPainterPath();
        path.addPolygon(O->jumpSymbol(size(), &game->player, 200, reflection));
        painter.fillPath(path, (reflection ? game->scheme.jumpSymbolReflection : game->scheme.jumpSymbol));
    }
    if (O->A == heal)
    {
        QPainterPath path;
        path.addPolygon(O->healSymbol(size(), &game->player, 0, reflection));
        painter.fillPath(path, (reflection ? game->scheme.healSymbolReflection : game->scheme.healSymbol));
    }
}

void MainWindow::drawObstacle(Box *O)
{
    if (!O->visible(size(), &game->player)) return;

    if (O->A == jump)
    {
        drawEvent(O, false);
        return;
    }

    if (O->A == heal)
    {
        drawEvent(O, false);
        return;
    }

    if (game->coresOn && !O->isCore)
    {
        Box P = O->core();
        drawObstacle(&P);
    }

    if (O->root.Z + O->W < game->player.Z) return;

    if (O->leftWallVisible(&game->player))
        drawWall(O->leftWall(size(), &game->player), O->color);

    if (O->rightWallVisible(&game->player))
        drawWall(O->rightWall(size(), &game->player), O->color);

    if (O->lowerWallVisible(&game->player))
        drawWall(O->lowerWall(size(), &game->player), O->color);

    if (O->upperWallVisible(&game->player))
        drawWall(O->upperWall(size(), &game->player), O->color);

    if (O->frontWallVisible(&game->player))
        drawWall(O->frontWall(size(), &game->player), O->color, true);

    if (O->root.Y >= 0)
    {
    //painter.setPen(game->scheme.font);
    if (O->A != crash) {
        //painter.fillRect(O->frontRect(size(), &game->player, (5 * SIZE)/1080), Qt::black);
        painter.setPen(game->scheme.font);
    }
    if (!O->isCore && O->A == resume)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString("resume"));
    }
    if (!O->isCore && O->A == snow)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString(game->snowOn ? "turn the snow off" : "let it snow"));
    }
    if (!O->isCore && O->A == restart)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString("new game"));
    }
    if (!O->isCore && O->A == reflection)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString("toggle reflections"));
    }
    if (!O->isCore && O->A == core)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString("toggle cores"));
    }
    if (!O->isCore && O->A == done)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString("exit game"));
    }
    if (!O->isCore && O->A == colorscheme)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString(game->schemeNo == 0 ? "paint it black" : "change color scheme"));
    }
    if (!O->isCore && O->A == steering)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString(game->tiltOn ? "steering: tilt" : "steering: touch"));
    }
    if (!O->isCore && O->A == comicstyle)
    {
        painter.drawText(O->frontRect(size(), &game->player), Qt::AlignCenter | Qt::TextWordWrap, QString("switch style"));
    }
    }
}

void MainWindow::drawParticle(Point O)
{   
    if (game->snowOn) {

        painter.setPen(O.Y >= 0 ? game->scheme.snow : game->scheme.snowReflection);

           //if (game->scheme.isRainbow) painter.setPen(randomColor());
        if (O.Y == 0) {
            painter.drawLine(PREPP(Point(O.X-10, O.Y, O.Z)), PREPP(Point(O.X+10, O.Y, O.Z)));
            painter.drawLine(PREPP(Point(O.X, O.Y, O.Z-10)), PREPP(Point(O.X, O.Y, O.Z+10)));
            painter.drawLine(PREPP(Point(O.X-10, O.Y, O.Z-10)), PREPP(Point(O.X+10, O.Y, O.Z+10)));
            painter.drawLine(PREPP(Point(O.X+10, O.Y, O.Z-10)), PREPP(Point(O.X-10, O.Y, O.Z+10)));
        }
        else {
            painter.drawLine(PREPP(Point(O.X-10, O.Y, O.Z)), PREPP(Point(O.X+10, O.Y, O.Z)));
            painter.drawLine(PREPP(Point(O.X, O.Y-160, O.Z)), PREPP(Point(O.X, O.Y+160, O.Z)));
            painter.drawLine(PREPP(Point(O.X-10, O.Y-120, O.Z)), PREPP(Point(O.X+10, O.Y+120, O.Z)));
            painter.drawLine(PREPP(Point(O.X+10, O.Y-120, O.Z)), PREPP(Point(O.X-10, O.Y+120, O.Z)));
        }
        return;
    }

    if (game->scheme.isRainbow) game->scheme.particle = randomColor(), game->scheme.particleReflection = randomColor(), game->scheme.particleReflection.setAlpha(170);

    if (O.Y == 0)
    {
        QPolygon poly = lyingParticle(size(), &game->player, O);
        QPainterPath path; path.addPolygon(poly);
        painter.fillPath(path, game->scheme.particle);
    }
    else
    {
        QPoint P = PREPP(O);
        QPoint Q = PREPP(Point(O.X+10, O.Y+80, O.Z));
        painter.fillRect(P.x(), P.y(), Q.x()-P.x(), Q.y()-P.y(), O.Y > 0 ? game->scheme.particle : game->scheme.particleReflection);
        return;
    }
}

const QString greetingString[] =
{
    "welcome (back) to SPEEDING\n\na game about obstacles",
    "\"Obstacles are those frightful things\nyou see when you take\nyour eyes off your goal\"\n\n\n~Henry Ford",
    "...just tilt your phone\nto dodge obstacles\n\ngood luck"
};

void MainWindow::drawGreeting()
{
    painter.fillRect(size().width()/6+(10*SIZE)/1080, size().height()/6+(10*SIZE)/1080, 2*(size().width()/3), 2*(size().height()/3), QColor(70, 27, 126, 170));
    painter.fillRect(size().width()/6, size().height()/6, 2*(size().width()/3), 2*(size().height()/3), QColor(0, 0, 0, 220));

    painter.setPen(game->scheme.font);
    painter.drawText(QRect(size().width()/6+(5*SIZE)/1080, size().height()/6+(5*SIZE)/1080, 2*(size().width()/3), 2*(size().height()/3)), Qt::TextWordWrap | Qt::AlignCenter,
                     greetingString[greetingClicks/2]);
    painter.setPen(Qt::white);
    painter.drawText(QRect(size().width()/6, size().height()/6, 2*(size().width()/3), 2*(size().height()/3)), Qt::TextWordWrap | Qt::AlignCenter,
                     greetingString[greetingClicks/2]);
    painter.setPen(game->scheme.font);

    painter.drawText(QRect(size().width()/6+(5*SIZE)/1080, 2*(size().height()/3)+(5*SIZE)/1080, 2*(size().width()/3), size().height()/6), Qt::TextWordWrap | Qt::AlignCenter,
                     QString("tap twice anywhere on the screen to proceed"));

    painter.setPen(Qt::white);
    painter.drawText(QRect(size().width()/6, 2*(size().height()/3), 2*(size().width()/3), size().height()/6), Qt::TextWordWrap | Qt::AlignCenter,
                     QString("tap twice anywhere on the screen to proceed"));
    painter.setPen(game->scheme.font);
}

void MainWindow::drawPauseButton()
{
    //painter.setOpacity(0.5f);
    painter.fillRect((120 * SIZE)/1080, size().height() - (80 * SIZE)/1080, (10 * SIZE)/1080, (30 * SIZE)/1080, game->scheme.font);
    painter.fillRect((140 * SIZE)/1080, size().height() - (80 * SIZE)/1080, (10 * SIZE)/1080, (30 * SIZE)/1080, game->scheme.font);
    painter.drawArc((95 * SIZE)/1080, size().height()-(105*SIZE)/1080, (80 * SIZE)/1080, (80 * SIZE)/1080, 0, 5760);
    //painter.setOpacity(1);
    //painter.drawRect((100 * SIZE)/1080, size().height() - (100 * SIZE)/1080, (70 * SIZE)/1080, (70 * SIZE)/1080);
}

int MainWindow::newsFall(int pos)
{
    pos = min(pos, SIZE-pos);
    return max(2, 2 * (10 + pos * 10) / SIZE) * max(2, 2 * (10 + pos * 10) / SIZE);
}

void MainWindow::paint()
{
    requestActivate();
    //showFPS = true;

    if (!initialized) return;

    //game->coresOn = false;

    //qDebug() << isExposed() << ariMean(&benchmarkVector) << this->windowState();
    appStates[0] = appStates[1];
    appStates[1] = app->applicationState();
    //qDebug() << app->applicationState();

    /*if (game->vibe)RESTORE
    {
        qDebug() << "jest super";
        QAndroidJniObject::callStaticMethod<void>("org/qtproject/example/speeding/Vibrate", "start", "(I)V", abs(game->speed+game->speed));
        game->vibe = false;
    }*/

    if (appStates[0] == 4 && appStates[1] != 4) game->paused = true;

    if (benchmarkFPSCount < BENCHMARK_FRAMES)
    {
        game->benchmarkOn = true;
        benchmarkFPSCount++;
        if (last_fps != 0) benchmarkVector.push_back(last_fps);

        if (myPhoneSux < 5 && benchmarkFPSCount >= 200 && int(benchmarkVector.size()) >= 70 && ariMean(&benchmarkVector) < 48.f && SIZE < 1000)
        {
            if (myPhoneSux == 0) game->coresOn = false;
            if (myPhoneSux == 1) blurLines = false;
            if (myPhoneSux == 2) game->particlesOn = false;
            if (myPhoneSux == 3) game->reflectionsOn = false;
            if (myPhoneSux == 4) gradientOn = false;

            myPhoneSux++;
            benchmarkVector.clear();
            BENCHMARK_FRAMES += 90;
        }

    }
    else game->benchmarkOn = false;

    if (benchmarkFPSCount == BENCHMARK_FRAMES)
    {
        game->start();
        game->benchmarkOn = false;
        benchmarkFPSCount++;
        if (SIZE >= 1000) myPhoneSux = 0;
        game->myPhoneSux = myPhoneSux;
        game->saveSettings();
        //game->tiltOn = true;
    }

    if (game->tiltOn)
    {
        if (abs(acc->reading()->z()) > 18.f) game->paused = true;

        qreal tmp = acc->reading()->y();
        tmp = (1- 2 * (tmp < 0)) * min(5.d, abs(tmp));


        if (int(tiltHistory.size()) == 7)
        {
            tiltSum -= tiltHistory.front();
            tiltHistory.pop_front();
        }

        tiltSum += tmp;
        tiltHistory.push_back(tmp);

        tmp = tiltSum/7.0f;

        game->direction = (tmp * 14) / 5;

        game->player.tilt = float(game->tilt)/30.f - float(tmp*25.f*abs(acc->reading()->x())/50.f);
    }
    else
    {
        game->player.tilt = float(game->tilt)/30.f;

        if (keyRightDown)
        {
            touchHistory.push_back(9.f);
            touchSum += 9.f;
        }
        else if (keyLeftDown)
        {
            touchHistory.push_back(-9.f);
            touchSum -= 9.f;
        }
        else
        {
            touchHistory.push_back(0.f);
        }
        while (int(touchHistory.size()) > 50-game->speed/2)
        {
            touchSum -= touchHistory.front();
            touchHistory.pop_front();
        }
        game->direction = touchSum/float(50-game->speed/2);
    }

    //game->update((last_fps == 0 ? 1: 64 / last_fps));
    game->update(1);
    //qDebug() << fps_count;

    glClearColor(1, 1, 1, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    context.makeCurrent(this);

    painter.begin(device);

    if (!game->paused) unveilEd = false;

    if (unveil && shadow == 0) unveilEd = true, unveil = false;

    if (unveil) shadow = max(shadow-5, 0);

    if (game->paused && game->lives > 0) unveil = true;

    if (game->paused && game->lives == 0 && shadow >= size().height()/2) unveil = true;

    if (!unveilEd && !unveil && game->lives == 0) shadow += 5;

    if (!unveil && game->lives > 0 && shadow > (3-game->lives) * size().height() / 6) shadow--;

    if (!unveil && game->lives > 0 && shadow < (3-game->lives) * size().height() / 6) shadow++;

    if (!game->paused)
    {
        painter.translate(size().width()/2, size().height()/2);
        painter.rotate(game->player.tilt);
        painter.translate(-size().width()/2, -size().height()/2);
    }

    if (game->shake) painter.translate(game->xShift*SIZE/1080, game->yShift*SIZE/1080);

    //speed_down->setGeometry(this->size().width()-200, 0, 100, 100);
    //speed_up->setGeometry(this->size().width()-100, 0, 100, 100);

    fps_count++;

    //qDebug() << game->obstacles.size();

    if (gradientOn)
    {
        painter.fillRect(-EPS, size().height()/2, size().width() + 2*EPS, size().height()/2 + EPS, gradientDown);
        painter.fillRect(-EPS, size().height()/2, size().width() + 2*EPS, -size().width()/2 - EPS, gradientUp);
        painter.fillRect(-EPS, size().height()/2, size().width() + 2*EPS, -size().width()/2 - EPS, gradientMid2);
    }

    else
    {
        painter.fillRect(-EPS, -EPS, size().width()+2*EPS, size().height()+2*EPS, game->scheme.sky);
    }

    drawGround();

    QString string;
    painter.setFont(font);

    if (!game->benchmarkOn)
    {
        string = QString( (iHeartString(game->lives) + " " + (showFPS ? "FPS: " + toString(last_fps) : "") ).c_str() );
        painter.setPen(game->scheme.font);
        if (showFPS) painter.drawText(0, (50 * SIZE) / 1080, string);

        painter.setOpacity(0.7f);
        string = QString( ("SCORE: " + toString(game->player.Z/25)).c_str() );
        painter.drawText(size().width()/2 - (200 * size().width()) / 1920, size().height()/2 - (30 * SIZE) / 1080, string);
        painter.setOpacity(1);
    }

    //qDebug() << string;


    //painter.setOpacity(1);

    if (game->reflectionsOn)
    {
        for(int i=0;i<int(game->particles.size());i++)
        {
            drawParticle(game->particles[i].reflection());
        }

        for (int i = 0; i < int(game->obstacles.size()); i++)
        {
            if (!game->paused || game->obstacles[i].A != crash || game->obstacles[i].root.Z-game->obstacles[i].W >= game->player.Z+350)
            {
                if (game->obstacles[i].A != jump && game->obstacles[i].A != heal)
                {
                    Box P = game->obstacles[i].reflection();
                    drawObstacle(&P);
                }
                else
                {
                    drawEvent(&game->obstacles[i], true);
                }
            }
        }
    }

    if (gradientOn) painter.fillRect(-EPS, size().height()/2, size().width() + 2*EPS, size().height()/2 + EPS, gradientMid1);

    for(int i=0;i<int(game->particles.size());i++)
    {
        drawParticle(game->particles[i]);
    }
    for(int i=0;i<int(game->obstacles.size());i++)
    {
        if (!game->paused || game->obstacles[i].A != crash || game->obstacles[i].root.Z-game->obstacles[i].W >= game->player.Z+350)
        {
            drawObstacle(&game->obstacles[i]);
        }

    }

    //if (game->paused) paintMenu();

    if (!game->paused)
    {
        painter.translate(size().width()/2, size().height()/2);
        painter.rotate(-game->player.tilt);
        painter.translate(-size().width()/2, -size().height()/2);
    }

    painter.fillRect(-shadow, -shadow, 2*shadow + size().width(), shadow*2, QColor(0, 0, 0, 240));

    painter.fillRect(-shadow, size().height()-shadow, 2*shadow + size().width(), size().height(), QColor(0, 0, 0, 240));

    if (game->firstLaunch && benchmarkFPSCount >= BENCHMARK_FRAMES)
    {
        drawGreeting();
    }

    if (!game->paused)
    {
        painter.setPen(QPen(game->scheme.font, (10 * SIZE)/1080));
        painter.drawArc(size().width()-(110*SIZE)/1080, (10*SIZE)/1080, (100*SIZE)/1080, (100*SIZE)/1080, -49, -16*360*game->speed/85+nextRandom(0, 96));

        painter.drawText(size().width()-(90*SIZE)/1080, (75*SIZE)/1080, toString(game->speed).c_str());

        drawPauseButton();
    }

    painter.setOpacity(1.0f);

    if (!game->benchmarkOn)
    {
        string = ("HIGH: " + toString(game->highScore/25)).c_str();
        painter.drawText((45 * SIZE) / 1080, (78 * SIZE) / 1080, string);
    }


    if (benchmarkFPSCount < BENCHMARK_FRAMES)
    {
        painter.fillRect(0, 0, size().width(), size().height(), QColor(255, 255, 255, 252));
        painter.drawText(0, 0, size().width(), size().height(), Qt::AlignCenter, "LOADING...");
    }

    painter.end();
    context.swapBuffers(this);

    //for (int i = 0; i < 100; i++) painter.drawLine(0, i, i, 0);
    //qDebug() << this->testAttribute(Qt::WA_OpaquePaintEvent);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (game->firstLaunch) return;
    if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Back)
    {
        game->paused = !game->paused;
    }
    if (!game->paused)
    {
        if (!game->tiltOn && e->key() == Qt::Key_Left && !game->autoSteering) keyLeftDown = true;
        if (!game->tiltOn && e->key() == Qt::Key_Right && !game->autoSteering) keyRightDown = true;
    }
    if (e->key() == Qt::Key_F) showFPS ^= 1;
    if (e->key() == Qt::Key_A) game->autoSteering ^= 1;
    if (e->key() == Qt::Key_S) game->startShake();
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{

    if (!game->tiltOn && e->key() == Qt::Key_Left && !game->autoSteering) keyLeftDown = keyRightDown = false;
    if (!game->tiltOn && e->key() == Qt::Key_Right && !game->autoSteering) keyLeftDown = keyRightDown = false;

}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    //this->close();
    if (!game->paused)
    {
        if ( (e->x() - (135 * SIZE)/1080) * (e->x() - (135 * SIZE)/1080) + (e->y() - size().height() + (65 * SIZE)/1080) * (e->y() - size().height() + (65 * SIZE)/1080) <=  ((140 * SIZE)/1080) * ((140 * SIZE)/1080) )
        //if (e->x() >= (100 * SIZE)/1080 && e->y() >= size().height()-(100 * SIZE)/1080 && e->x() <= (170 * SIZE)/1080 && e->y() <= size().height() - (30 * SIZE)/1080)
                game->paused = true;
    }
    if (game->firstLaunch && benchmarkFPSCount >= BENCHMARK_FRAMES)
    {
        greetingClicks++;
        if (greetingClicks >= 6) game->firstLaunch = false;
        game->saveSettings();
        return;
    }
    if (game->paused)
    {
        if (game->resumeBox.clicked(size(), &game->player, QPoint(e->x(), e->y())))
        {
            game->paused = false;
        }
        if (game->restartBox.clicked(size(), &game->player, QPoint(e->x(), e->y())))
        {
            game->start();
            game->paused = false;
            unveil = true;
            unveilEd = false;
        }
        if (game->steeringBox.clicked(size(), &game->player, QPoint(e->x(), e->y())))
        {
            game->tiltOn ^= 1;
            if (!game->tiltOn)
            {
                game->direction = game->player.tilt = 0;
            }
            game->saveSettings();
        }
        if (game->closeBox.clicked(size(), &game->player, QPoint(e->x(), e->y())))
        {
            this->close();
        }
        if (game->snowBox.clicked(size(), &game->player, QPoint(e->x(), e->y())))
        {
            game->snowOn ^= 1;
        }
        if (game->schemeBox.clicked(size(), &game->player, QPoint(e->x(), e->y())))
        {
            game->schemeNo = (game->schemeNo+1)%5;
            game->scheme = ColorScheme(game->schemeNo);
            font.setFamily(game->scheme.fonttype);
            painter.setFont(font);
            initializeGradients();
            game->saveSettings();
        }
    }
    else if (!game->tiltOn)
    {
        if (e->x() > this->size().width()/2) keyLeftDown = true;
        else keyRightDown = true;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    if (game->paused)
    {

    }
    else
    {
       keyLeftDown = keyRightDown = false;
    }
}

MainWindow::~MainWindow()
{

}
