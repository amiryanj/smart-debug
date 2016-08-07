#ifndef DEBUGFACTORY_H
#define DEBUGFACTORY_H

#include "debugger.h"
//#include "networkplotter.h"

namespace dbug {

    Debugger* dbuger();

    class DebugFactory
    {
    public:

        enum DebuggerStyle{eBuiltInDebugger, eNetworkDebugger};

    protected:
        static DebuggerStyle sDebuggerStyle;
        static Debugger*     sDebugger;

    public:
        static Debugger* createDebugger(DebuggerStyle style = eBuiltInDebugger, QWidget *parent = 0);
        static Debugger* debugger();
        static DebuggerStyle getDebuggerStyle();

        static void clearDebugger();
    };

}

#endif // DEBUGFACTORY_H
