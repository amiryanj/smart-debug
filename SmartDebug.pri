QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

INCLUDEPATH += $$PWD \
    $$PWD/qcustomplot \
    $$PWD/debug-tools \
    $$PWD/widgets

SOURCES += \
    $$PWD/qcustomplot/qcustomplot.cpp \
    $$PWD/debug-tools/builtindebugger.cpp \
    $$PWD/debug-tools/debugger.cpp \
    $$PWD/debug-tools/logger.cpp \
    $$PWD/debug-tools/plotterpacket.cpp \
    $$PWD/debug-tools/debugfactory.cpp \
#    $$PWD/debug-tools/networkplotter.cpp \
    $$PWD/widgets/plotwidget.cpp \
    $$PWD/widgets/scatterplotwidget.cpp \
    $$PWD/widgets/widgetsmanager.cpp \
    $$PWD/debug-tools/scatterpacket.cpp

HEADERS += \
    $$PWD/qcustomplot/qcustomplot.h \
    $$PWD/debug-tools/builtindebugger.h \
    $$PWD/debug-tools/debugger.h \
    $$PWD/debug-tools/logger.h \
    $$PWD/debug-tools/plotterpacket.h \
    $$PWD/debug-tools/debugfactory.h \
#    $$PWD/debug-tools/networkplotter.h \
    $$PWD/widgets/plotwidget.h \
    $$PWD/widgets/scatterplotwidget.h \
    $$PWD/widgets/widgetsmanager.h \
    $$PWD/debug-tools/scatterpacket.h

FORMS += \
    $$PWD/widgets/plotwidget.ui \
    $$PWD/widgets/widgetsmanager.ui \
    $$PWD/widgets/scatterplotwidget.ui


RESOURCES += \
    $$PWD/resources/icons.qrc




