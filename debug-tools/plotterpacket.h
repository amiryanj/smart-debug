//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#ifndef _PLOTTERPACKET_H
#define _PLOTTERPACKET_H

#include <vector>
#include <string>
#include <cstdio>

namespace dbug {

using namespace std;

class PlotterPacket
{
public:
    PlotterPacket();
    PlotterPacket(const string &name_, double value_);
    PlotterPacket(const string &name_, const vector<double> values_, vector<string> legends_);
    void setKey(double key_);
    void append(const string &name_, double value_);

    string toJson() const;

    string name;
    double key;  //negative values are considered invalid
    vector<double> values;
    vector<string> legends;
};
}

#endif // PLOTTERPACKET_H
