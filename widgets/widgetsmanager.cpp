//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#include "widgetsmanager.h"
#include "ui_widgetsmanager.h"
#include "plotwidget.h"
#include "scatterplotwidget.h"
#include "logger.h"

#include <QDateTime>
#include <QString>

namespace dbug
{
WidgetsManager::WidgetsManager(QWidget *parent) :
    QWidget(parent), ui(new Ui::WidgetsManager)
{
    ui->setupUi(this);

    //    centralLayout = new QVBoxLayout(this);
    //    centralLayout->setSpacing(2);
    //    centralLayout->setContentsMargins(1, 1, 1, 1);

    //    scrollArea = new QScrollArea(this);
    //    scrollArea->setWidgetResizable(true);
    //    centralLayout->addWidget(scrollArea);
    //    scrollLayout = new QVBoxLayout(this);

    //    scrollAreaWidgetContents = new QWidget(this);
    //    scrollLayout->addWidget(scrollAreaWidgetContents);
    //    scrollLayout->setSpacing(2);
    //    scrollLayout->setContentsMargins(0, 0, 0, 0);
    //    scrollArea->setWidget(scrollAreaWidgetContents);

    //    verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
    //    verticalLayout->setSpacing(2);
    //    verticalLayout->setContentsMargins(0, 0, 0, 0);
}

WidgetsManager::~WidgetsManager()
{
}

void WidgetsManager::plot(const PlotterPacket &packet)
{
    try {
        if(packet.values.empty())
            throw "";
        if(packet.legends.size() != packet.values.size())
            throw "";

        QString q_name = QString::fromStdString(packet.name);
        QVector<double> valuesQVec = QVector<double>::fromStdVector(packet.values);
        QVector<QString> legendsQVec;
        for(uint i=0; i<packet.legends.size(); i++)
            legendsQVec.append(QString::fromStdString(packet.legends[i]));

        if( !mPlotsMap.keys().contains(q_name))
        {
            PlotWidget* new_plot = new PlotWidget(ui->scrollAreaWidgetContents);
            ui->verticalLayout->addWidget(new_plot);
            new_plot->setName(q_name);
            new_plot->setLegendsFont(QFont("Ubuntu", 9));
            mPlotsMap.insert(q_name, new_plot);
            connect(new_plot, SIGNAL(closeMe(QString)), this, SLOT(turnOffWidget(QString)));
        }
        mPlotsMap[q_name]->addValue(packet.key, valuesQVec, legendsQVec);

        if(ui->recordButton->isChecked())
        {
            Logger::getInstance()->addLogJson(packet.key, packet.toJson());
        }

    }
    catch (const char* msg) {
        qDebug() << "Error: PlotManagerWidget: " << msg << endl;
    }
}

void WidgetsManager::scatter(const ScatterPacket &packet)
{
    QString q_name = QString::fromStdString(packet.name);
    if(!mScattersMap.keys().contains(q_name))
    {
        ScatterPlotWidget *new_scatter = new ScatterPlotWidget(ui->scrollAreaWidgetContents);
        ui->verticalLayout->addWidget(new_scatter);
        mScattersMap.insert(q_name, new_scatter);
    }
    mScattersMap[q_name]->addData(QPointF(packet.x, packet.y));
}

void WidgetsManager::turnOffWidget(const QString &name)
{
    if(mPlotsMap.contains(name)) {
        PlotWidget* quitted_plot = mPlotsMap[name];
        mPlotsMap.remove(name);
        quitted_plot->deleteLater();
    }
}

void WidgetsManager::on_saveToButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly);
    ui->saveToDirLineEdit->setText(dir);
}

void WidgetsManager::on_recordButton_clicked(bool checked)
{
    if(checked) {
        bool res = Logger::getInstance()->setLogDir(ui->saveToDirLineEdit->text());
        if(res) {
            ui->saveToFileLabel->setText("/" + Logger::getInstance()->getFileInfo().fileName());
        }
        else {

        }
    }
    else {

    }

}


void WidgetsManager::on_palyButton_clicked()
{
    foreach (PlotWidget* pw, mPlotsMap.values())
        pw->forceToPlay();
}

void WidgetsManager::on_pauseButton_clicked()
{
    foreach (PlotWidget* pw, mPlotsMap.values())
        pw->forceToPause();
}

void WidgetsManager::savePlotterPacket(double key, const dbug::PlotterPacket &packet)
{
    QString saveFolder = ui->saveToDirLineEdit->text();
    if(saveFolder.isEmpty())
        return;

    QFileInfo outFileInfo;
    outFileInfo.setFile(saveFolder, "log.txt");
    QFile outFile(outFileInfo.absoluteFilePath());

    //Logger::getInstance()->
}
}

