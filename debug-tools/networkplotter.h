#ifndef _NETWORKPLOTTER_H
#define _NETWORKPLOTTER_H

#include <boost/signals2/mutex.hpp>
#include <vector>
#include <queue>
#include <QMutex>
#include <QUdpSocket>
#if QT_VERSION >= 0x050000
#include <QtConcurrent>
#else
#include <QtCore>
#endif

class NetworkPlotter
{
    NetworkPlotter();
    static NetworkPlotter* instance;
public:
    static NetworkPlotter* getInstance();

    bool buildAndSendPacket(string plot_name,
                            vector<double> values, vector<string> legends, double key = -1);

    bool buildAndSendPacket(string plot_name, double value, double key = -1);
    bool appendQueue(Plotter_Packet &packet);

    static void * sendPackets(void *);

private:
    bool openSocket(int port, string address);

    static vector<Plotter_Packet> packets;
    static QMutex mtx_;
    static QUdpSocket socket;

    static int port;
    static string address;
};

#endif // NETWORKPLOTTER_H
