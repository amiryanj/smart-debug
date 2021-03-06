//========================================================================
/*!
  @file    plotter.h
  @class   Plotter
  @date    10/24/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================
#ifndef _PLOTTER_H
#define _PLOTTER_H

#include <vector>
#include <string>
#include <cstdio>
#include "plottable.h"

namespace sdbug {

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
    string toCSV() const;

    string name;
    double key;  //negative values are considered invalid
    vector<double> values;
    vector<string> legends;
};

class Plotter : public Plottable
{
public:
    Plotter() {}

    virtual void addValue(double val, double key = -1, std::string legend = ""){};
    virtual void addPacket(const PlotterPacket& /*packet*/) {};
    virtual void clearData(const string &legend = "") {}
    virtual void reset() {};

};

}

#endif // PLOTTER_H
