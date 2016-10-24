//========================================================================
/*!
  @file    plotter.cpp
  @class   Plotter
  @date    10/24/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================
#include "plotter.h"

namespace dbug {

PlotterPacket::PlotterPacket()
{
    setKey(-1);
}

PlotterPacket::PlotterPacket(const string &name_, double value_)
{
    setKey(-1);
    name = name_;
    values.push_back(value_);
    legends.push_back(name_);
}

PlotterPacket::PlotterPacket(const string &name_, const vector<double> values_, vector<string> legends_)
{
    setKey(-1);
    name = name_;
    values = values_;
    legends = legends_;
}

void PlotterPacket::setKey(double key_)
{
    key = key_;
}

void PlotterPacket::append(const string &name_, double value_)
{
    values.push_back(value_);
    legends.push_back(name_);
}

string PlotterPacket::toJson() const
{
    string str;
    char buffer_i[255];
    std::sprintf(buffer_i, "{\"type\": \"plot packet\", \"category\": \"%s\", \"data\": {", name.c_str());
    str.append(buffer_i);
    for(unsigned int j=0; j<legends.size(); j++)
    {
        char buffer_j[255];
        sprintf(buffer_j, "\"%s\" : %3f", legends[j].c_str(), values[j]);
        str.append(buffer_j);
        if(j<legends.size()-1)
            str.append(", ");
    }
    str.append("} }");
    return str;
}

string PlotterPacket::toCSV() const
{
    string str;
    str.append(to_string(key) + ",");
    for(unsigned int i=0; i<legends.size(); i++) {
        str.push_back(values[i]);
        if(i<legends.size()-1)
            str.append(", ");
        else
            str.append("\n");
    }
}

Plotter::Plotter()
{

}


}

