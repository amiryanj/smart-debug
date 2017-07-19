//========================================================================
/*!
  @file    colormanager.h
  @class   ColorManager
  @date    7/8/2017
  @brief
  @author  Javad Amiryan, (C) 2017
*/
//========================================================================
#ifndef _COLORMANAGER_H
#define _COLORMANAGER_H

#include <QColor>
#include <QObject>
namespace sdbug
{

class ColorManager: public QObject
{
    Q_OBJECT
public:
    explicit ColorManager(QObject* parent = 0);
    QColor getNewRandomColor();
    QColor getNewDifferentColor();
    QColor getColor(int index);

protected:
    QList<QColor> used_colors;

};


}

#endif // PLOTCOLORMANAGER_H
