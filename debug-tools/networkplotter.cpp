#include "networkplotter.h"
#include "../paramater-manager/parametermanager.h"

using namespace std;
NetworkPlotter* NetworkPlotter::instance = NULL;
vector<Plotter_Packet> NetworkPlotter::packets;
QMutex NetworkPlotter::mtx_;
QUdpSocket NetworkPlotter::socket;
int NetworkPlotter::port;
string NetworkPlotter::address;

NetworkPlotter::NetworkPlotter()
{
    ParameterManager* pm = ParameterManager::getInstance();
    port = pm->get<int>("network.plotter_port");
    address = pm->get<string>("network.plotter_address");
    openSocket(port, address);

    QtConcurrent::run(sendPackets, (void *)NULL);
}

NetworkPlotter *NetworkPlotter::getInstance()
{
    if(instance == NULL) {
        instance = new NetworkPlotter();
    }
    return instance;
}

bool NetworkPlotter::buildAndSendPacket(string plot_name, double value, double key)
{
    if( ParameterManager::getInstance()->get<bool>("network.plotter_enabled") == false)
        return false;
    Plotter_Packet packet;

    packet.set_name(plot_name);
    packet.add_values(value);
    packet.add_legends(plot_name);

    if(key >= 0)
        packet.set_key(key);

    this->appendQueue(packet);
    //    this->sendPacket(packet);
}

bool NetworkPlotter::appendQueue(Plotter_Packet &packet)
{
    mtx_.lock();
    packets.push_back(packet);
    mtx_.unlock();
}

bool NetworkPlotter::buildAndSendPacket(string plot_name, vector<double> values, vector<string> legends, double key)
{
    if( ParameterManager::getInstance()->get<bool>("network.plotter_enabled") == false)
        return false;
    Plotter_Packet packet;
    //            required string name = 1;
    //            repeated double values = 2;
    //            optional double key  = 3;
    //            repeated string legends = 4;
    //            required bool clear_history = 5;
    //            optional string sender_name = 6;

    packet.set_name(plot_name);
    for(int i=0; i<values.size(); i++)  {
        packet.add_values(values[i]);
    }
    for(int i=0; i<values.size(); i++)  {
        packet.add_legends(legends[i]);
    }
    if(key >= 0)
        packet.set_key(key);
    this->appendQueue(packet);

//    this->sendPackets(packet);
//    cout << "Vector test sent" << endl;
}


bool NetworkPlotter::openSocket(int port, string address)
{
//    socket.close();
    socket.open(QIODevice::WriteOnly);
#if QT_VERSION >= 0x050000
    socket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
#else
    socket.bind(QHostAddress::Any, port, QUdpSocket::ShareAddress);
#endif
    socket.joinMulticastGroup(QHostAddress(QString(address.c_str())));
//    socket.setSocketOption(QAbstractSocket::MulticastTtlOption, 3);
    return true;
}

void *NetworkPlotter::sendPackets(void*)
{
    while(true) {

        mtx_.lock();
        if(packets.empty()) {
//            cout << "no packet..." << endl;
            mtx_.unlock();
            continue;
        }
        Plotter_Packet &pp = packets[0];
        Plotter_Packet packet(pp);
        packets.erase(packets.begin());
        mtx_.unlock();
        string buffer;
        packet.set_sender_name("Cyrus Server");
        packet.SerializeToString(&buffer);
        int sent_size = socket.writeDatagram(buffer.c_str(), buffer.length(),
                                             QHostAddress(QString(address.c_str())), port);
        cout << "packet sent ... << " << sent_size << endl;
    }
}

