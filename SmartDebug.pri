QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

#DEFINES += ACTIVE_SMART_DEBUG

INCLUDEPATH += $$PWD \
    $$PWD/qcustomplot \
    $$PWD/debug-tools \
    $$PWD/widgets

SOURCES += \
    $$PWD/qcustomplot/qcustomplot.cpp \
    $$PWD/debug-tools/builtindebugger.cpp \
    $$PWD/debug-tools/debugger.cpp \
    $$PWD/debug-tools/logger.cpp \
    $$PWD/debug-tools/debugfactory.cpp \
#    $$PWD/debug-tools/networkplotter.cpp \
    $$PWD/widgets/scatterwidget.cpp \
    $$PWD/widgets/builtinwidgetmanager.cpp \
    $$PWD/debug-tools/widgetmanger.cpp \
    $$PWD/debug-tools/plotter.cpp \
    $$PWD/debug-tools/scatter.cpp \
    $$PWD/widgets/plotterwidget.cpp \
    $$PWD/debug-tools/plottable.cpp

HEADERS += \
    $$PWD/qcustomplot/qcustomplot.h \
    $$PWD/debug-tools/builtindebugger.h \
    $$PWD/debug-tools/debugger.h \
    $$PWD/debug-tools/logger.h \
    $$PWD/debug-tools/debugfactory.h \
#    $$PWD/debug-tools/networkplotter.h \
    $$PWD/widgets/scatterwidget.h \
    $$PWD/widgets/builtinwidgetmanager.h \
    $$PWD/debug-tools/widgetmanger.h \
    $$PWD/debug-tools/plotter.h \
    $$PWD/debug-tools/scatter.h \
    $$PWD/widgets/plotterwidget.h \
    $$PWD/debug-tools/plottable.h

FORMS += \
    $$PWD/widgets/builtinwidgetmanager.ui \
    $$PWD/widgets/scatterwidget.ui \
    $$PWD/widgets/plotterwidget.ui


RESOURCES += \
    $$PWD/resources/icons.qrc




