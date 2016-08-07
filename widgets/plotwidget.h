#ifndef _PLOTWIDGET_H
#define _PLOTWIDGET_H

#include <QTimer>
#include <qcustomplot.h>

namespace Ui {
class PlotWidget;
}
namespace dbug {

class PlotWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlotWidget(QWidget *parent = 0);
    ~PlotWidget();

    void setName(QString name);
    void setLegendsFont(const QFont &font);

public slots:
    void addValue(double val, double key = -1, QString legend = "");
    void addValue(double key, const QVector<double> &vals, const QVector<QString> &legends);
    void forceToPause();
    void forceToPlay();

    void setConnected(bool connected);

    void setYAxisRange(double lower, double upper);

signals:
    void closeMe(QString);
    void setMeMinimized(QString, bool);
    
private:
    Ui::PlotWidget *ui;
    QGraphicsPixmapItem* statusGItem;
    QMap<QString, QPair<QCPGraph*, QCPGraph*> > graphsMap;

    double mKey;
    QString plotName;
    bool connected;
    bool freezed;

private slots:
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void graphClicked(QCPAbstractPlottable *plottable);
    void on_closeButton_clicked();
    void on_minimizeButton_clicked(bool checked);
    void on_pauseButton_clicked(bool checked);
    void on_recButton_clicked(bool checked);
};
}

#endif // _PLOTWIDGET_H
