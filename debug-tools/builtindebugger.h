#ifndef BUILTINDEBUG_H
#define BUILTINDEBUG_H

#include "builtinwidgetmanager.h"
#include "debugger.h"
#include "scatterwidget.h"
#include "plotterwidget.h"
#include <QObject>
#include <QString>

namespace sdbug
{
    class BuiltInDebugger : public QObject , public Debugger
    {
        Q_OBJECT
    public:
        BuiltInDebugger(QWidget* parent = 0);
        BuiltInWidgetManager* widgetManager() const {
            return widgets_manager;
        }
        ~BuiltInDebugger() {}

        /// c-style functions
        void print(const char *msg, const std::string &category = "general");
        void print(const char* msg, double time, const std::string &category = "general");

        void plot(PlotterPacket &packet);
        void plot(const std::string &name, double value, double key = -1, const std::string &category = "general");

        void scatter(ScatterPacket &packet);
        void scatter(double x, double y, std::string name = "", const std::string &category = "general");

        PlotterWidget *getPlotter(const std::string &name);
        ScatterWidget *getScatter(const std::string &name);

    //    void drawCircle();
    //    void drawLine();
    //    void drawRect();

    signals:
        void printRequest(QString msg , QString category);

    protected:
        PlotterPacket under_use_plotter_packet;
        BuiltInWidgetManager* widgets_manager;


    };

}

#endif // BUILTINDEBUG_H
