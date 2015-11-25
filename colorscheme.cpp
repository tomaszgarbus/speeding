#include "colorscheme.h"

ColorScheme::ColorScheme()
{

}

ColorScheme::ColorScheme(int no)
{
    if (no == 3)
    {
        menuBox = QColor(0, 0, 0, 215);
        crashBox1 = QColor(250, 80, 0, 170);
        crashBox2 = QColor(0, 200, 0, 170);
        particle = QColor(170, 0, 0, 255);
        particleReflection = QColor(170, 0, 0, 120);
        ground1 = Qt::white;
        ground2 = Qt::gray;
        groundLine = QColor(170, 170, 170);
        center = Qt::white;
        sky = Qt::gray;
        font = QColor(250, 80, 0);
        contour = Qt::black;
        fonttype = "SF New Republic";
        //fonttype = "Secret Code";
        snow = QColor(200, 200, 200, 255);
        snowReflection = QColor(200, 200, 200, 35);
        jumpSymbol = QColor(255, 204, 0, 255);
        jumpSymbolReflection = QColor(255, 204, 0, 80);
        healSymbol = QColor(255, 0, 0, 255);
        healSymbolReflection = QColor(255, 0, 0, 75);
        isRainbow = false;
    }

    if (no == 1)
    {
        menuBox = QColor(100, 100, 100, 215);
        crashBox1 = QColor(200, 0, 0, 170);
        crashBox2 = QColor(215, 15, 15, 100);
        particle = QColor(255, 0, 0, 255);
        particleReflection = QColor(255, 0, 0, 120);
        ground1 = QColor(46, 46, 46);
        ground2 = Qt::black;
        groundLine = QColor(16, 16, 16);
        center = QColor(46, 46, 46);
        sky = Qt::black;
        font = Qt::gray;
        contour = Qt::gray;
        fonttype = "SF New Republic";
        snow = QColor(200, 200, 200, 255);
        snowReflection = QColor(200, 200, 200, 35);
        jumpSymbol = QColor(255, 0, 0, 255);
        jumpSymbolReflection = QColor(255, 0, 0, 80);
        healSymbol = QColor(255, 0, 0, 255);
        healSymbolReflection = QColor(255, 0, 0, 75);
        isRainbow = false;
    }

    if (no == 2)
    {
        menuBox = QColor(100, 100, 100, 215);
        crashBox1 = QColor(36, 36, 36, 170);
        crashBox2 = QColor(49, 49, 49, 100);
        particle = QColor(100, 100, 100, 255);
        particleReflection = QColor(100, 100, 100, 120);
        ground1 = QColor(86, 86, 86);
        ground2 = Qt::black;
        groundLine = QColor(36, 36, 36);
        center = QColor(86, 86, 86);
        sky = Qt::black;
        font = Qt::gray;
        contour = Qt::gray;
        fonttype = "SF New Republic";
        snow = QColor(200, 200, 200, 255);
        snowReflection = QColor(200, 200, 200, 35);
        jumpSymbol = QColor(255, 255, 0, 255);
        jumpSymbolReflection = QColor(255, 255, 0, 80);
        healSymbol = QColor(255, 0, 0, 255);
        healSymbolReflection = QColor(255, 0, 0, 75);
        isRainbow = false;
    }

    if (no == 0)
    {
        menuBox = QColor(190, 190, 170, 215);
        crashBox1 = QColor(80, 0, 80, 170);
        crashBox2 = QColor(70, 27, 126, 170);
        particle = QColor(70, 27, 126, 255);
        particleReflection = QColor(70, 27, 126, 120);
        ground1 = Qt::white;
        ground2 = Qt::gray;
        groundLine = QColor(170, 170, 170);
        center = Qt::white;
        sky = Qt::gray;
        font = QColor(70, 27, 126);
        contour = Qt::black;
        fonttype = "SF New Republic";
        snow = QColor(70, 27, 126, 255);
        snowReflection = QColor(200, 200, 200, 35);
        jumpSymbol = QColor(255, 204, 0, 255);
        jumpSymbolReflection = QColor(255, 204, 0, 80);
        healSymbol = QColor(255, 0, 0, 255);
        healSymbolReflection = QColor(255, 0, 0, 75);
        isRainbow = false;
    }

    if (no == 4)
    {
        menuBox = QColor(100, 100, 100, 215);
        crashBox1 = QColor(200, 0, 0, 170);
        crashBox2 = QColor(215, 15, 15, 100);
        particle = QColor(255, 0, 0, 255);
        particleReflection = QColor(255, 0, 0, 120);
        ground1 = QColor(46, 46, 46);
        ground2 = Qt::black;
        groundLine = QColor(16, 16, 16);
        center = QColor(46, 46, 46);
        sky = Qt::black;
        font = Qt::gray;
        contour = Qt::gray;
        fonttype = "SF New Republic";
        snow = QColor(200, 200, 200, 255);
        snowReflection = QColor(200, 200, 200, 35);
        jumpSymbol = QColor(255, 0, 0, 255);
        jumpSymbolReflection = QColor(255, 0, 0, 80);
        healSymbol = QColor(255, 0, 0, 255);
        healSymbolReflection = QColor(255, 0, 0, 75);
        isRainbow = true;
    }
}
