#ifndef _LOGGER_H
#define _LOGGER_H

#include <mutex>
#include <string>
#include <fstream>
#include <iostream>
#include <QTimer>

class Logger : public QObject
{
    Q_OBJECT

    static std::string log_dir;
    static QTimer timer;
    mutable std::mutex mtx_;
    std::string logFileName;
    std::ofstream logFile;
    bool headerIsWritten;

    std::map<double, std::vector<std::string> > jsonLogMap;

    std::vector<std::string> csvColumnTitles;
    std::map<double, std::map<std::string, double> > csvLogMap;

public:
    Logger();
    ~Logger();
    static bool setLogDir(const std::string &dir);
    static std::string getLogDir();
    static void startTimer(int interval, Qt::TimerType timerType = Qt::VeryCoarseTimer);
    static void stopTimer();

    std::string getFileName() const;
    void setFileName(const std::string &file_name);

    void addLogCsv(double key, const std::vector<std::string> &legends, std::vector<double> vals);
    void addLogJson(double key, const std::string& str);
    void writeToJsonFile(double key, const std::vector<std::string> &data);
    //void writeToCsvFile(double key, const QVector<QString> &data);

private slots:
    void persistJsonLogs();
    void persistCsvLogs();

};

#endif // LOGGER_H
