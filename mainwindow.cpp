#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize( 500, 250 );

    m_config_file.setFileName( m_config_file_name );
    config_app_directories();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::centralize_window( const QApplication &app )
{
    int width = frameGeometry().width();
    int height = frameGeometry().height();

    QScreen *screen = app.primaryScreen();
    int screen_width = screen->geometry().width();
    int screen_height = screen->geometry().height();

    setGeometry( (screen_width / 2) - (width / 2), (screen_height / 2) - (height / 2), width, height );
}

void MainWindow::config_app_directories()
{
    QFile config_file( m_config_file_name );
    if (!config_file.exists())
    {
        create_config_file();
    }

    if (m_config_file.open( QFile::ReadOnly ))
    {
        QStringList error_list;
        QString error_1 = "ERROR IN OPENING CONFIG_FILE.TXT!";
        QString error_2 = "PLEASE REOPEN THE PROGRAM OR DELETE CONFIG_FILE.TXT IN APP FOLDER!";

        error_list.push_back( error_1 );
        error_list.push_back( error_2);

        log.create_log( error_list );
        exit( EXIT_FAILURE );
    }
}

void MainWindow::create_config_file()
{
    if (!m_config_file.open( QFile::WriteOnly ))
    {
        close();
    }

    QJsonDocument document;
    QJsonObject object;

    object["app_dir"];
    document.setObject( object );

    QByteArray data_json = document.toJson();

    m_config_file.write( data_json );
    m_config_file.close();
}
