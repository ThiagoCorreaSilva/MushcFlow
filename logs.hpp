#pragma once

#include <QTextStream>
#include <QTime>
#include <QFile>
#include <QDir>

struct ERROR_TYPE
{
    static QString FATAL;;
    static QString NON_FATAL;
};

QString ERROR_TYPE::FATAL = "FATAL ERROR! PLEASE READ LOG MESSAGE WITH ATTENTION!";
QString ERROR_TYPE::NON_FATAL = "NON-FATAL ERROR! PROBALY YOU CAN IGNORE LOG MESSAGE!";

class Logs
{
    private:
        void create_directory();

    public:
        void create_log( const QStringList &log_message, const ERROR_TYPE &error_type );
};
