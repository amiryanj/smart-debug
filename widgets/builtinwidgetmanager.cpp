//========================================================================
/*!
  @file
  @class
  @date    1/31/2016
  @brief
  @author  Javad Amiryan, (C) 2016
*/
//========================================================================

#include "builtinwidgetmanager.h"
#include "ui_builtinwidgetmanager.h"
#include "plotterwidget.h"
#include "scatterwidget.h"
#include "logger.h"

#include <QDateTime>
#include <QString>

namespace sdbug
{
BuiltInWidgetManager::BuiltInWidgetManager(QWidget *parent) :
    QWidget(parent), ui(new Ui::BuiltInWidgetManager)
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

BuiltInWidgetManager::~BuiltInWidgetManager()
{
}

void BuiltInWidgetManager::addPlotter(QString name, PlotterWidget *plot_widget)
{
    ui->verticalLayout->addWidget(plot_widget);
    mPlotsMap.insert(name, plot_widget);
}

void BuiltInWidgetManager::addScatter(QString name, ScatterWidget *scatter_widget)
{
    ui->verticalLayout->addWidget(scatter_widget);
    mScattersMap.insert(name, scatter_widget);
}

//void WidgetsManager::plot(const PlotterPacket &packet)
//{
//    try {
//        if(packet.values.empty())
//            throw "";
//        if(packet.legends.size() != packet.values.size())
//            throw "";

//        QString q_name = QString::fromStdString(packet.name);
//        QVector<double> valuesQVec = QVector<double>::fromStdVector(packet.values);
//        QVector<QString> legendsQVec;
//        for(uint i=0; i<packet.legends.size(); i++)
//            legendsQVec.append(QString::fromStdString(packet.legends[i]));

//        if( !mPlotsMap.keys().contains(q_name))
//        {
//            PlotWidget* new_plot = new PlotWidget(ui->scrollAreaWidgetContents);
//            ui->verticalLayout->addWidget(new_plot);
//            new_plot->setName(q_name);
//            new_plot->setLegendsFont(QFont("Ubuntu", 9));
//            mPlotsMap.insert(q_name, new_plot);
//            connect(new_plot, SIGNAL(closeMe(QString)), this, SLOT(turnOffWidget(QString)));
//        }
//        mPlotsMap[q_name]->addValue(packet.key, valuesQVec, legendsQVec);

//        if(ui->recordButton->isChecked())
//        {
//            Logger::getInstance()->addLogJson(packet.key, packet.toJson());
//        }

//    }
//    catch (const char* msg) {
//        qDebug() << "Error: PlotManagerWidget: " << msg << endl;
//    }
//}

//void WidgetsManager::scatter(const ScatterPacket &packet)
//{
//    QString q_name = QString::fromStdString(packet.name);
//    if(!mScattersMap.keys().contains(q_name))
//    {
//        ScatterWidget *new_scatter = new ScatterWidget(ui->scrollAreaWidgetContents);
//        ui->verticalLayout->addWidget(new_scatter);
//        mScattersMap.insert(q_name, new_scatter);
//    }
//    mScattersMap[q_name]->addData(QPointF(packet.x, packet.y));
//}

void BuiltInWidgetManager::turnOffWidget(const QString &name)
{
    if(mPlotsMap.contains(name)) {
        PlotterWidget* quitted_plot = mPlotsMap[name];
        mPlotsMap.remove(name);
        quitted_plot->deleteLater();
    }
}

void BuiltInWidgetManager::on_saveToButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui->saveToDirLineEdit->text(), QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
        ui->saveToDirLineEdit->setText(dir);
}

void BuiltInWidgetManager::on_recordButton_clicked(bool checked)
{
    if(checked) {
        Logger::setLogDir(ui->saveToDirLineEdit->text().toStdString());
        Logger::startTimer(3000);
    }
    else {
        Logger::stopTimer();
    }
}

void BuiltInWidgetManager::on_palyButton_clicked()
{
    foreach (PlotterWidget* pw, mPlotsMap.values())
        pw->forceToPlay();
}

void BuiltInWidgetManager::on_pauseButton_clicked()
{
    foreach (PlotterWidget* pw, mPlotsMap.values())
        pw->forceToPause();
}

//void WidgetsManager::savePlotterPacket(double key, const dbug::PlotterPacket &packet)
//{
//    QString saveFolder = ui->saveToDirLineEdit->text();
//    if(saveFolder.isEmpty())
//        return;

//    QFileInfo outFileInfo;
//    outFileInfo.setFile(saveFolder, "log.txt");
//    QFile outFile(outFileInfo.absoluteFilePath());

//    //Logger::getInstance()->
//}
}

