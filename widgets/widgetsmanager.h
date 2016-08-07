//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#ifndef _PLOTMANAGERWIDGET_H
#define _PLOTMANAGERWIDGET_H

#include <QScrollArea>
#include <QBoxLayout>
#include <QMutex>
#include <QTimer>
#include <QMap>
#include "plotterpacket.h"

namespace Ui {
class WidgetsManager;
}
namespace dbug
{
    class PlotWidget;

    class WidgetsManager : public QWidget
    {
        Q_OBJECT
    public:
        explicit WidgetsManager(QWidget *parent = 0);
        ~WidgetsManager();

    public slots:
        void plot(const PlotterPacket &packet);

    private slots:
        void turnOffWidget(const QString &name);

        void on_saveToButton_clicked();
        void on_palyButton_clicked();
        void on_pauseButton_clicked();

        void on_recordButton_clicked(bool checked);

    private:
        Ui::WidgetsManager *ui;
        QMap<QString , PlotWidget*> mPlotsMap;

        void savePlotterPacket(double key, const PlotterPacket& packet);
    };

}
#endif // PLOTMANAGERWIDGET_H
