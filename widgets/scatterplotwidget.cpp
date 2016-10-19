//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#include "scatterplotwidget.h"
#include "ui_scatterplotwidget.h"
namespace dbug {

ScatterPlotWidget::ScatterPlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScatterPlotWidget)
{
    ui->setupUi(this);

    // ----------------------- Scatter Configuration ---------------------------
    ui->scatter->addGraph(); // blue circle
    ui->scatter->graph(0)->setPen(QPen(Qt::blue));
    ui->scatter->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->scatter->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, 4));

    ui->scatter->addGraph(); // blue line
    ui->scatter->graph(1)->setPen(QPen(Qt::red));
    ui->scatter->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->scatter->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, 5));

    ui->scatter->addGraph(); // blue line
    ui->scatter->graph(2)->setPen(QPen(Qt::green));
    ui->scatter->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->scatter->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 30));


    ui->scatter->xAxis->setTickStep(2);
    ui->scatter->axisRect()->setupFullAxesBox();
    ui->scatter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->scatter->xAxis->setRange(-100, 100);
    ui->scatter->yAxis->setRange(-100, 100);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->scatter->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->scatter->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->scatter->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->scatter->yAxis2, SLOT(setRange(QCPRange)));

    // Add Drag, Zoom and ... capabilities
    ui->scatter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems);
}

ScatterPlotWidget::~ScatterPlotWidget()
{
    delete ui;
}

void ScatterPlotWidget::setData(const QVector<QPoint> &data)
{
    QVector<double> x;
    QVector<double> y;
    foreach (QPoint point, data) {
        x.append(point.x());
        y.append(point.y());
    }
    ui->scatter->graph(0)->setData(x, y);

    ui->scatter->replot();
}

void ScatterPlotWidget::addData(const QPointF &p)
{
    ui->scatter->graph(0)->addData(p.x(), p.y());;

    QVector<double> x;
    QVector<double> y;
    x.append(p.x());
    y.append(p.y());
    ui->scatter->graph(1)->setData(x, y);
    ui->scatter->replot();
}

void ScatterPlotWidget::clearData()
{
    ui->scatter->graph(0)->clearData();
    ui->scatter->graph(1)->clearData();
    ui->scatter->replot();
}

void ScatterPlotWidget::mouseWheel()
{
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed

    if (ui->scatter->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->scatter->axisRect()->setRangeZoom(ui->scatter->xAxis->orientation());
    else if (ui->scatter->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->scatter->axisRect()->setRangeZoom(ui->scatter->yAxis->orientation());
    else
        ui->scatter->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}
}
