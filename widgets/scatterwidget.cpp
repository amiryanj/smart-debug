//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#include "scatterwidget.h"
#include "ui_scatterwidget.h"

using namespace std;

namespace dbug
{

ScatterWidget::ScatterWidget() :
    ui(new Ui::ScatterWidget)
{
    ui->setupUi(this);    

    ui->scatter->xAxis->setTickStep(2);
    ui->scatter->axisRect()->setupFullAxesBox();
    ui->scatter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->scatter->xAxis->setRange(-1, 1);
    ui->scatter->yAxis->setRange(-1, 1);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->scatter->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->scatter->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->scatter->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->scatter->yAxis2, SLOT(setRange(QCPRange)));

    // Add Drag, Zoom and ... capabilities
    ui->scatter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems);
}

ScatterWidget::~ScatterWidget()
{
    delete ui;
}

void ScatterWidget::addPacket(const ScatterPacket &packet)
{
    QCPGraph *graph = NULL;
    for(int j=0; j<ui->scatter->legend->itemCount(); j++)
    {
        if(ui->scatter->graph(j)->name() == QString::fromStdString(packet.legend))
        {
            graph = ui->scatter->graph(j);
            break;
        }
    }

    if(!graph)
    {
        graph = ui->scatter->addGraph();
        // ----------------------- Scatter Configuration ---------------------------
        graph->setName(QString::fromStdString(packet.legend));
        graph->setPen(QPen((Qt::GlobalColor)(ui->scatter->graphCount()%13+6)));
        graph->setLineStyle(QCPGraph::lsNone);
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, 4));
        ui->scatter->legend->setVisible(true);
    }

    graph->addData(packet.point.x, packet.point.y);
    ui->scatter->replot();

    if(ui->recButton->isChecked())
    {
        vector<double> vec2_double(2);
        vec2_double[0] = packet.point.x;
        vec2_double[1] = packet.point.y;
        vector<string> vec2_string(2);
        vec2_string[0] = packet.legend + "_x";
        vec2_string[1] = packet.legend + "_y";
        logger.addLogCsv(graph->data()->count(), vec2_string, vec2_double);
    }
}

void ScatterWidget::addData(float x, float y, string legend)
{
    addData(Point(x, y), legend);
}

void ScatterWidget::addData(const Point &point, string legend)
{
    ScatterPacket packet;
    packet.point = point;
    packet.legend = legend;
    addPacket(packet);
}

void ScatterWidget::setData(const std::vector<Point> &data, string legend)
{
    ScatterPacket packet;
    packet.legend = legend;
    for(unsigned int i=0; i<data.size(); i++) {
        packet.point = data[i];
        addPacket(packet);
    }
}

void ScatterWidget::clearData()
{
    ui->scatter->graph(0)->clearData();
    ui->scatter->graph(1)->clearData();
    ui->scatter->replot();
}

void ScatterWidget::mouseWheel()
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
