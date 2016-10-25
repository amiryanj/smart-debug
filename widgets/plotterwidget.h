#ifndef _PLOTWIDGET_H
#define _PLOTWIDGET_H

#include <QTimer>
#include <qcustomplot.h>
#include "plotter.h"

namespace Ui {
class PlotterWidget;
}
namespace dbug {

class PlotterWidget : public QWidget, public Plotter
{
    Q_OBJECT
    
public:
    explicit PlotterWidget();
    ~PlotterWidget();

    void setName(QString name);
    void setLegendsFont(const QFont &font);

public slots:
    void addValue(double val, double key = -1, std::string legend = "");
    void addPacket(const PlotterPacket &packet);

    void addValue(double key, const QVector<double> &vals, const QVector<QString> &legends);
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

    QString plotName;
    bool connected;
    bool freezed;
};
}

#endif // _PLOTWIDGET_H