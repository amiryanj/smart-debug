//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#ifndef _SCATTERWIDGET_H
#define _SCATTERWIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"
#include "scatter.h"

namespace Ui {
class ScatterWidget;
}
namespace dbug {
class ScatterWidget : public QWidget, public Scatter
{
    Q_OBJECT
    
public:
    explicit ScatterWidget();
    ~ScatterWidget();

    void setData(const QVector<QPoint> &data);
    virtual void addPacket(const ScatterPacket& packet) {}
    void addData(const QPointF &p);
    void addData(float x, float y);
    void clearData();

    void mouseWheel();

    
private:
    Ui::ScatterWidget *ui;
};
}

#endif // SCATTERPLOTWIDGET_H
