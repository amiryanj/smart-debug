//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================
#ifndef _BUILTINWIDGETMANAGER_H
#define _BUILTINWIDGETMANAGER_H

#include <QScrollArea>
#include <QBoxLayout>
#include <QMutex>
#include <QTimer>
#include <QMap>

#include "plotter.h"
#include "scatter.h"
#include "widgetmanger.h"

namespace Ui {
class BuiltInWidgetManager;
}
namespace dbug
{
    class PlotterWidget;
    class ScatterWidget;

    class BuiltInWidgetManager : public QWidget, public WidgetManger
    {
        Q_OBJECT
    public:
        explicit BuiltInWidgetManager(QWidget *parent = 0);
        ~BuiltInWidgetManager();

    public slots:
        void addPlotter(QString name, PlotterWidget *plot_widget);
        void addScatter(QString name, ScatterWidget *scatter_widget);
//        void scatter(const ScatterPacket &packet);

    private slots:
        void turnOffWidget(const QString &name);

        void on_saveToButton_clicked();
        void on_palyButton_clicked();
        void on_pauseButton_clicked();
        void on_recordButton_clicked(bool checked);

    private:
        Ui::BuiltInWidgetManager *ui;
        QMap<QString , PlotterWidget*> mPlotsMap;
        QMap<QString , ScatterWidget*> mScattersMap;
//        void savePlotterPacket(double key, const PlotterPacket& packet);
    };

}
#endif // _BUILTINWIDGETMANAGER_H
