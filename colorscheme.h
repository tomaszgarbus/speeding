#ifndef COLORSCHEME_H
#define COLORSCHEME_H

#include <QColor>
#include <QString>

class ColorScheme
{

public:
    bool isRainbow;

    QColor
        menuBox,
        crashBox1,
        crashBox2,
        particle,
        particleReflection,
        ground1,
        ground2,
        groundLine,
        center,
        sky,
        font,
        contour,
        snow,
        snowReflection,
        jumpSymbol,
        jumpSymbolReflection,
        healSymbol,
        healSymbolReflection
        ;

    QString fonttype;

    ColorScheme();
    ColorScheme(int no);

};

#endif // COLORSCHEME_H
