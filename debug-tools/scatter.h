//========================================================================
/*!
  @file    scatter.h
  @class   Scatter
  @date    10/24/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================
#ifndef _SCATTER_H
#define _SCATTER_H

#include <string>
#include "plottable.h"

namespace sdbug {

struct PointD {
    PointD() {}
    PointD(double x_, double y_): x(x_), y(y_) { }
    double x, y;
};

class ScatterPacket
{
public:
    ScatterPacket():name("general") {}
    PointD point;
    std::string legend;
    std::string name; // category
};

class Scatter : public Plottable
{
public:
    Scatter() {}

    virtual void addPacket(const ScatterPacket& packet) {};
    virtual void addData(float x, float y, std::string legend = "Unknown") {};
    virtual void setData(const std::vector<PointD> &data, std::string legend = "Unknown") {};
    virtual void clearData(const std::string &legend = "") {};
    virtual void addBaseLine(const PointD &p1, const PointD &p2, std::string legend = "Unknown Line") {};
    virtual void addLineSegment(const PointD &p1, const PointD &p2, std::string legend = "") {};
    virtual void addText(const std::string & text, const PointD &origin, int q_color = 2) {};

};


}

#endif // _SCATTER_H
