#ifndef _DEBUGFACTORY_H
#define _DEBUGFACTORY_H

#include "debugger.h"
#include "plotterwidget.h"
#include "scatterwidget.h"
#include <map>

namespace dbug {

    Debugger* dbuger();

    class DebugFactory
    {
    public:
        enum DebuggerStyle{eBuiltInDebugger, eNetworkDebugger};

        static Debugger* createDebugger(DebuggerStyle style = eBuiltInDebugger, QWidget *parent = 0);
        static Debugger* debugger();
        static DebuggerStyle getDebuggerStyle();
        static void clearDebugger();

    protected:
        static DebuggerStyle sDebuggerStyle;
        static Debugger*     sDebugger;
        static std::map<std::string, PlotterWidget*> plotterMap;
        static std::map<std::string, ScatterWidget*> scatterMap;
    };

}

#endif // DEBUGFACTORY_H
