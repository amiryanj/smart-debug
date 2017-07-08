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
#include "colormanager.h"

namespace Ui {
class ScatterWidget;
}
namespace sdbug {
class ScatterWidget : public QWidget, public Scatter
{
    Q_OBJECT
    
public:
    explicit ScatterWidget();
    ~ScatterWidget();

    void addPacket(const ScatterPacket& packet);
    void addData(float x, float y, std::string legend = "Unknown Data");
    void addData(const PointD &point, std::string legend = "Unknown Data");
    void setData(const std::vector<PointD> &data, std::string legend = "Unknown Data");
    void clearData(std::string legend = "");
    void addBaseLine(const PointD &p1, const PointD &p2, std::string legend = "Unknown Line");
    void addLineSegment(const PointD &p1, const PointD &p2, std::string legend = "");
    void enableRecording(bool enable);
    void setRenderPriority(bool real_time);

private slots:
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void zoomToAllData();
    
    void on_eraseButton_clicked();
    void on_snapButton_clicked();
    void on_zoomButton_clicked();

private:
    Ui::ScatterWidget *ui;
    ColorManager colorManager;
};
}

#endif // SCATTERPLOTWIDGET_H
