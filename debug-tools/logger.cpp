#include "logger.h"
#include <map>
#include <string>
#include <fstream>
#include <cstdio>

using namespace std;

string Logger::log_dir;
QTimer Logger::timer;

Logger::Logger()
{
    connect(&timer, &QTimer::timeout, this, &Logger::persistCsvLogs);
    headerIsWritten = false;
}

Logger::~Logger()
{
}

bool Logger::setLogDir(const string &dir)
{
    if(dir.empty())
        return false;
    if(dir.back() != '/')
        log_dir = dir;
    log_dir.append("/");
#ifdef WIN32
    if(!CreateDirectory(log_dir.c_str(), NULL))
        return false;
#else
    char command[255];
    sprintf(command, "mkdir -p %s", log_dir.c_str());
    system(command);
#endif
    return true;
}

string Logger::getLogDir()
{
    return log_dir;
}

void Logger::startTimer(int interval, Qt::TimerType timerType)
{
    timer.setTimerType(timerType);
    timer.start(interval);
}

void Logger::stopTimer()
{
    timer.stop();
}

string Logger::getFileName() const
{
    return logFileName;
}

void Logger::setFileName(const string &file_name)
{
    logFileName = file_name;
}

void Logger::addLogCsv(double key, const std::vector<std::string> &legends, std::vector<double> vals)
{
    Q_ASSERT(legends.size() == vals.size());

    mtx_.lock();
    for(unsigned int i=0; i<legends.size(); i++) {
        if(std::find(csvColumnTitles.begin(), csvColumnTitles.end(), legends[i]) == csvColumnTitles.end())
            csvColumnTitles.push_back(legends[i]);
        csvLogMap[key][legends[i]] = vals[i];
    }
    mtx_.unlock();
}

void Logger::addLogJson(double key, const std::string &str)
{
    mtx_.lock();
    jsonLogMap[key].push_back(str);
    mtx_.unlock();
}

void Logger::persistJsonLogs()
{
    while(!jsonLogMap.empty())
    {
        double first_key = jsonLogMap.begin()->first;
        vector<string> first_vec = jsonLogMap.begin()->second;
        writeToJsonFile(first_key, first_vec);
        mtx_.lock();
        jsonLogMap.erase(first_key);
        mtx_.unlock();
    }
}

void Logger::persistCsvLogs()
{
    try
    {
        logFile.open(log_dir + logFileName, ios::out | ios::app | ios::ate);
        if(!logFile.is_open())
            throw "Can not open log file";

        if(!headerIsWritten)
        {
            cout << "Writing to " << logFileName << endl;
            /// write csv header
            logFile << "key, ";
            for(unsigned int i=0; i<csvColumnTitles.size(); i++) {
                logFile << csvColumnTitles[i];
                if(i!=csvColumnTitles.size()-1)
                    logFile << ", ";
                else
                    logFile << "\n";
            }
            headerIsWritten = true;
        }

        while(!csvLogMap.empty())
        {
            if(csvColumnTitles.empty())
                throw "Data column not found";

            double first_key = csvLogMap.begin()->first;
            map<string, double> first_val = csvLogMap.begin()->second;

            logFile << first_key << ", ";
            for(unsigned int i=0; i<csvColumnTitles.size(); i++)
            {
                string col = csvColumnTitles[i];
                if(first_val.find(col) != first_val.end())
                    logFile << std::to_string(first_val[col]).c_str();
                else
                    logFile << "nan";
                if(i!=csvColumnTitles.size()-1)
                    logFile << ", ";
                else
                    logFile << "\n";
            }

            mtx_.lock();
            csvLogMap.erase(first_key);
            mtx_.unlock();
        }

        logFile.flush();
        logFile.close();
    }

    catch(const char* err) {
        cerr << err << endl;
    }

}

void Logger::writeToJsonFile(double key, const std::vector<std::string> &data)
{
    if(!logFile.is_open()) {
        logFile.open(logFileName, ios::out | ios::ate | ios::app);
    }

    if(logFile.is_open())
    {
        std::string str;
        char buffer[255];
        sprintf(buffer, "{\n\t\"key\": %3f, \n\t\"log\":[\t", key);
        str.append(buffer);
        for (unsigned int i=0; i<data.size(); i++)
        {
            string sub_str = data[i];
            str.append(sub_str);
            //if(i < data.size()-1)
            str.append("\n\t\t");
        }
        str.append("] }\n");
        logFile << str.c_str();
        logFile.flush();
    }
}
