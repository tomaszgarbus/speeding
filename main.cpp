#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mainwindow.h"
#include "game.h"

int screen_width, screen_height;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    srand(666);

    Game game;
    game.start();

    MainWindow window(&game);
    //window.setClearBeforeRendering(false);
    //window.setSource(QUrl("qrc:/main.qml"));
    //window.setResizeMode(MainWindow::SizeRootObjectToView);
    window.show();

    window.app = &a;
    //window.showFullScreen();

    screen_width = window.size().width();
    screen_height = window.size().height();

    qDebug() << "Found screen " << screen_width << "x" << screen_height << "y";

    a.exec();
    return 0;
}
