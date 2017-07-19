//========================================================================
/*!
  @file    colormanager.cpp
  @class   ColorManager
  @date    7/8/2017
  @brief
  @author  Javad Amiryan, (C) 2017
*/
//========================================================================
#include "colormanager.h"
#include <cstdlib>
#include <qdebug.h>

namespace sdbug
{
ColorManager::ColorManager(QObject *parent) : QObject(parent)
{
    srand(time(0));
}

QColor ColorManager::getNewRandomColor()
{
    bool new_found;
    QColor c;
    int try_counter = 0;
    do {
        try_counter++;
        new_found = true;
        c = QColor(rand()%255, rand()%255, rand()%255);
        for(int i=0; i<used_colors.size(); ++i)
        {
            if(abs(c.hue() - used_colors[i].hue()) < 20 &&
               abs(c.saturation() - used_colors[i].saturation()) < 30 &&
               abs(c.value() - used_colors[i].value()) < 40 )
            {
                new_found = false;
                break;
            }
        }

    } while(!new_found && try_counter < 50);

    //qDebug() << "HUE = " << c.hue();
    used_colors.push_back(c);
    return c;
}

QColor ColorManager::getNewDifferentColor()
{
    QColor c;

    int N = used_colors.size();
    int v = ceil(log2(N+1));
    int m = round(pow(2,v));
    double a = 360./m;
    int r = m/2;
    int s = N-r;
    int p = 2*s;
    int hue = (p+1)*a;
    c.setHsv(hue, 225+rand()%30, 200+rand()%56);

    //qDebug() << "HUE = " << c.hue();
    used_colors.push_back(c);
    return c;
}

QColor ColorManager::getColor(int index)
{
    if(index< used_colors.size() && index >=0)
    {
        return used_colors.at(index);
    }
    return QColor(Qt::black);
}

}
