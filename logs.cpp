#include "logs.hpp"

void Logs::create_log( const QStringList &log_messages, const ERROR_TYPE &error_tpye )
{
    create_directory();

    auto current_time = QTime::currentTime();
    QFile log_file( "Logs/Log - " + current_time.toString() + ".txt" );

    if (!log_file.open( QFile::WriteOnly | QFile::Text ))
    {
        exit(EXIT_FAILURE);
    }

    QTextStream output( &log_file );
    output << "ERROR TYPE: " << error_type << '\n';
    output << "ERROR:\n";

    for ( const auto &message : log_messages )
    {
        output << '\t' << message << '\n';
    }

    output << "\nTIME OF ERROR: " << current_time.toString();

    log_file.flush();
    log_file.close();
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
