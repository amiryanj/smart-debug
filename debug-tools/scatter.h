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

namespace dbug {

class ScatterPacket
{
public:
    ScatterPacket() {}
    double x, y;
    std::string legend;
    std::string name; // category
};

class Scatter
{
public:
    Scatter();

    virtual void addData(float x, float y) = 0;
    virtual void addPacket(const ScatterPacket& packet) = 0;
    //virtual void addData(const QPointF &p);
    //virtual void setData(const vector<QPoint> &data);
    virtual void clearData() = 0;
    //virtual void setName(const std::string &name) = 0;
};


}

#endif // _SCATTER_H
