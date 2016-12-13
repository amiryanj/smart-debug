#include "debugfactory.h"
#include "builtindebugger.h"
#include <iostream>

using namespace std;
namespace sdbug {

// initialize static members
DebugFactory::DebuggerStyle DebugFactory::sDebuggerStyle = DebugFactory::DebuggerStyle::eBuiltInDebugger;
Debugger*     DebugFactory::sDebugger = NULL;

std::map<std::string, PlotterWidget*> DebugFactory::plotterMap;
std::map<std::string, ScatterWidget*> DebugFactory::scatterMap;

Debugger *DebugFactory::createDebugger(DebugFactory::DebuggerStyle style, QWidget *parent)
{
    if(sDebugger)
        delete sDebugger;
    sDebugger = NULL;

    sDebuggerStyle = style;
    switch (sDebuggerStyle) {
    case DebuggerStyle::eBuiltInDebugger:
#ifdef ACTIVE_SMART_DEBUG
        sDebugger = new BuiltInDebugger(parent);
#else
        sDebugger = new Debugger();
#endif
        break;

    case DebuggerStyle::eNetworkDebugger:
    default:
        cerr << "DebugFactory can not create new object. Invalid style!" << endl;
        break;
    }
    return sDebugger;
}

Debugger *DebugFactory::debugger()
{
    if(sDebugger == NULL)
        createDebugger();
    return sDebugger;
}

DebugFactory::DebuggerStyle DebugFactory::getDebuggerStyle() {
    return sDebuggerStyle;
}

void DebugFactory::clearDebugger()
{
    cout << "deleting debugger in debug factory destructor" << endl;

    for(auto itr = plotterMap.begin(); itr != plotterMap.end(); itr++) {
        delete itr->second;
    }
    plotterMap.clear();

    for(auto itr = scatterMap.begin(); itr != scatterMap.end(); itr++) {
        delete itr->second;
    }
    scatterMap.clear();

    if(sDebugger)
        delete sDebugger;
    sDebugger = NULL;
}

Debugger *dbuger() {
    return DebugFactory::debugger();
}

}
