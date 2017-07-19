#include "plotterwidget.h"
#include "ui_plotterwidget.h"

namespace sdbug {

PlotterWidget::PlotterWidget() :
    ui(new Ui::PlotterWidget)
{
    ui->setupUi(this);
    ui->statusFrame->hide();
    QGraphicsScene *statusScene = new QGraphicsScene(this);
    statusScene->setBackgroundBrush(Qt::yellow);
    ui->statusGraphicsView->setScene(statusScene);
    QPixmap pixmap(":/png-icons/design-collection/rotate-512.png");
    double w = pixmap.width(), h = pixmap.height();
    statusScene->setSceneRect(0, 0, w, h);
    ui->statusGraphicsView->scale(ui->statusGraphicsView->maximumWidth()/w, ui->statusGraphicsView->maximumWidth()/h);

    // connect slot that ties some axis selections together (especially opposite axes):
    connect(ui->qPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
    connect(ui->qPlot, &QCustomPlot::mousePress, this, &PlotterWidget::mousePress);
    connect(ui->qPlot, &QCustomPlot::mouseWheel, this, &PlotterWidget::mouseWheel);

    statusGItem = statusScene->addPixmap(pixmap);
    statusGItem->setOffset(-w/2, -h/2);
    statusGItem->setPos(w/2, h/2);

    // Add Drag, Zoom and ... capabilities
    ui->qPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems | QCP::iSelectPlottables | QCP::iSelectAxes | QCP::iMultiSelect);

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(ui->qPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->qPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->qPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->qPlot->yAxis2, SLOT(setRange(QCPRange)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->qPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

    // ************* Set Axis Settings ****************
    ui->qPlot->xAxis->setTickLength(0, 5);
    ui->qPlot->xAxis->setSubTickLength(0, 3);
    ui->qPlot->xAxis->setTickLabelRotation(30);

    ui->qPlot->yAxis->setRange(-1, 1);

    connected = true;
    freezed = false;
}

void PlotterWidget::setCategory(const string &category_)
{
    ui->minimizeButton->setToolTip(QString("Plot: %1").arg(category_.c_str()));
    Plotter::setCategory(category_);
}

void PlotterWidget::setLegendsFont(const QFont& font)
{
    ui->qPlot->legend->setVisible(true);
    ui->qPlot->legend->setFont(font);
    ui->qPlot->legend->setRowSpacing(-3);
}

void PlotterWidget::enableRecording(bool enable)
{
    ui->recButton->setChecked(enable);
}

void PlotterWidget::addPacket(const PlotterPacket &packet)
{
    if(!this->connected)
        return;

    double key = packet.key;
    if(key < 0)
        key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 ;

    // ************* Adding New Graph *****************
    for(unsigned int i=0; i<packet.legends.size(); i++)
    {
        QCPGraph *g1 = NULL, *g2 = NULL;
        for(int j=0; j<ui->qPlot->legend->itemCount(); j++)
        {
            if(ui->qPlot->graph(2*j)->name() == QString::fromStdString(packet.legends[i]))
            {
                g1 = ui->qPlot->graph(2*j);
                g2 = ui->qPlot->graph(2*j +1);
                break;
            }
        }

        if(!g1 && !g2)
        {
            g1 = ui->qPlot->addGraph();
            g2 = ui->qPlot->addGraph(); // Add a blue dot in end of graph
            ui->qPlot->legend->removeItem(ui->qPlot->legend->itemCount()-1); // don't show two graphs in legend

            QColor color_ = colorManager.getNewDifferentColor();

            g1->setName(QString::fromStdString(packet.legends[i]));
            g1->setPen(QPen(color_));

            g2->setPen(QPen(color_));
            g2->setLineStyle(QCPGraph::lsNone);
            g2->setScatterStyle(QCPScatterStyle::ssDisc);
            ui->qPlot->legend->setVisible(true);

            if(key > QDateTime::currentMSecsSinceEpoch()/2000) {
                QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
                dateTicker->setDateTimeFormat("mm:ss");
                ui->qPlot->xAxis->setTicker(dateTicker);
            }
            else {
                //ui->qPlot->xAxis->setNumberFormat("f");

                ui->qPlot->xAxis->ticker()->setTickCount(9);
                ui->qPlot->xAxis->setNumberFormat("ebc");
                ui->qPlot->xAxis->setNumberPrecision(1);

            }
        }        

        //! add data to lines:
        g1->addData(key, packet.values[i]);
        g2->setData(QVector<double>(1, key), QVector<double>(1, packet.values[i]));

        if(!freezed) {
            double upper_bound = qMax(packet.values[i], ui->qPlot->yAxis->range().upper);
            double lower_bound = qMin(packet.values[i], ui->qPlot->yAxis->range().lower);
            ui->qPlot->yAxis->setRangeUpper(upper_bound);
            ui->qPlot->yAxis->setRangeLower(lower_bound);
        }
    }

    if(!freezed)
        ui->qPlot->xAxis->setRange(key+ui->qPlot->xAxis->range().size()/4, ui->qPlot->xAxis->range().size(), Qt::AlignRight);

    if(realTimePlot)
        ui->qPlot->replot();
    else
        ui->qPlot->replot(QCustomPlot::rpQueuedReplot);


    if(ui->recButton->isChecked()) {
        logger.addLogCsv(key, packet.legends, packet.values);
    }

    if(statusGItem->isVisible())
        statusGItem->setRotation(statusGItem->rotation()-5);
}

void PlotterWidget::addValue(double val, double key, string legend)
{
    PlotterPacket packet(legend, val);
    packet.setKey(key);

    addPacket(packet);
}

void PlotterWidget::clearData(const string &legend)
{
    for(int i=ui->qPlot->graphCount()-1; i>=0; i--) {
        if(legend.empty()) {
            ui->qPlot->removeGraph(i);
        }
        else if(legend == ui->qPlot->graph(i)->name().toStdString()) {
            ui->qPlot->removeGraph(i);
            break;
        }
    }
    for(int i=ui->qPlot->lineGraphCount()-1; i>=0; i--) {
        if(legend.empty()) {
            ui->qPlot->removeLineGraph(i);
        }
        else if(legend == ui->qPlot->lineGraph(i)->name().toStdString()) {
            ui->qPlot->removeLineGraph(i);
            break;
        }
    }

    if(legend.empty())
    {
        int I = ui->qPlot->itemCount();
        for(int i=I-1; i>=0; i--)
        {
            ui->qPlot->removeItem(i);
        }
    }

    ui->qPlot->replot();
}

void PlotterWidget::reset()
{
    ui->qPlot->clearGraphs();
}

void PlotterWidget::forceToPause()
{
    if(!freezed)
        ui->pauseButton->click();
}
void PlotterWidget::forceToPlay()
{
    if(freezed)
        ui->pauseButton->click();
}

void PlotterWidget::setConnected(bool connected)
{
    this->connected = connected;
}

void PlotterWidget::setYAxisRange(double lower, double upper)
{
    ui->qPlot->yAxis->setRange(lower, upper);
}

PlotterWidget::~PlotterWidget()
{
    delete ui;
}

void PlotterWidget::selectionChanged()
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
    if (ui->qPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->qPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->qPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->qPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
    if (ui->qPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
            ui->qPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->qPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->qPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    // synchronize selection of graphs with selection of corresponding legend items:
    for (int i=0; i<ui->qPlot->graphCount(); ++i)
    {
        QCPGraph *graph = ui->qPlot->graph(i);
        QCPPlottableLegendItem *item = ui->qPlot->legend->itemWithPlottable(graph);
        if(item)
            item->setSelected(graph->selected());
    }
}

void PlotterWidget::mousePress()
{
    // if an axis is selected, only allow the direction of that axis to be dragged
    // if no axis is selected, both directions may be dragged
    if (ui->qPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->qPlot->axisRect()->setRangeDrag(ui->qPlot->xAxis->orientation());
    else if (ui->qPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->qPlot->axisRect()->setRangeDrag(ui->qPlot->yAxis->orientation());
    else
        ui->qPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void PlotterWidget::mouseWheel()
{
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed
    if (ui->qPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->qPlot->axisRect()->setRangeZoom(ui->qPlot->xAxis->orientation());
    else if (ui->qPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->qPlot->axisRect()->setRangeZoom(ui->qPlot->yAxis->orientation());
    else
        ui->qPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void PlotterWidget::graphClicked(QCPAbstractPlottable *plottable)
{
    //    ui->vel_label->setText(plottable->name());
}


void PlotterWidget::on_closeButton_clicked()
{
    emit closeMe(QString::fromStdString(this->category));
}

void PlotterWidget::on_minimizeButton_clicked(bool checked)
{
    if(checked) {
        ui->qPlot->hide();
        ui->toolBoxframe->hide();
        ui->statusFrame->show();

        this->layout()->invalidate();
        ui->leftLayout->invalidate();
        ui->verticalSpacer->changeSize(0, 0, QSizePolicy::Preferred, QSizePolicy::Preferred);
        this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    }
    else {
        ui->qPlot->show();
        ui->toolBoxframe->show();
        ui->statusFrame->hide();

        this->layout()->invalidate();
        ui->leftLayout->invalidate();
        ui->verticalSpacer->changeSize(0, 0, QSizePolicy::Preferred, QSizePolicy::Expanding);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
}

void PlotterWidget::on_pauseButton_clicked(bool checked)
{
    freezed = checked;
}

void PlotterWidget::on_recButton_clicked(bool checked)
{
    //if(checked)
}
}
