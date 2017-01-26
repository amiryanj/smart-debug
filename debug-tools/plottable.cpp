//========================================================================
/*!
  @file    plottable.cpp
  @class   Plottable
  @date    1/26/2017
  @brief
  @author  Javad Amiryan, (C) 2017
*/
//========================================================================
#include "plottable.h"


namespace sdbug {
Plottable::Plottable()
{

}

void Plottable::setCategory(const std::string &category_)
{
    this->category = category_;
    logger.setFileName(category_ + ".csv");
}

std::string Plottable::getCategory() const
{
    return category;
}

void Plottable::enableRealTimePlotting(bool val)
{
    realTimePlot = val;
}

}
