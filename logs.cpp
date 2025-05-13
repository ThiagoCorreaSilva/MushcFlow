#include "logs.hpp"

void Logs::create_log( const QStringList &log_messages, const ERROR_TYPE &error, const std::source_location &location )
{
    create_directory();

    QString current_time = QDateTime::currentDateTime().toString();
    current_time.replace( QRegularExpression(":"), "-" );

    QFile log_file( "Logs/log - " + current_time + ".txt" );

    if (!log_file.open( QFile::WriteOnly | QFile::Text ))
    {
        return;
    }

    QTextStream output( &log_file );
    output << "ERROR TYPE: " << get_error_message( error ) << '\n';
    output << "LOG MESSAGE:\n";

    for ( const auto &message : log_messages )
    {
        output << '\t' << message << '\n';
    }

    output << '\n' << "ERROR CAUSED BY: "<< location.file_name() << ":" << location.line();

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

    qDebug() << "LOG FOLDER CREATED SUCCESSFULLY";
}
