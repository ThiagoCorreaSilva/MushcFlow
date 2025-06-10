#pragma once

#include <QRegularExpression>
#include <QDesktopServices>
#include <QTextStream>
#include <QMessageBox>
#include <QTranslator>
#include <QDateTime>
#include <QWidget>
#include <QFile>
#include <QUrl>
#include <QDir>

#include <source_location>

enum class ERROR_TYPE
{
    FATAL,
    NON_FATAL
};

class Logs
{
    private:
        Logs() = default;
        ~Logs() = default;

        void create_directory();
        void show_message_box( QWidget *parent, const QString &log_name );
        QString get_error_message( const ERROR_TYPE &error );

    public:

        static Logs &get_Instance()
        {
            static Logs instance;
            return instance;
        }

        void create_log( const QStringList &log_message, QWidget *parent = nullptr, const ERROR_TYPE &error = ERROR_TYPE::FATAL, const std::source_location &location = std::source_location::current() );
};
