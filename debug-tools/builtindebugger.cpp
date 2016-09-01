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
    plot_manager = new WidgetsManager(parent);
    plot_manager->show();
    connect(this, &BuiltInDebug::plotRequest, plot_manager, &WidgetsManager::plot);
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


}
