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

#include "logger.h"
#include <string>

namespace dbug {

struct Point {
    Point() {}
    Point(double x_, double y_): x(x_), y(y_) { }
    double x, y;
};

class ScatterPacket
{
public:
    ScatterPacket():name("general") {}
    Point point;
    std::string legend;
    std::string name; // category
};

class Scatter
{
protected:
    Logger logger;
    std::string category;

public:
    Scatter() {}

    virtual void addPacket(const ScatterPacket& packet) = 0;
    virtual void addData(float x, float y, std::string legend = "Unknown") = 0;
    virtual void setData(const std::vector<Point> &data, std::string legend = "Unknown") = 0;
    virtual void clearData() = 0;
    virtual void enableRecording(bool enable) {}
    virtual void setCategory(const std::string &category_);
    std::string getCategory() const;
};


}

#endif // _SCATTER_H
