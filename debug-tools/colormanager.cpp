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
namespace sdbug
{
ColorManager::ColorManager(QObject *parent) : QObject(parent)
{

}

QColor ColorManager::getNewColor()
{
    bool new_found;
    QColor c;
    do {
        new_found = true;
        c = QColor(rand()%255, rand()%255, rand()%255);
        for(int i=0; i<used_colors.size(); ++i)
        {
            if(c == used_colors[i])
            {
                new_found = false;
                break;
            }
        }

    } while(!new_found);
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
