//========================================================================
/*!
  @file    plottable.h
  @class   Plottable
  @date    1/26/2017
  @brief
  @author  Javad Amiryan, (C) 2017
*/
//========================================================================
#ifndef _PLOTTABLE_H
#define _PLOTTABLE_H

#include "logger.h"

namespace sdbug {

class Plottable
{    
public:
    Plottable();

    virtual void enableRecording(bool enable) {};
    virtual void setCategory(const std::string &category_);
    std::string getCategory() const;
    void enableRealTimePlotting(bool val);

protected:
    Logger logger;
    std::string category;
    bool realTimePlot = false;

};

}

#endif // PLOTTABLE_H
