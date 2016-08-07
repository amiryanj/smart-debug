#include "logger.h"
#include <map>
#include <string>
#include <fstream>
#include <cstdio>

const std::string Logger::path = "../../logs/";
Logger* Logger::instance = NULL;

Logger::Logger()
{
    connect(&timer, &QTimer::timeout, this, &Logger::saveToFile);
}

Logger::~Logger()
{
}

Logger *Logger::getInstance()
{
    if(instance == NULL)
        instance = new Logger;
    return instance;
}

bool Logger::setLogDir(const QString &dir)
{
    log_dir.setPath(dir);
    if(!log_dir.mkpath(log_dir.absolutePath()))
        return false;
    QFileInfo file_info;
    QString file_name = QString("log_%1.json").arg(QDateTime::currentDateTime().toString("d|MMM|yyyy_hh-mm"));
    file_info.setFile(log_dir, file_name);
    log_file.setFileName(file_info.absoluteFilePath());
    timer.start(3000);
    return true;
}

QFileInfo Logger::getFileInfo() const
{
    return QFileInfo(log_file.fileName());
}

void Logger::addLogJson(double key, const std::string &str)
{
    mtx_.lock();
    logsMap[key].push_back(QString::fromStdString(str));
    mtx_.unlock();
}

void Logger::saveToFile()
{
    while(logsMap.size() > 1)
    {
        double first_key = logsMap.firstKey();
        QVector<QString> first_vec = logsMap.first();
        mtx_.lock();
        logsMap.remove(first_key);
        mtx_.unlock();
        writeToFile(first_key, first_vec);
    }
}

void Logger::writeToFile(double key, const QVector<QString> &data)
{

    if(log_file.isOpen() || log_file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        std::string str;
        char buffer[255];
        sprintf(buffer, "{\n\t\"key\": %3f, \n\t\"log\":[\t", key);
        str.append(buffer);
        for (int i=0; i<data.size(); i++)
        {
            QString sub_str = data[i];
            str.append(sub_str.toStdString());
            //if(i < data.size()-1)
                str.append("\n\t\t");
            //else
                //str
        }
        str.append("] }\n");
        log_file.write(str.c_str());
        log_file.flush();
    }
}
