#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>
#include <iostream>
#include <QMap>
#include <QFile>
#include <QMutex>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QObject>
#include <QVector>
#include <QDir>

class Logger : public QObject
{
    Q_OBJECT

    QMutex mtx_;
    static Logger* instance;
    static const std::string path;
    Logger();
    ~Logger();    
    QTimer timer;

public:
    static Logger* getInstance();

    bool setLogDir(const QString &dir);

    QFileInfo getFileInfo() const;

    QDir log_dir;
    QFile log_file;
    QMap<double, QVector<QString> > logsMap;

    void addLogJson(double key, const std::string& str);
    void writeToFile(double key, const QVector<QString> &data);

private slots:
    void saveToFile();






};

#endif // LOGGER_H
