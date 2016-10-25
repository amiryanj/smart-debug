//========================================================================
/*!
  @file    scatter.cpp
  @class   Scatter
  @date    10/24/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================
#include "scatter.h"

namespace dbug
{
std::string Scatter::getCategory() const
{
    return category;
}

void Scatter::setCategory(const std::string &category_)
{
    logger.setFileName(category_ + ".csv");
    category = category_;
}

}
