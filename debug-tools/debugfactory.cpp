#include "debugfactory.h"
#include "builtindebugger.h"
#include <iostream>

using namespace std;
namespace dbug {

// initialize static members
DebugFactory::DebuggerStyle DebugFactory::sDebuggerStyle = DebugFactory::DebuggerStyle::eBuiltInDebugger;
Debugger*     DebugFactory::sDebugger = NULL;

Debugger *DebugFactory::createDebugger(DebugFactory::DebuggerStyle style, QWidget *parent)
{
    if(sDebugger)
        delete sDebugger;
    sDebugger = NULL;

    sDebuggerStyle = style;
    switch (sDebuggerStyle) {
    case DebuggerStyle::eBuiltInDebugger:
         sDebugger = new BuiltInDebug(parent);
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
    if(sDebugger)
        delete sDebugger;
    sDebugger = NULL;

}

Debugger *dbuger() {
    return DebugFactory::debugger();
}

}
