#include "builtindebugger.h"
#include <QWidget>
#include <QVector>
#include <QDateTime>
#include <iostream>
#include <sstream>

using namespace std;
namespace dbug
{
BuiltInDebug::BuiltInDebug(QWidget *parent) : QObject()
{
    widgets_manager = new WidgetsManager(parent);
    widgets_manager->show();
    connect(this, &BuiltInDebug::plotRequest, widgets_manager, &WidgetsManager::plot);
    connect(this, &BuiltInDebug::scatterRequest, widgets_manager, &WidgetsManager::scatter);
}

void BuiltInDebug::print(const char *msg, double time, const string &category)
{
    ostringstream ss;
    ss << msg << "  " << time;
    const char * m = ss.str().c_str();
    emit printRequest(m, QString::fromStdString(category));
    //  cout << "[" << category << "] " << "(" << time << "): " << msg << endl;
}

void BuiltInDebug::print(const char *msg, const string &category)
{
    emit printRequest(msg, QString::fromStdString(category));
    //  cout << "[" << category << "]:" << msg << endl;
}

void BuiltInDebug::plot(PlotterPacket &packet)
{
    if(packet.key < 0)
        packet.key = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()/1000.;
    emit plotRequest(packet);
}

void BuiltInDebug::plot(const string &name, double value, double key, const string &category)
{
    if(key < 0)
        key = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()/1000.;
    PlotterPacket packet;
    packet.append(name, value);
    packet.key = key;
    packet.name = category;
    emit plotRequest(packet);
}

void BuiltInDebug::scatter(ScatterPacket &packet)
{
    scatterRequest(packet);
}

void BuiltInDebug::scatter(double x, double y, string name, const string &category)
{
    ScatterPacket packet;
    packet.x = x;
    packet.y = y;
    packet.name = category;
    packet.legend = name;
    scatterRequest(packet);
}


}
