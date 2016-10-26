#include "builtindebugger.h"
#include <QWidget>
#include <QVector>
#include <QDateTime>
#include <iostream>
#include <sstream>

using namespace std;
namespace dbug
{
BuiltInDebugger::BuiltInDebugger(QWidget *parent) : QObject()
{
    widgets_manager = new BuiltInWidgetManager(parent);
    widgets_manager->show();
}

void BuiltInDebugger::print(const char *msg, double time, const string &category)
{
    ostringstream ss;
    ss << msg << "  " << time;
    const char * m = ss.str().c_str();
    emit printRequest(m, QString::fromStdString(category));
    //  cout << "[" << category << "] " << "(" << time << "): " << msg << endl;
}

void BuiltInDebugger::print(const char *msg, const string &category)
{
    emit printRequest(msg, QString::fromStdString(category));
    //  cout << "[" << category << "]:" << msg << endl;
}

void BuiltInDebugger::plot(PlotterPacket &packet)
{
    if(packet.key < 0)
        packet.key = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()/1000.;
    getPlotter(packet.name)->addPacket(packet);
}

void BuiltInDebugger::plot(const string &name, double value, double key, const string &category)
{    
    PlotterPacket packet;
    packet.append(name, value);
    packet.key = key;
    packet.name = category;
    plot(packet);
}

void BuiltInDebugger::scatter(ScatterPacket &packet)
{
    getScatter(packet.name)->addPacket(packet);
}

void BuiltInDebugger::scatter(double x, double y, string name, const string &category)
{
    ScatterPacket packet;
    packet.point.x = x;
    packet.point.y = y;
    packet.name = category;
    packet.legend = name;
    scatter(packet);
}

PlotterWidget *BuiltInDebugger::getPlotter(const string &name)
{
    PlotterWidget *widget;
    auto itr = plotterMap.find(name);
    if(itr == plotterMap.end()) {
        widget = new PlotterWidget();
        widget->setCategory(name);
        widget->setParent(this->widgets_manager);
        widgets_manager->addPlotter(QString::fromStdString(name), widget);
        plotterMap.insert(make_pair(name, widget));
    }
    else
        widget = static_cast<PlotterWidget*>(itr->second);
    return widget;
}

ScatterWidget *BuiltInDebugger::getScatter(const string &name)
{
    ScatterWidget *widget = NULL;
    auto itr = scatterMap.find(name);
    if(itr == scatterMap.end()) {
        widget = new ScatterWidget();
        widget->setCategory(name);
        widget->setParent(widgets_manager);
        widgets_manager->addScatter(QString::fromStdString(name), widget);
        scatterMap.insert(make_pair(name, widget));
    }
    else
        widget = static_cast<ScatterWidget*>(itr->second);
    return widget;
}


}
