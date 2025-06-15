#include "../headers/mainwindow.hpp"
#include "uis/ui_mainwindow.h"

#include "../headers/config_file_handler.hpp"

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize( 500, 250 );

    make_configurations();
    set_tab_widgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_app( QApplication *app )
{
    this->app = app;
}

void MainWindow::centralize_window()
{
    if (app == nullptr)
    {
        qDebug() << "NULL APPLICATION IN MAIN WINDOW!";
        return;
    }

    int width = frameGeometry().width();
    int height = frameGeometry().height();

    QScreen *screen = app->primaryScreen();
    int screen_width = screen->geometry().width();
    int screen_height = screen->geometry().height();

    setGeometry( (screen_width / 2) - (width / 2), (screen_height / 2) - (height / 2), width, height );
}

void MainWindow::make_configurations()
{
    First_Open_Config first_time_window;
    first_time_window.setModal( true );
    first_time_window.setFixedSize( 600, 250 );

    if (!QFile::exists( "configs.json" ))
    {
        first_time_window.exec();

        return;
    }

    if (!Config_file_handler::get_Instance().check_config_file( first_time_window.get_json_names() ))
    {
        first_time_window.exec();
    }
}

void MainWindow::set_tab_widgets()
{
    ui->tabWidget->addTab( new Playlist(), "Player" );
    ui->tabWidget->addTab( new Downloader(), "Downloader" );
    //ui->tabWidget->addTab( new Theme( ui->tabWidget, app ), "Theme" );
}
