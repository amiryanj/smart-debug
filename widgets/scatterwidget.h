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

    void addPacket(const ScatterPacket& packet);
    void addData(float x, float y, std::string legend = "Unknown");
    void addData(const Point &point, std::string legend = "Unknown");
    void setData(const std::vector<Point> &data, std::string legend = "Unknown");
    void clearData();
    void enableRecording(bool enable);


    void mouseWheel();

    
private:
    Ui::ScatterWidget *ui;
};
}

#endif // SCATTERPLOTWIDGET_H
