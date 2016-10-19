//========================================================================
/*!
  @file    scatterpacket.h
  @class   ScatterPacket
  @date    10/2/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================
#ifndef _SCATTERPACKET_H
#define _SCATTERPACKET_H

#include <string>

class ScatterPacket
{
public:
    ScatterPacket() {}
    double x, y;
    std::string legend;
    std::string name; // category
};

#endif // _SCATTERPACKET_H
