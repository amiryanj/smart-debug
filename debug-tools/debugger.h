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

        virtual void print(const char* msg, double time, const std::string &category = "general") {};
        virtual void print(const char *msg, const std::string &category = "general") {};

        virtual void plot(PlotterPacket &packet) {};
        virtual void plot(const std::string &name, double value, double key = -1, const std::string &category = "general") {};

        virtual void scatter(ScatterPacket &packet) {};
        virtual void scatter(double x, double y, std::string name = "", const std::string &category = "general") {};


        virtual Plotter* getPlotter(const std::string &name) {
            static Plotter* empty_plotter = new Plotter;
            return empty_plotter;
        }
        virtual Scatter* getScatter(const std::string &name) {
            static Scatter* empty_scatter = new Scatter;
            return empty_scatter;
        }

    };

}


#endif // DEBUGCLIENT_H
