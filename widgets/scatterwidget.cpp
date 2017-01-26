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
#include <QDateTime>
#include <QFileDialog>

using namespace std;

namespace sdbug
{

ScatterWidget::ScatterWidget() :
    ui(new Ui::ScatterWidget)
{
    ui->setupUi(this);

    ui->scatter->xAxis->setTickLength(0, 5);
    ui->scatter->xAxis->setSubTickLength(0, 3);
    ui->scatter->axisRect()->setupFullAxesBox();
    ui->scatter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->scatter->xAxis->setRange(-1, 1);
    ui->scatter->yAxis->setRange(-1, 1);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->scatter->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->scatter->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->scatter->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->scatter->yAxis2, SLOT(setRange(QCPRange)));

    // connect slot that ties some axis selections together (especially opposite axes):
    connect(ui->scatter, &QCustomPlot::selectionChangedByUser, this, &ScatterWidget::selectionChanged);
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->scatter, &QCustomPlot::mousePress, this, &ScatterWidget::mousePress);
    connect(ui->scatter, &QCustomPlot::mouseWheel, this, &ScatterWidget::mouseWheel);

    // Add Drag, Zoom and ... capabilities
    ui->scatter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems | QCP::iSelectPlottables | QCP::iSelectAxes | QCP::iMultiSelect);
}

ScatterWidget::~ScatterWidget()
{
    delete ui;
}

void ScatterWidget::addPacket(const ScatterPacket &packet)
{
    QCPGraph *graph = NULL;
    for(int i=0; i<ui->scatter->legend->itemCount(); i++)
    {
        if(ui->scatter->graph(i)->name() == QString::fromStdString(packet.legend))
        {
            graph = ui->scatter->graph(i);
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
    //if(packet.point.x > ui->scatter->xAxis->max)

    double max_x = qMax(packet.point.x, ui->scatter->xAxis->range().upper);
    double min_x = qMin(packet.point.x, ui->scatter->xAxis->range().lower);
    double max_y = qMax(packet.point.y, ui->scatter->yAxis->range().upper);
    double min_y = qMin(packet.point.y, ui->scatter->yAxis->range().lower);

    ui->scatter->xAxis->setRange(min_x, max_x);
    ui->scatter->yAxis->setRange(min_y, max_y);

    if(realTimePlot)
        ui->scatter->replot();
    else
        ui->scatter->replot(QCustomPlot::rpQueuedReplot);

    if(ui->recButton->isChecked())
    {
        vector<double> vec2_double(2);
        vec2_double[0] = packet.point.x;
        vec2_double[1] = packet.point.y;
        vector<string> vec2_string(2);
        vec2_string[0] = packet.legend + "_x";
        vec2_string[1] = packet.legend + "_y";
        logger.addLogCsv(graph->dataCount(), vec2_string, vec2_double);
    }
}

void ScatterWidget::addData(float x, float y, string legend)
{
    addData(PointD(x, y), legend);
}

void ScatterWidget::addData(const PointD &point, string legend)
{
    ScatterPacket packet;
    packet.point = point;
    packet.legend = legend;
    addPacket(packet);
}

void ScatterWidget::setData(const std::vector<PointD> &data, string legend)
{
    QCPGraph *graph = NULL;
    for(int i=0; i<ui->scatter->legend->itemCount(); i++)
    {
        if(ui->scatter->graph(i)->name() == QString::fromStdString(legend))
        {
            graph = ui->scatter->graph(i);
            break;
        }
    }

    if(!graph)
    {
        graph = ui->scatter->addGraph();
        // ----------------------- Scatter Configuration ---------------------------
        graph->setName(QString::fromStdString(legend));
        graph->setPen(QPen((Qt::GlobalColor)(ui->scatter->graphCount()%13+6)));
        graph->setLineStyle(QCPGraph::lsNone);
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, 4));
        ui->scatter->legend->setVisible(true);
    }

    double min_x =  INFINITY, min_y =  INFINITY;
    double max_x = -INFINITY, max_y = -INFINITY;

    QVector<QCPGraphData> q_data(data.size());
    for(unsigned int i=0; i<data.size(); i++)
    {
        q_data[i] = QCPGraphData(data[i].x, data[i].y);

        if(ui->recButton->isChecked())
        {
            vector<double> vec2_double(2);
            vec2_double[0] = data[i].x;
            vec2_double[1] = data[i].y;
            vector<string> vec2_string(2);
            vec2_string[0] = legend + "_x";
            vec2_string[1] = legend + "_y";
            logger.addLogCsv(graph->dataCount(), vec2_string, vec2_double);
        }

        max_x = qMax(data[i].x, ui->scatter->xAxis->range().upper);
        min_x = qMin(data[i].x, ui->scatter->xAxis->range().lower);
        max_y = qMax(data[i].y, ui->scatter->yAxis->range().upper);
        min_y = qMin(data[i].y, ui->scatter->yAxis->range().lower);
    }

    graph->data()->set(q_data);
    ui->scatter->xAxis->setRange(min_x, max_x);
    ui->scatter->yAxis->setRange(min_y, max_y);
    ui->scatter->replot();
}

void ScatterWidget::clearData(string legend)
{
    for(int i=0; i<ui->scatter->graphCount(); i++) {
        if(legend.empty())
            ui->scatter->graph(i)->data()->clear();
        else if(legend == ui->scatter->graph(i)->name().toStdString()) {
            ui->scatter->graph(i)->data()->clear();
            break;
        }
    }
    ui->scatter->replot();
}

void ScatterWidget::addBaseLine(const PointD &p1, const PointD &p2, string legend)
{
    QCPGraph *graph = NULL;
    for(int j=0; j<ui->scatter->legend->itemCount(); j++)
    {
        if(ui->scatter->graph(j)->name() == QString::fromStdString(legend))
        {
            graph = ui->scatter->graph(j);
            break;
        }
    }

    if(!graph)
    {
        graph = ui->scatter->addGraph();
        // ----------------------- Scatter Configuration ---------------------------
        graph->setName(QString::fromStdString(legend));
        graph->setPen(QPen((Qt::GlobalColor)(ui->scatter->graphCount()%13+6)));
    }
    QVector<double> keys, vals;
    keys << p1.x << p2.x;
    vals << p1.y << p2.y;
    graph->setData(keys, vals);
}

void ScatterWidget::enableRecording(bool enable)
{
    if(ui->recButton->isChecked() != enable)
        ui->recButton->click();
}

void ScatterWidget::selectionChanged()
{
    /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
   */

    // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (ui->scatter->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->scatter->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->scatter->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->scatter->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->scatter->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->scatter->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (ui->scatter->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->scatter->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->scatter->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->scatter->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->scatter->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->scatter->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    // synchronize selection of graphs with selection of corresponding legend items:
    for (int i=0; i<ui->scatter->graphCount(); ++i)
    {
        QCPGraph *graph = ui->scatter->graph(i);
        QCPPlottableLegendItem *item = ui->scatter->legend->itemWithPlottable(graph);
        if (graph->selected())
        {
            item->setSelected(true);
            //graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
        else
            item->setSelected(false);
    }
}

void ScatterWidget::mousePress()
{
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged
    if (ui->scatter->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->scatter->axisRect()->setRangeDrag(ui->scatter->xAxis->orientation());
    else if (ui->scatter->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->scatter->axisRect()->setRangeDrag(ui->scatter->yAxis->orientation());
    else
        ui->scatter->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
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

void ScatterWidget::zoomToAllData()
{
//    double max_x = qMax(packet.point.x, ui->scatter->xAxis->range().upper);
//    double min_x = qMin(packet.point.x, ui->scatter->xAxis->range().lower);
//    double max_y = qMax(packet.point.y, ui->scatter->yAxis->range().upper);
//    double min_y = qMin(packet.point.y, ui->scatter->yAxis->range().lower);

//    ui->scatter->xAxis->setRange(min_x, max_x);
//    ui->scatter->yAxis->setRange(min_y, max_y);
}
void ScatterWidget::on_eraseButton_clicked()
{
    this->clearData();
}

void ScatterWidget::on_snapButton_clicked()
{
    QPixmap pixmap(ui->scatter->size());
    ui->scatter->render(&pixmap);

    QFileDialog dialog(this);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList formats;
    formats << "pdf" << "bmp" << "png" << "jpg" ;
    dialog.setNameFilters(formats);

    QString default_name = QDateTime::currentDateTime().toString("yyyy-MMM-dd_HH:mm:ss");
    dialog.selectFile(default_name);
    //QString format = "png";
    //dialog.setDirectory(QDir::homePath());

    if(dialog.exec() == QFileDialog::Accepted)
    {
        QStringList file_names  = dialog.selectedFiles();
        QString selected_format = dialog.selectedNameFilter();
        if(!file_names.empty())
        {
            if (selected_format == "pdf")
                ui->scatter->savePdf(file_names[0] + "." + selected_format, 0, 0);
            else if(selected_format == "bmp")
                ui->scatter->saveBmp(file_names[0] + "." + selected_format, 0, 0, 1., -1);
            else if(selected_format == "png")
                ui->scatter->savePng(file_names[0] + "." + selected_format, 0, 0, 1., -1);
            else if(selected_format == "jpg")
                ui->scatter->saveJpg(file_names[0] + "." + selected_format, 0, 0, 1., -1);
        }
    }
    
}

void ScatterWidget::on_zoomButton_clicked()
{

}

}




