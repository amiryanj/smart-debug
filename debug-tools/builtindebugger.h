#ifndef BUILTINDEBUG_H
#define BUILTINDEBUG_H

#include "widgetsmanager.h"
#include "debugger.h"
#include <QObject>
#include <QString>

namespace dbug
{
    class BuiltInDebug : public QObject , public Debugger
    {
        Q_OBJECT
    public:
        BuiltInDebug(QWidget* parent = 0);
        QWidget* widget() const {
            return plot_manager;
        }
        ~BuiltInDebug() {}

        void print(const char *msg, const std::string &category = "general");
        void print(const char* msg, double time, const std::string &category = "general");

        void plot(PlotterPacket &packet);
        void plot(const std::string &name, double value, double key = -1, const std::string &category = "general");

    //    void drawCircle();
    //    void drawLine();
    //    void drawRect();

    signals:
        void plotRequest(const PlotterPacket &p);
        void printRequest(QString msg , QString category);

    protected:
        PlotterPacket under_use_plotter_packet;
        WidgetsManager* plot_manager;


    };

}

#endif // BUILTINDEBUG_H
