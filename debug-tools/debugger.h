#ifndef DEBUGCLIENT_H
#define DEBUGCLIENT_H

#include <QVector2D>
#include <string>
#include <vector>
#include "plotterpacket.h"
#include "scatterpacket.h"

namespace dbug {

    class Debugger
    {
    protected:

    public:
        Debugger(QWidget* parent = 0) {}
        virtual QWidget *widget() const { return NULL;}
        virtual ~Debugger() {;}

        virtual void print(const char* msg, double time, const std::string &category = "general") {}
        virtual void print(const char *msg, const std::string &category = "general") {}

        virtual void plot(PlotterPacket &packet) = 0;
        virtual void plot(const std::string &name, double value, double key = -1, const std::string &category = "general") {}

        virtual void scatter(ScatterPacket &packet) = 0;
        virtual void scatter(double x, double y, std::string name = "", const std::string &category = "general") {}

        virtual void drawCircle() {}
        virtual void drawLine() {}
        virtual void drawRect() {}

    };

}


#endif // DEBUGCLIENT_H
