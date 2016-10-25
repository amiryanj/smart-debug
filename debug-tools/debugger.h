#ifndef _DEBUGGER_H
#define _DEBUGGER_H

#include <map>
#include "plotter.h"
#include "scatter.h"
#include "widgetmanger.h"
#include "logger.h"

namespace dbug {

    class Debugger
    {
    protected:
        std::map<std::string, Plotter*> plotterMap;
        std::map<std::string, Scatter*> scatterMap;

    public:
        Debugger() { }
        virtual WidgetManger *widgetManager() const { return NULL;}
        virtual ~Debugger() {;}

        virtual void print(const char* msg, double time, const std::string &category = "general") = 0;
        virtual void print(const char *msg, const std::string &category = "general") = 0;

        virtual void plot(PlotterPacket &packet) = 0;
        virtual void plot(const std::string &name, double value, double key = -1, const std::string &category = "general") {}

        virtual void scatter(ScatterPacket &packet) = 0;
        virtual void scatter(double x, double y, std::string name = "", const std::string &category = "general") {}

        virtual void drawCircle() {}
        virtual void drawLine() {}
        virtual void drawRect() {}


        virtual Plotter* getPlotter(const std::string &name) { (void) name; }
        virtual Scatter* getScatter(const std::string &name) { (void) name; }

    };

}


#endif // DEBUGCLIENT_H
