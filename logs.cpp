#include "logs.hpp"

void Logs::create_log( const QStringList &log_messages, const ERROR_TYPE &error, const std::source_location &location )
{
    create_directory();

    auto current_time = QDateTime::currentDateTime();
    QFile log_file( "Logs/Log - " + current_time.toString() + ".txt" );

    if (!log_file.open( QFile::WriteOnly | QFile::Text ))
    {
        exit(EXIT_FAILURE);
    }

    QTextStream output( &log_file );
    output << "ERROR TYPE: " << get_error_message( error ) << '\n';
    output << "LOG MESSAGE:\n";

    for ( const auto &message : log_messages )
    {
        output << '\t' << message << '\n';
    }

    output << '\n' << "ERROR CAUSED BY: "<< location.file_name() << ":" << location.line();
    output << '\n' << "TIME OF ERROR: " << current_time.toString();

    log_file.flush();
    log_file.close();
}

QString Logs::get_error_message( const ERROR_TYPE &error )
{
    switch ( error )
    {
        case ERROR_TYPE::FATAL:
            return "FATAL ERROR! PLEASE READ LOG MESSAGE WITH ATTENTION";

        case ERROR_TYPE::NON_FATAL:
            return "NON-FATAL ERROR! PROBALY YOU CAN IGNORE THE LOG MESSAGE!";
    }

    return {};
}

void Logs::create_directory()
{
    QDir log_dir;
    if (log_dir.exists( "Logs" ))
    {
        qDebug() << "LOG FOLDER ALREADY EXISTS!";
        return;
    }

    log_dir.mkdir( "Logs" );
}
