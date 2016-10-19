#include "plotwidget.h"
#include "ui_plotwidget.h"

namespace dbug {

PlotWidget::PlotWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotWidget)
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

    statusGItem = statusScene->addPixmap(pixmap);
    statusGItem->setOffset(-w/2, -h/2);
    statusGItem->setPos(w/2, h/2);


    // Add Drag, Zoom and ... capabilities
    ui->qPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectItems);

    // make bottom and left axes transfer their ranges to top and right axes:
    connect(ui->qPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->qPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->qPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->qPlot->yAxis2, SLOT(setRange(QCPRange)));

    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->qPlot, SIGNAL(plottableClick(QCPAbstractPlottable*,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*)));

    // ************* Set Axis Settings ****************
    ui->qPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    //ui->qPlot->xAxis->setDateTimeFormat("mm:ss");
    ui->qPlot->xAxis->setAutoTickStep(true);
    ui->qPlot->xAxis->setTickStep(1);
    ui->qPlot->xAxis->setTickLabelRotation(30);

    ui->qPlot->yAxis->setRange(-1, 1);

    connected = true;
    freezed = false;
}

void PlotWidget::setName(QString name)
{
    this->plotName = name;
    ui->minimizeButton->setToolTip("Plot: " + name);
}

void PlotWidget::setLegendsFont(const QFont& font)
{
    ui->qPlot->legend->setVisible(true);
    ui->qPlot->legend->setFont(font);
    ui->qPlot->legend->setRowSpacing(-3);

}

void PlotWidget::addValue(double key, const QVector<double> &vals, const QVector<QString> &legends)
{
    if(!this->connected)
        return;

    // ************* Adding New Graph *****************
    static QVector<QColor> colorList {Qt::green, Qt::red, Qt::magenta, Qt::blue, Qt::darkGreen, Qt::darkYellow, Qt::darkRed};
    int n = vals.size();

    for(int i=0; i<legends.size(); i++)
    {
        QCPGraph *g1 = NULL, *g2 = NULL;
        for(int j=0; j<ui->qPlot->legend->itemCount(); j++)
        {
            if(ui->qPlot->graph(2*j)->name() == legends[i])
            {
                g1 = ui->qPlot->graph(2*j);
                g2 = ui->qPlot->graph(2*j +1);
                break;
            }
        }

        if(!g1 && !g2)
        {
            int k = ui->qPlot->graphCount();
            g1 = ui->qPlot->addGraph();
            g2 = ui->qPlot->addGraph();

            g1->setName(legends[i]);

            g1->setPen(colorList[(k/2)%colorList.size()]);
            ui->qPlot->legend->removeItem(ui->qPlot->legend->itemCount()-1); // don't show two graphs in legend

            // Add a blue dot in end of graph
            g2->setPen(QPen(Qt::blue));
            g2->setLineStyle(QCPGraph::lsNone);
            g2->setScatterStyle(QCPScatterStyle::ssDisc);
        }

        //! add data to lines:
        g1->addData(key, vals[i]);
        //g1->removeDataBefore(key-120);  // delete memory after 2 minutes
        g2->clearData();
        g2->addData(key, vals[i]);

        double upper_bound = qMax(vals[i] , ui->qPlot->yAxis->range().upper) ;
        double lower_bound = qMin(vals[i] , ui->qPlot->yAxis->range().lower) ;
        ui->qPlot->yAxis->setRangeUpper(upper_bound);
        ui->qPlot->yAxis->setRangeLower(lower_bound);

        if(key > QDateTime::currentMSecsSinceEpoch()/2)
            ui->qPlot->xAxis->setDateTimeFormat("mm:ss");

        // ui->qPlot->yAxis->setRangeUpper(-3000);
        // ui->qPlot->yAxis->setRangeLower(3000);
    }

    // make key axis range scroll with the data (at a constant range size of 8):
    if(!freezed)
        ui->qPlot->xAxis->setRange(key+4, 20, Qt::AlignRight);
    ui->qPlot->replot();

    if(statusGItem->isVisible())
        statusGItem->setRotation(statusGItem->rotation()-5);

    //! calculate frames per second:
    /*
    static double lastFpsKey;
    static int frameCount;
    frameCount ++;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        lastFpsKey = key;
        frameCount = 0;
    }
    */

}

void PlotWidget::addValue(double val, double key, QString legend)
{
    if(key < 0)
        key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0 ;
    mKey = key;

    QVector<double> valVector(1, val);
    QVector<QString> legVector(1, legend);
    addValue(key, valVector, legVector);

    //    static int cntr = 0;
    //    cntr ++;
    //    addValue(cntr, val);
}

void PlotWidget::forceToPause()
{
    if(!freezed)
        ui->pauseButton->click();
}
void PlotWidget::forceToPlay()
{
    if(freezed)
        ui->pauseButton->click();
}

void PlotWidget::setConnected(bool connected)
{
    this->connected = connected;
}

void PlotWidget::setYAxisRange(double lower, double upper)
{
    ui->qPlot->yAxis->setRange(lower, upper);
}

PlotWidget::~PlotWidget()
{
    delete ui;
}

void PlotWidget::selectionChanged()
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
        if (item->selected() || graph->selected())
        {
            item->setSelected(true);
            graph->setSelected(true);
        }
    }
}

void PlotWidget::mousePress()
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

void PlotWidget::mouseWheel()
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

void PlotWidget::graphClicked(QCPAbstractPlottable *plottable)
{
    //    ui->vel_label->setText(plottable->name());
}


void PlotWidget::on_closeButton_clicked()
{
    emit closeMe(this->plotName);
}

void PlotWidget::on_minimizeButton_clicked(bool checked)
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

void PlotWidget::on_pauseButton_clicked(bool checked)
{
    freezed = checked;
}

void PlotWidget::on_recButton_clicked(bool checked)
{
    //if(checked)
}
}
