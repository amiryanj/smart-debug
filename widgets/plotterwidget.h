#ifndef _PLOTWIDGET_H
#define _PLOTWIDGET_H

#include <QTimer>
#include <qcustomplot.h>
#include "plotter.h"

namespace Ui {
class PlotterWidget;
}
namespace sdbug {

class PlotterWidget : public QWidget, public Plotter
{
    Q_OBJECT
    
public:
    explicit PlotterWidget();
    ~PlotterWidget();

    void setCategory(const string &category_);
    void setLegendsFont(const QFont &font);
    void enableRecording(bool enable);

public slots:
    void addPacket(const PlotterPacket &packet);
    void addValue(double val, double key = -1, std::string legend = "");
    void reset();

    void forceToPause();
    void forceToPlay();
    void setConnected(bool connected);
    void setYAxisRange(double lower, double upper);

private slots:
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void graphClicked(QCPAbstractPlottable *plottable);
    void on_closeButton_clicked();
    void on_minimizeButton_clicked(bool checked);
    void on_pauseButton_clicked(bool checked);
    void on_recButton_clicked(bool checked);

signals:
    void closeMe(QString);
    void setMeMinimized(QString, bool);
    
private:
    Ui::PlotterWidget *ui;
    QGraphicsPixmapItem* statusGItem;

    bool connected;
    bool freezed;
};
}

#endif // _PLOTWIDGET_H
