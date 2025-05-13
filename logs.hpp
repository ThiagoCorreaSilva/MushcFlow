#pragma once

#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QDir>

enum class ERROR_TYPE
{
    FATAL,
    NON_FATAL
};

class Logs
{
    private:
        void create_directory();
        QString get_error_message( const ERROR_TYPE &error );

    public:
        void create_log( const QStringList &log_message, const ERROR_TYPE &error = ERROR_TYPE::FATAL );
};
