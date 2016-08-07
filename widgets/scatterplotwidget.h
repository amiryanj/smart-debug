//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#ifndef _SCATTERPLOTWIDGET_H
#define _SCATTERPLOTWIDGET_H

#include <QWidget>
#include "qcustomplot/qcustomplot.h"

namespace Ui {
class ScatterPlotWidget;
}

class ScatterPlotWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ScatterPlotWidget(QWidget *parent = 0);
    ~ScatterPlotWidget();

    void setData(const QVector<QPoint> &data);
    void addData(const QPointF &p);
    void clearData();
    
private:
    Ui::ScatterPlotWidget *ui;
};

#endif // SCATTERPLOTWIDGET_H
